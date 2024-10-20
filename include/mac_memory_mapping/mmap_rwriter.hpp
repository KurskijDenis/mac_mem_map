#pragma once

#include <mac_memory_mapping/base_file_mmapper.hpp>

#include <cstdlib>


namespace MMap
{
class FileRWMMapper {
public:
	explicit FileRWMMapper(
		const int fd,
		const size_t offset,
		const size_t size,
		const bool sync_after_write = false);

	void Sync(const size_t offset, const size_t len, const MMapSync flag);

	[[nodiscard]] size_t Offset() const noexcept;
	[[nodiscard]] size_t Size() const noexcept;

	template<typename Type>
	[[nodiscard]] Type Read(const size_t offset) const { return mmapper_.Read<Type>(offset); }

	template<typename Type>
	void Write(const Type& value, const size_t offset) {
		mmapper_.Write<Type>(value, offset);

		if (sync_after_write_) {
			Sync(offset, sizeof(Type), MMapSync::Sync);
		}
	}

private:
	BaseFileMMapper mmapper_;
	bool sync_after_write_ = false;
};

} // namespace mmap
