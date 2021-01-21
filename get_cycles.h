// 2019-06-23
//
// The achitecture's header must define get_cycles()
// function or macro. get_cycles_{start,end}() are optional.

#ifndef GET_CYCLES_H
#define GET_CYCLES_H

#ifndef ARCH
#define ARCH x86
#endif

#define __stringify(x) #x
#define   stringify(x) __stringify(x)
#define __concat(x, y) x ## y
#define   concat(x, y) __concat(x, y)

#define prefix_with_arch(str) stringify(concat(ARCH, str))

#include prefix_with_arch(_get_cycles.h)

#ifndef get_cycles_start
#define get_cycles_start get_cycles
#endif

#ifndef get_cycles_end
#define get_cycles_end get_cycles
#endif

#endif // GET_CYCLES_H
