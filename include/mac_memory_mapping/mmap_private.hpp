#pragma once

#include <mac_memory_mapping/base_file_mmapper.hpp>

#include <cstdlib>


namespace MMap
{

class FilePMMapper {
public:
	explicit FilePMMapper(const int fd, const size_t offset, const size_t size);

	void ReloadMemoryArea(const size_t offset, const size_t len);

	[[nodiscard]] size_t Offset() const noexcept;
	[[nodiscard]] size_t Size() const noexcept;

	template<typename Type>
	[[nodiscard]] Type Read(const size_t offset) const { return mmapper_.Read<Type>(offset); }

	template<typename Type>
	void Write(const Type& value, const size_t offset) { mmapper_.Write<Type>(value, offset); }

private:
	BaseFileMMapper mmapper_;
};

} // namespace mmap
