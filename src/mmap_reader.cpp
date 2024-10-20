#include <mac_memory_mapping/mmap_reader.hpp>

#include <sys/mman.h>


namespace MMap
{

FileRMMapper::FileRMMapper(const int fd, const size_t offset, const size_t size)
	: mmapper_(fd, offset, size, MMapFileType::R) {
}

void FileRMMapper::ReloadMemoryArea(const size_t offset, const size_t len) {
	mmapper_.Sync(offset, len, MMapSync::Invalidate);
}

size_t FileRMMapper::Offset() const noexcept {
	return mmapper_.Offset();
}

size_t FileRMMapper::Size() const noexcept {
	return mmapper_.Size();
}

} // namespace mmap
