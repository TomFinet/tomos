ENABLE_LANGUAGE(C ASM)

SET(CMAKE_C_FLAGS "-O0 -ffreestanding -fbuiltin -Wno-write-strings -Wall -mno-red-zone -g")

SET(KERNEL_SRCS
	startup/startup.S

	memory/gdt.s
	memory/gdt.c
	memory/frame.c
	memory/paging.c
  	memory/kslab.c
  	memory/kalloc.c

	interrupts/idt.s
	interrupts/isr.S
	interrupts/idt.c
	interrupts/isr.c

	klib/kstdio.c
	klib/kstring.c
  	klib/kmath.c

	io.c
	serial.c
	list.c
	panic.c
)

if(KTESTS)
	SET(ENTRY_SRCS
		tests/ktest.c
		tests/klib/kbitmap_tests.c
		tests/memory/frame_tests.c
    	tests/memory/paging_tests.c
    	tests/memory/kslab_tests.c
    	tests/memory/kalloc_tests.c
		tests/ktest_entry.c
	)
else()
	SET(ENTRY_SRCS
		kentry.c
	)
endif()

ADD_EXECUTABLE(tomos ${KERNEL_SRCS} ${ENTRY_SRCS})

ADD_CUSTOM_TARGET(linker_script
	COMMAND "${CMAKE_C_COMPILER}" -E -P -I"${CMAKE_SOURCE_DIR}"/include/
	-x c ${CMAKE_SOURCE_DIR}/scripts/i386-pc.ld.in -o ${CMAKE_SOURCE_DIR}/build/i386-pc.ld -DKTESTS
)

SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/build/i386-pc.ld)
SET_TARGET_PROPERTIES(tomos PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
TARGET_LINK_LIBRARIES(tomos gcc)
TARGET_LINK_OPTIONS(tomos PRIVATE LINKER:-T ${LINKER_SCRIPT} -nostdlib)

TARGET_INCLUDE_DIRECTORIES(tomos PRIVATE ${CMAKE_SOURCE_DIR})

INSTALL(TARGETS tomos RUNTIME DESTINATION boot)