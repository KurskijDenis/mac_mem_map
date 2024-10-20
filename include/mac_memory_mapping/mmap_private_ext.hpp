#pragma once

#include <mac_memory_mapping/base_file_mmapper_ext.hpp>
#include <mac_memory_mapping/file_descriptor_guard.hpp>

#include <cstdlib>


namespace MMap
{

class FilePMMapperExt {
public:
	explicit FilePMMapperExt(const char* const file_name, const size_t offset);

	void ReloadMemoryArea(const size_t offset, const size_t len);

	[[nodiscard]] size_t Offset() const noexcept;
	[[nodiscard]] size_t Size() const noexcept;

	template<typename Type>
	[[nodiscard]] Type Read(const size_t offset) const { return mmapper_.Read<Type>(offset); }

	template<typename Type>
	[[nodiscard]] Type Read(const size_t offset) { return mmapper_.Read<Type>(guard_, offset); }

	template<typename Type>
	void Write(const Type& value, const size_t offset) { mmapper_.Write<Type>(value, offset); }

private:
	FileDescriptorGuard guard_;
	BaseFileMMapperExt mmapper_;
};

} // namespace mmap
