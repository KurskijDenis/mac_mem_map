#include <mac_memory_mapping/base_file_mmapper.hpp>

#include <mac_memory_mapping/utils.hpp>

#include <sys/mman.h>

#include <utility>


namespace MMap
{

namespace
{

int GetSyncMap(const MMapSync flag) {
	switch(flag) {
	case MMapSync::Sync: return MS_SYNC;
	case MMapSync::Async: return MS_ASYNC;
	default: return MS_INVALIDATE;
	}
}

std::pair<int, int> GetMMapFlags(const MMapFileType type) {
	if (type == MMapFileType::R) {
		return {PROT_READ, MAP_SHARED};
	}

	if (type == MMapFileType::W) {
		return {PROT_WRITE, MAP_SHARED};
	}

	if (type == MMapFileType::P) {
		return {PROT_WRITE | PROT_READ, MAP_PRIVATE};
	}

	return {PROT_WRITE | PROT_READ, MAP_SHARED};
}

} // namespace

BaseFileMMapper::BaseFileMMapper(const int fd, const size_t offset, const size_t size, const MMapFileType type)
	: type_(type) {
	const auto page_size = PageSize();
	const auto page_offset = offset / page_size;

	ptr_offset_ = page_offset * page_size;
	offset_ = offset - ptr_offset_;

	if (size != 0) {
		size_ = size + offset_;

		const auto [prot_flag, flag] = GetMMapFlags(type);
		auto mmap_ptr = mmap(nullptr, size_, prot_flag, flag, fd, ptr_offset_);
		if (mmap_ptr == nullptr) {
			const auto error = errno;
			throw std::runtime_error("can't mmap file: error descriptor " + std::to_string(error));
		}

		ptr_ = reinterpret_cast<char*>(mmap_ptr);
	}
}

BaseFileMMapper::BaseFileMMapper(BaseFileMMapper&& other) noexcept
	: type_(other.type_)
	, ptr_(other.ptr_)
	, offset_(other.offset_)
	, size_(other.size_)
	, ptr_offset_(other.ptr_offset_) {
	other.type_ = MMapFileType::R;
	other.ptr_ = nullptr;
	other.offset_ = 0;
	other.size_ = 0;
	other.ptr_offset_= 0;
}

BaseFileMMapper& BaseFileMMapper::operator=(BaseFileMMapper&& other) noexcept {
	if (&other != this) {
		BaseFileMMapper tmp(std::move(other));
		swap(tmp);
	}

	return *this;
}

void BaseFileMMapper::swap(BaseFileMMapper& other) noexcept {
	std::swap(type_, other.type_);
	std::swap(ptr_, other.ptr_);
	std::swap(offset_, other.offset_);
	std::swap(size_, other.size_);
	std::swap(ptr_offset_, other.ptr_offset_);
}

MMapFileType BaseFileMMapper::Type() const noexcept {
	return type_;
}

size_t BaseFileMMapper::Offset() const noexcept {
	return ptr_offset_ + offset_;
}

size_t BaseFileMMapper::Size() const noexcept {
	return size_ - offset_;
}

void BaseFileMMapper::Sync(const size_t offset, const size_t len, const MMapSync flag) {
	if (ptr_ == nullptr) {
		return;
	}

	const auto page_size = PageSize();
	const auto page_count = (offset_ + offset) / page_size;
	const auto offset_to_sync = (offset_ + offset) - page_count * page_size;

	if (const auto res = msync(ptr_ + page_count * page_size, offset_to_sync + len, GetSyncMap(flag)); res == -1) {
		const auto error = errno;
		throw std::runtime_error("can't sync file: error descriptor " + std::to_string(error));
	}
}

BaseFileMMapper::~BaseFileMMapper() {
	if (ptr_ == nullptr) {
		return;
	}

	if (auto res = munmap(ptr_, size_); res == -1) {
		const auto error = errno;
		std::cerr << "can't unmap file: error descriptor " << error << "\n";
	}
}

} // namespace mmap
