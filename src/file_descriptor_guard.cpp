#include <mac_memory_mapping/file_descriptor_guard.hpp>

#include <mac_memory_mapping/utils.hpp>

#include <fcntl.h>
#include <unistd.h>
#include <optional>


namespace MMap
{

namespace
{

int GetOpenFileFlag(const OpenFileMode mode) {
	if (mode == OpenFileMode::R) {
		return O_RDONLY;
	}

	return O_RDWR | O_CREAT;
}

std::optional<int> GetAccessOpenFileMode(const int mode) {
	if (mode == static_cast<int>(AccessMode::None)) {
		return std::nullopt;
	}

	int result = 0;
	if ((mode & static_cast<int>(AccessMode::UR)) != 0) {
		result |= S_IRUSR;
	}
	if ((mode & static_cast<int>(AccessMode::UW)) != 0) {
		result |= S_IWUSR;
	}
	if ((mode & static_cast<int>(AccessMode::GR)) != 0) {
		result |= S_IRGRP;
	}
	if ((mode & static_cast<int>(AccessMode::GW)) != 0) {
		result |= S_IWGRP;
	}
	if ((mode & static_cast<int>(AccessMode::OR)) != 0) {
		result |= S_IROTH;
	}
	if ((mode & static_cast<int>(AccessMode::OW)) != 0) {
		result |= S_IWOTH;
	}

	return result;
}

} // namespace

FileDescriptorGuard::FileDescriptorGuard(const int fd, size_t file_size, const OpenFileMode mode)
	: file_descriptor_(fd)
	, file_size_(file_size)
	, mode_(mode) {
}

FileDescriptorGuard::FileDescriptorGuard(const char* const file_name, const OpenFileMode mode, const int amode)
	: mode_(mode) {
	const int flag = GetOpenFileFlag(mode_);
	if (const auto m = GetAccessOpenFileMode(amode); m) {
		file_descriptor_ = open(file_name, flag, *m);
	} else {
		file_descriptor_ = open(file_name, flag);
	}

	if (file_descriptor_ == -1) {
		const auto error = errno;
		throw std::runtime_error("can't open file: error descriptor " + std::to_string(error));
	}

	RecheckSize();
}

FileDescriptorGuard::FileDescriptorGuard(FileDescriptorGuard&& other) noexcept
	: file_descriptor_(other.file_descriptor_)
	, file_size_(other.file_size_)
	, mode_(other.mode_) {
	other.file_descriptor_ = -1;
	other.file_size_ = 0;
}

FileDescriptorGuard& FileDescriptorGuard::operator=(FileDescriptorGuard&& other) noexcept {
	FileDescriptorGuard guard(std::move(other));
	swap(guard);

	return *this;
}

void FileDescriptorGuard::Resize(const size_t file_size) {
	if (file_descriptor_ == -1) {
		return;
	}

	if (mode_ == OpenFileMode::R) {
		throw std::runtime_error("can't resize read only file");
	}

	if (const auto res = ftruncate(file_descriptor_, file_size); res == -1) {
		const auto error = errno;
		throw std::runtime_error("can't resize file: error descriptor " + std::to_string(error));
	}

	file_size_ = file_size;
}

void FileDescriptorGuard::swap(FileDescriptorGuard& other) noexcept {
	std::swap(file_descriptor_, other.file_descriptor_);
	std::swap(file_size_, other.file_size_);
	std::swap(mode_, other.mode_);
}

size_t FileDescriptorGuard::RecheckSize() {
	if (file_descriptor_ == -1) {
		return 0;
	}

	const auto fsize = lseek(file_descriptor_, 0, SEEK_END);
	if (fsize == -1) {
		const auto error = errno;
		throw std::runtime_error("can't check file size: error descriptor " + std::to_string(error));
	}

	file_size_ = static_cast<size_t>(fsize);

	return file_size_;
}

int FileDescriptorGuard::FileDescriptor() noexcept {
	return file_descriptor_;
}

size_t FileDescriptorGuard::FileSize() noexcept {
	return file_size_;
}

FileRWMMapper FileDescriptorGuard::GetRWMMapProjection(const size_t offset, const bool sync_after_write) const {
	if (file_descriptor_ == -1) {
		throw std::runtime_error("can't mmap file with undefined descriptor");
	}

	if (mode_ != OpenFileMode::RW) {
		throw std::runtime_error("can't write and read from read only or write only file");
	}

	return FileRWMMapper{file_descriptor_, offset, RemainingFileSize(offset, file_size_), sync_after_write};
}

FileWMMapper FileDescriptorGuard::GetWMMapProjection(const size_t offset, const bool sync_after_write) const {
	if (file_descriptor_ == -1) {
		throw std::runtime_error("can't mmap file with undefined descriptor");
	}

	if (mode_ == OpenFileMode::R) {
		throw std::runtime_error("can't write to read only file");
	}

	return FileWMMapper{file_descriptor_, offset, RemainingFileSize(offset, file_size_), sync_after_write};
}

FilePMMapper FileDescriptorGuard::GetPMMapProjection(const size_t offset) const {
	if (file_descriptor_ == -1) {
		throw std::runtime_error("can't mmap file with undefined descriptor");
	}

	return FilePMMapper{file_descriptor_, offset, RemainingFileSize(offset, file_size_)};
}

FileRMMapper FileDescriptorGuard::GetRMMapProjection(const size_t offset) const {
	return FileRMMapper{file_descriptor_, offset, RemainingFileSize(offset, file_size_)};
}

FileDescriptorGuard::~FileDescriptorGuard() {
	if (file_descriptor_ == -1) {
		return;
	}

	if (const auto res = close(file_descriptor_); res == -1) {
		const auto error = errno;
		std::cerr << "can't close file: error descriptor " << error << "\n";
	}
}

} // namespace mmap
