#pragma once

#include <mac_memory_mapping/base_file_mmapper_ext.hpp>
#include <mac_memory_mapping/file_descriptor_guard.hpp>

#include <cstdlib>


namespace MMap
{
class FileRWMMapperExt {
public:
	explicit FileRWMMapperExt(
		const char* const file_name,
		const size_t offset,
		const int access_mode,
		const bool sync_after_write = false);

	void Sync(const size_t offset, const size_t len, const MMapSync flag);

	[[nodiscard]] size_t Offset() const noexcept;
	[[nodiscard]] size_t Size() const noexcept;

	template<typename Type>
	[[nodiscard]] Type Read(const size_t offset) const { return mmapper_.Read<Type>(offset); }

	template<typename Type>
	[[nodiscard]] Type Read(const size_t offset) { return mmapper_.Read<Type>(guard_, offset); }

	template<typename Type>
	void Write(const Type& value, const size_t offset) {
		mmapper_.Write<Type>(value, offset);
		CheckAndSync(offset, static_cast<size_t>(sizeof(Type)));
	}

	template<typename Type>
	void WriteAndExtend(const Type& value, const size_t offset) {
		mmapper_.Write<Type>(guard_, value, offset);
		CheckAndSync(offset, static_cast<size_t>(sizeof(Type)));
	}

	void Shrink();

private:
	void CheckAndSync(const size_t offset, const size_t size);

private:
	FileDescriptorGuard guard_;
	BaseFileMMapperExt mmapper_;
	bool sync_after_write_ = false;
};

} // namespace mmap
