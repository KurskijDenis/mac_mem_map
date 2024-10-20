#pragma once

#include <mac_memory_mapping/base_file_mmapper.hpp>

#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdio.h>


namespace MMap
{

class FileDescriptorGuard;

class BaseFileMMapperExt {
public:
	explicit BaseFileMMapperExt(const int fd, const size_t offset, const size_t size, const MMapFileType type);

	BaseFileMMapperExt(const BaseFileMMapperExt&) = delete;
	BaseFileMMapperExt& operator=(const BaseFileMMapperExt&) = delete;

	[[nodiscard]] size_t Offset() const noexcept;
	[[nodiscard]] size_t Size() const noexcept;

	void Sync(const size_t offset, const size_t len, const MMapSync flag = MMapSync::Sync);

	template<typename Type>
	[[nodiscard]] Type Read(const size_t offset) const {
		return mmap_.Read<Type>(offset);
	}

	template<typename Type>
	void Write(const Type& value, const size_t offset) {
		max_written_offset_ = std::max(mmap_.Offset() + offset + sizeof(Type), max_written_offset_);
		mmap_.Write<Type>(value, offset);
	}

	template<typename Type>
	[[nodiscard]] Type Read(FileDescriptorGuard& guard, const size_t offset) {
		ReloadIfInaccessible(guard, offset + sizeof(Type));

		return mmap_.Read<Type>(offset);
	}

	template<typename Type>
	void Write(FileDescriptorGuard& guard, const Type& value, const size_t offset) {
		ResizeIfInaccessible(guard, offset + sizeof(Type));

		Write(value, offset);
	}

	void Shrink(FileDescriptorGuard& guard);

private:
	void ReloadIfInaccessible(FileDescriptorGuard& guard, const size_t max_offset);
	void ResizeIfInaccessible(FileDescriptorGuard& guard, const size_t max_offset);

private:
	BaseFileMMapper mmap_;
	size_t max_written_offset_ = 0;
};

} // namespace mmap
