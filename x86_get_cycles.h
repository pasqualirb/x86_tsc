// 2019-06-18
//
// Issues with Time Stamp Counter:
//
//   See cpu flags in /proc/cpuinfo
//
// - With constant_tsc flag, TSC is incremented by a fixed
//   frequency (i.e. it's not related to clock cycles).
//   Otherwise, it's incremented as CPU clock (i.e. TSC
//   counts clock cycles); the CPU clock frequency may vary.
//
// - With nonstop_tsc flag, TSC does not stop when clock
//   halts. It's invariant between CPUs?
//
// - With rdtscp flag, the rdtscp instruction is available.
//   This instruction waits for previous instructions that
//   are running out-of-order to complete on the superscalar
//   pipeline before read TSC. However, it does not prevent
//   subsequent instructions to run out-of-order.
//
// - The cpuid instruction ensures that previous and
//   subsequent instructions will not run out-of-order.
//
// See <https://stackoverflow.com/a/39002894>,
//     <https://stackoverflow.com/a/51907627>,
//     <https://stackoverflow.com/q/12631856>,
//     <https://stackoverflow.com/a/48523760>,
//     <http://blog.tinola.com/?e=54>.
//
// See Intel's paper "How to Benchmark Code Execution Times
// on Intel IA-32 and IA-64 Instruction Set Architectures"
// by Grabriele Paoloni (2019-09).

#include <x86intrin.h> // __rdtsc(), __rdtscp()

#include <stdint.h>    // uint64_t

#define get_cycles       rdtsc
#define get_cycles_start ____get_cycles_start
#define get_cycles_end   asm_get_cycles_end

#define rdtsc()         __rdtsc()
#define rdtscp_cpu(cpu) __rdtscp(cpu)
#define rdtscp()        ({ unsigned cpu; __rdtscp(&cpu); })

#define cpuid() \
	asm volatile ("cpuid" : : : "eax", "ebx", "ecx", "edx", "memory")

#define stop_and_drain_pipeline() cpuid()

static inline uint64_t __attribute__((always_inline))
____get_cycles_start()
{
	stop_and_drain_pipeline();
	return rdtsc();
}

static inline uint64_t __attribute__((always_inline))
____get_cycles_end()
{
	uint64_t cycles = rdtscp();
	stop_and_drain_pipeline();
	return cycles;
}

#define asm_rdtsc(lo, hi)  asm volatile ("rdtsc"  : "=a" (lo), "=d" (hi))
#define asm_rdtscp(lo, hi) asm volatile ("rdtscp" : "=a" (lo), "=d" (hi))

static inline uint64_t __attribute__((always_inline))
asm_get_cycles_start()
{
	uint32_t lo, hi;
	stop_and_drain_pipeline();
	asm_rdtsc(lo, hi);
	return ((uint64_t) hi << 32) | lo;
}

static inline uint64_t __attribute__((always_inline))
asm_get_cycles_end()
{
	uint32_t lo, hi;
	asm_rdtscp(lo, hi);
	stop_and_drain_pipeline();
	return ((uint64_t) hi << 32) | lo;
}
