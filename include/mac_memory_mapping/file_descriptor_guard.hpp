#pragma once

#include <mac_memory_mapping/types.hpp>
#include <mac_memory_mapping/mmap_reader.hpp>
#include <mac_memory_mapping/mmap_writer.hpp>
#include <mac_memory_mapping/mmap_rwriter.hpp>
#include <mac_memory_mapping/mmap_private.hpp>

#include <cstdlib>
#include <exception>


namespace MMap
{

class FileDescriptorGuard {
public:
	explicit FileDescriptorGuard(const int fd, size_t file_size, const OpenFileMode mode = OpenFileMode::R);
	explicit FileDescriptorGuard(
		const char* const file_name,
		const OpenFileMode mode = OpenFileMode::R,
		const int access_mode = static_cast<int>(AccessMode::None)
	);

	FileDescriptorGuard(const FileDescriptorGuard&) = delete;
	FileDescriptorGuard& operator=(const FileDescriptorGuard&) = delete;

	FileDescriptorGuard(FileDescriptorGuard&& other) noexcept;
	FileDescriptorGuard& operator=(FileDescriptorGuard&& other) noexcept;

	void Resize(const size_t file_size);

	void swap(FileDescriptorGuard& other) noexcept;

	size_t RecheckSize();
	[[nodiscard]] int FileDescriptor() noexcept;
	[[nodiscard]] size_t FileSize() noexcept;

	[[nodiscard]] FileRWMMapper GetRWMMapProjection(const size_t offset, const bool sync_after_write = false) const;
	[[nodiscard]] FileWMMapper GetWMMapProjection(const size_t offset, const bool sync_after_write = false) const;
	[[nodiscard]] FilePMMapper GetPMMapProjection(const size_t offset) const;
	[[nodiscard]] FileRMMapper GetRMMapProjection(const size_t offset) const;

	~FileDescriptorGuard();

private:
	int file_descriptor_ = -1;
	size_t file_size_ = 0;
	OpenFileMode mode_ = OpenFileMode::R;
};

} // namespace mmap
