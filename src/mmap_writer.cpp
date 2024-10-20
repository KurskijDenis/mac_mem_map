#include <mac_memory_mapping/mmap_writer.hpp>

#include <sys/mman.h>


namespace MMap
{

FileWMMapper::FileWMMapper(
	const int fd,
	const size_t offset,
	const size_t size,
	const bool sync_after_write)
	: mmapper_(fd, offset, size, MMapFileType::W)
	, sync_after_write_(sync_after_write) {
}

void FileWMMapper::Sync(const size_t offset, const size_t len, const MMapSync flag) {
	mmapper_.Sync(offset, len, flag);
}

size_t FileWMMapper::Offset() const noexcept {
	return mmapper_.Offset();
}

size_t FileWMMapper::Size() const noexcept {
	return mmapper_.Size();
}

} // namespace mmap
