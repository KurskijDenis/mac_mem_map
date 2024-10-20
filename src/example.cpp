#include <mac_memory_mapping/mmap_writer_ext.hpp>
#include <mac_memory_mapping/mmap_reader_ext.hpp>
#include <mac_memory_mapping/mmap_private.hpp>
#include <mac_memory_mapping/file_descriptor_guard.hpp>

#include <unistd.h>
#include <thread>


void WriteAndReadDifferentProcesses(const char* const file_name) {
	const auto pid = fork();

	if (pid == -1) {
		throw std::runtime_error("can't create child process");
	} else if (pid != 0) {
		std::this_thread::sleep_for(std::chrono::seconds{2});
		std::cout << "Start read from file" << std::endl;
		MMap::FileRMMapperExt file(file_name, 2000);

		for (size_t i = 0; i < 10 ;++i) {
			std::cout << file.Read<int>(i * sizeof(int)) << std::endl;
		}
		std::cout << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds{5});
		std::cout << file.Read<int>(100 * sizeof(int)) << std::endl;

		return;
	} else {
		MMap::FileWMMapperExt file(
			file_name,
			0,
			static_cast<int>(MMap::AccessMode::UR) | static_cast<int>(MMap::AccessMode::UW),
			false);

		int value = 0;
		for (size_t i = 0; i < 1000 ;++i) {
			file.WriteAndExtend(value, i * sizeof(value));
			value += 10;
		}

		std::cout << "Finish write to file" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds{5});
		value = 1234;
		file.Write<int>(value, 600 * sizeof(int));
		file.Shrink();

		return;
	}
}

void WriteToPrivateMMap(const char* const file_name) {
	{
		MMap::FileWMMapperExt file(
			file_name,
			0,
			static_cast<int>(MMap::AccessMode::UR) | static_cast<int>(MMap::AccessMode::UW),
			false);

		int value = 0;
		for (size_t i = 0; i < 1000 ;++i) {
			file.WriteAndExtend(value, i * sizeof(value));
			value += 10;
		}
	}
	{
		MMap::FileDescriptorGuard guard(file_name, MMap::OpenFileMode::RW);
		MMap::FilePMMapper file(guard.FileDescriptor(), 2000, 400);

		int value = 1;
		for (size_t i = 0; i < 100 ; ++i) {
			file.Write(value, i * sizeof(value));
			value += 10;
		}
	}
	{
		MMap::FileDescriptorGuard guard(file_name, MMap::OpenFileMode::R);
		MMap::FileRMMapper file(guard.FileDescriptor(), 2000, 400);

		for (size_t i = 0; i < 10 ;++i) {
			std::cout << file.Read<int>(i * sizeof(int)) << std::endl;
		}
	}
}
