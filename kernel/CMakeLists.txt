ENABLE_LANGUAGE(C ASM)

SET(CMAKE_C_FLAGS "-O2 -ffreestanding -fbuiltin -Wno-write-strings -Wall -mno-red-zone")

SET(KERNEL_SRCS
	startup/boot.s
	startup/gdt.s
	startup/gdt.c

	interrupts/idt.s
	interrupts/isr.s
	interrupts/idt.c
	interrupts/isr.c

	klib/kstdio.c
	klib/kstring.c

	io.c
	serial.c

	kmain.c
)

ADD_EXECUTABLE(tomos ${KERNEL_SRCS})

SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/kernel/i386-pc.ld)
SET_TARGET_PROPERTIES(tomos PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
TARGET_LINK_LIBRARIES(tomos gcc)
TARGET_LINK_OPTIONS(tomos PRIVATE LINKER:-T ${LINKER_SCRIPT} -nostdlib)

TARGET_INCLUDE_DIRECTORIES(tomos PRIVATE ${CMAKE_SOURCE_DIR})

INSTALL(TARGETS tomos RUNTIME DESTINATION boot)