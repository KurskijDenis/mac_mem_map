#include <mac_memory_mapping/utils.hpp>

#include <iostream>

#include <sys/mman.h>
#include <unistd.h>


namespace MMap
{

size_t PageSize() {
	const static int page_size = sysconf(_SC_PAGESIZE);

	if (page_size < 1) {
		throw std::runtime_error("page size has incorrect value");
	}

	return static_cast<size_t>(page_size);
}

size_t RemainingFileSize(const size_t offset, const size_t file_size) {
	if (offset > file_size) {
		throw std::runtime_error("offset is out of file");
	}

	return file_size - offset;
}

} // namespace mmap
