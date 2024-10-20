#include <mac_memory_mapping/mmap_private.hpp>

#include <sys/mman.h>


namespace MMap
{

FilePMMapper::FilePMMapper(const int fd, const size_t offset, const size_t size)
	: mmapper_(fd, offset, size, MMapFileType::P) {
}

void FilePMMapper::ReloadMemoryArea(const size_t offset, const size_t len) {
	mmapper_.Sync(offset, len, MMapSync::Invalidate);
}

size_t FilePMMapper::Offset() const noexcept {
	return mmapper_.Offset();
}

size_t FilePMMapper::Size() const noexcept {
	return mmapper_.Size();
}

} // namespace mmap
