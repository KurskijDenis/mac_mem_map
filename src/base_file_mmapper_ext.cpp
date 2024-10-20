#include <mac_memory_mapping/base_file_mmapper_ext.hpp>

#include <mac_memory_mapping/file_descriptor_guard.hpp>
#include <mac_memory_mapping/utils.hpp>

#include <sys/mman.h>

#include <utility>


namespace MMap
{

BaseFileMMapperExt::BaseFileMMapperExt(
	const int fd,
	const size_t offset,
	const size_t size,
	const MMapFileType type)
	: mmap_(fd, offset, size, type)
	, max_written_offset_(offset) {
}

size_t BaseFileMMapperExt::Offset() const noexcept {
	return mmap_.Offset();
}

size_t BaseFileMMapperExt::Size() const noexcept {
	return mmap_.Size();
}

void BaseFileMMapperExt::Sync(const size_t offset, const size_t len, const MMapSync flag) {
	return mmap_.Sync(offset, len, flag);
}

void BaseFileMMapperExt::Shrink(FileDescriptorGuard& guard) {
	mmap_ = BaseFileMMapper{
		guard.FileDescriptor(),
		mmap_.Offset(),
		max_written_offset_- mmap_.Offset(),
		mmap_.Type()};
	guard.Resize(max_written_offset_);
}

void BaseFileMMapperExt::ReloadIfInaccessible(FileDescriptorGuard& guard, const size_t max_offset) {
	const auto expected_size = max_offset + mmap_.Offset();
	if (guard.FileSize() >= expected_size) {
		return;
	}

	const auto new_file_size = guard.RecheckSize();
	if (expected_size <= new_file_size) {
		mmap_ = BaseFileMMapper{
			guard.FileDescriptor(),
			mmap_.Offset(),
			new_file_size - mmap_.Offset(),
			mmap_.Type()};
	} else {
		throw std::runtime_error("offset out of file boundary");
	}
}

void BaseFileMMapperExt::ResizeIfInaccessible(FileDescriptorGuard& guard, const size_t max_offset) {
	const auto expected_size = max_offset + Offset();
	if (guard.FileSize() >= expected_size) {
		return;
	}

	guard.Resize(expected_size + PageSize());

	mmap_ = BaseFileMMapper{
		guard.FileDescriptor(),
		mmap_.Offset(),
		guard.FileSize() - mmap_.Offset(),
		mmap_.Type()};
}

} // namespace mmap
