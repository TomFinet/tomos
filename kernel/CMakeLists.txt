ENABLE_LANGUAGE(C ASM)

SET(CMAKE_C_FLAGS "-O2 -ffreestanding -fbuiltin -Wno-write-strings -Wall -mno-red-zone -g")

SET(KERNEL_SRCS
	startup/startup.s

	memory/gdt.s
	memory/gdt.c
	memory/mm.c
	memory/paging.c
	memory/vmm.c

	interrupts/idt.s
	interrupts/isr.s
	interrupts/idt.c
	interrupts/isr.c

	klib/kstdio.c
	klib/kstring.c

	io.c
	serial.c
	list.c
	panic.c
)

if(KTESTS)
	SET(ENTRY_SRCS
		tests/ktest.c
		tests/memory/mm_tests.c
		tests/ktest_entry.c
	)
else()
	SET(ENTRY_SRCS
		kentry.c
	)
endif()

ADD_EXECUTABLE(tomos ${KERNEL_SRCS} ${ENTRY_SRCS})

ADD_CUSTOM_TARGET(preprocess_linker_script
    COMMAND "${CMAKE_C_COMPILER}" -E -P -x c ${CMAKE_SOURCE_DIR}/scripts/i386-pc.ld.in -o ${CMAKE_SOURCE_DIR}/build/i386-pc.ld -DKTESTS
)

SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/build/i386-pc.ld)
SET_TARGET_PROPERTIES(tomos PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
TARGET_LINK_LIBRARIES(tomos gcc)
TARGET_LINK_OPTIONS(tomos PRIVATE LINKER:-T ${LINKER_SCRIPT} -nostdlib)

TARGET_INCLUDE_DIRECTORIES(tomos PRIVATE ${CMAKE_SOURCE_DIR})

INSTALL(TARGETS tomos RUNTIME DESTINATION boot)