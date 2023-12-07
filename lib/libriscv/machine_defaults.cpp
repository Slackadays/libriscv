/**
 * Some default implementations of OS-specific I/O routines
 * stdout: Used by write/writev system calls
 * stdin:  Used by read/readv system calls
 * rdtime: Used by the RDTIME/RDTIMEH instructions
**/
#include "machine.hpp"

#include <chrono>
extern "C" {
#ifdef _WIN32
	int write(int fd, const void *buf, unsigned count);
#else
	ssize_t write(int fd, const void *buf, size_t count);
#endif
}

namespace riscv
{
	// Default: Stdout allowed
	template <int W>
	void Machine<W>::default_printer(const Machine<W>&, const char* buffer, size_t len) {
		std::ignore = ::write(1, buffer, len);
	}
	// Default: Stdin *NOT* allowed
	template <int W>
	long Machine<W>::default_stdin(const Machine<W>&, char* /*buffer*/, size_t /*len*/) {
		return 0;
	}

	// Default: RDTIME produces monotonic time with *millisecond*-granularity
	template <int W>
	uint64_t Machine<W>::default_rdtime(const Machine<W>&) {
		auto now = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
	}

#ifndef __GNUG__ /* Workaround for GCC bug */
	template struct Machine<4>;
	template struct Machine<8>;
	INSTANTIATE_128_IF_ENABLED(Machine);
#endif
}