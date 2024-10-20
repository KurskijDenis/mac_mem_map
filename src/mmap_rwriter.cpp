#include <mac_memory_mapping/mmap_rwriter.hpp>

#include <sys/mman.h>


namespace MMap
{

FileRWMMapper::FileRWMMapper(
	const int fd,
	const size_t offset,
	const size_t size,
	const bool sync_after_write)
	: mmapper_(fd, offset, size, MMapFileType::RW)
	, sync_after_write_(sync_after_write) {
}

void FileRWMMapper::Sync(const size_t offset, const size_t len, const MMapSync flag) {
	mmapper_.Sync(offset, len, flag);
}

size_t FileRWMMapper::Offset() const noexcept {
	return mmapper_.Offset();
}

size_t FileRWMMapper::Size() const noexcept {
	return mmapper_.Size();
}

} // namespace mmap
