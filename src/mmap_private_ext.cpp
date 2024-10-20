#include <mac_memory_mapping/mmap_private_ext.hpp>

#include <mac_memory_mapping/utils.hpp>

#include <sys/mman.h>


namespace MMap
{

FilePMMapperExt::FilePMMapperExt(const char* const file_name, const size_t offset)
	: guard_(file_name, OpenFileMode::R, static_cast<int>(AccessMode::None))
	, mmapper_{guard_.FileDescriptor(), offset, RemainingFileSize(offset, guard_.FileSize()), MMapFileType::R} {
}

void FilePMMapperExt::ReloadMemoryArea(const size_t offset, const size_t len) {
	mmapper_.Sync(offset, len, MMapSync::Invalidate);
}

size_t FilePMMapperExt::Offset() const noexcept {
	return mmapper_.Offset();
}

size_t FilePMMapperExt::Size() const noexcept {
	return mmapper_.Size();
}

} // namespace mmap
