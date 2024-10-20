#include <mac_memory_mapping/mmap_rwriter_ext.hpp>

#include <mac_memory_mapping/utils.hpp>

#include <sys/mman.h>


namespace MMap
{

FileRWMMapperExt::FileRWMMapperExt(
	const char* const file_name,
	const size_t offset,
	const int access_mode,
	const bool sync_after_write)
	: guard_(file_name, OpenFileMode::RW, access_mode)
	, mmapper_{guard_.FileDescriptor(), offset, RemainingFileSize(offset, guard_.FileSize()), MMapFileType::RW}
	, sync_after_write_(sync_after_write) {
}

void FileRWMMapperExt::Sync(const size_t offset, const size_t len, const MMapSync flag) {
	mmapper_.Sync(offset, len, flag);
}

size_t FileRWMMapperExt::Offset() const noexcept {
	return mmapper_.Offset();
}

size_t FileRWMMapperExt::Size() const noexcept {
	return mmapper_.Size();
}

void FileRWMMapperExt::Shrink() {
	mmapper_.Shrink(guard_);
}

void FileRWMMapperExt::CheckAndSync(const size_t offset, const size_t size) {
	if (sync_after_write_) {
		Sync(offset, size, MMapSync::Sync);
	}
}

} // namespace mmap
