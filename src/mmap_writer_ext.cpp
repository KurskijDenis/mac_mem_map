#include <mac_memory_mapping/mmap_writer_ext.hpp>

#include <mac_memory_mapping/utils.hpp>

#include <sys/mman.h>


namespace MMap
{

FileWMMapperExt::FileWMMapperExt(
	const char* const file_name,
	const size_t offset,
	const int access_mode,
	const bool sync_after_write)
	: guard_(file_name, OpenFileMode::RW, access_mode)
	, mmapper_{guard_.FileDescriptor(), offset, RemainingFileSize(offset, guard_.FileSize()), MMapFileType::W}
	, sync_after_write_(sync_after_write) {
}

void FileWMMapperExt::Sync(const size_t offset, const size_t len, const MMapSync flag) {
	mmapper_.Sync(offset, len, flag);
}

size_t FileWMMapperExt::Offset() const noexcept {
	return mmapper_.Offset();
}

size_t FileWMMapperExt::Size() const noexcept {
	return mmapper_.Size();
}

void FileWMMapperExt::Shrink() {
	mmapper_.Shrink(guard_);
}

void FileWMMapperExt::CheckAndSync(const size_t offset, const size_t size) {
	if (sync_after_write_) {
		Sync(offset, size, MMapSync::Sync);
	}
}

} // namespace mmap
