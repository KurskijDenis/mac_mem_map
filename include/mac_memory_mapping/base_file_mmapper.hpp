#pragma once

#include <mac_memory_mapping/types.hpp>

#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdio.h>


namespace MMap
{

class BaseFileMMapper {
public:
	explicit BaseFileMMapper(const int fd, const size_t offset, const size_t size, const MMapFileType type);

	BaseFileMMapper(const BaseFileMMapper&) = delete;
	BaseFileMMapper& operator=(const BaseFileMMapper&) = delete;

	BaseFileMMapper(BaseFileMMapper&& other) noexcept;
	BaseFileMMapper& operator=(BaseFileMMapper&& other) noexcept;

	void swap(BaseFileMMapper& other) noexcept;

	[[nodiscard]] MMapFileType Type() const noexcept;
	[[nodiscard]] size_t Offset() const noexcept;
	[[nodiscard]] size_t Size() const noexcept;

	void Sync(const size_t offset, const size_t len, const MMapSync flag = MMapSync::Sync);

	template<typename Type>
	[[nodiscard]] Type Read(const size_t offset) const {
		static_assert(std::is_pod_v<Type>);

		if (ptr_ == nullptr) {
			throw std::runtime_error("can't read from unmapped file");
		}

		const auto ptr_offset = offset_ + offset;
		if (size_ < ptr_offset + sizeof(Type)) {
			throw std::runtime_error("read out of memory scope");
		}

		auto ptr = ptr_ + ptr_offset;

		Type value;
		std::memcpy(&value, ptr, sizeof(Type));

		return value;
	}

	template<typename Type>
	void Write(const Type& value, const size_t offset) {
		static_assert(std::is_pod_v<Type>);

		if (ptr_ == nullptr) {
			throw std::runtime_error("can't write to unmapped file");
		}

		const auto ptr_offset = offset_ + offset;
		if (size_ < ptr_offset + sizeof(Type)) {
			throw std::runtime_error("write out of memory scope");
		}

		auto ptr = ptr_ + ptr_offset;
		std::memcpy(ptr, &value, sizeof(Type));
	}

	~BaseFileMMapper();

private:
	MMapFileType type_ = MMapFileType::R;
	char* ptr_ = nullptr;
	size_t offset_ = 0;
	size_t size_ = 0;
	size_t ptr_offset_ = 0;
};

} // namespace mmap
