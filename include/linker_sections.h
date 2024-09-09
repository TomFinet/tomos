#pragma once

#ifdef KTESTS
#define SECT_KTESTS                                                            \
	.= ALIGN(4);                                                           \
	_ktest_start =.;                                                       \
	KEEP(*(.ktest))                                                        \
	_ktest_end =.;
#else
#define SECT_KTESTS
#endif
