#include <mac_memory_mapping/mmap_reader_ext.hpp>

#include <mac_memory_mapping/utils.hpp>

#include <sys/mman.h>


namespace MMap
{

FileRMMapperExt::FileRMMapperExt(const char* const file_name, const size_t offset)
	: guard_(file_name, OpenFileMode::R, int(AccessMode::None))
	, mmapper_{guard_.FileDescriptor(), offset, RemainingFileSize(offset, guard_.FileSize()), MMapFileType::R} {
}

void FileRMMapperExt::ReloadMemoryArea(const size_t offset, const size_t len) {
	mmapper_.Sync(offset, len, MMapSync::Invalidate);
}

size_t FileRMMapperExt::Offset() const noexcept {
	return mmapper_.Offset();
}

size_t FileRMMapperExt::Size() const noexcept {
	return mmapper_.Size();
}

} // namespace mmap
