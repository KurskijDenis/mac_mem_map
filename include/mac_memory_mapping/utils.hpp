#pragma once

#include <cstddef>


namespace MMap
{

size_t PageSize();

size_t RemainingFileSize(const size_t offset, const size_t file_size);

} // namespace mmap
