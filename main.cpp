#include <example.hpp>

#include <mac_memory_mapping/file_descriptor_guard.hpp>
#include <mac_memory_mapping/utils.hpp>
#include <mac_memory_mapping/mmap_writer_ext.hpp>

#include <iostream>


int main() {
	try {
		WriteToPrivateMMap("test3.dat");
		WriteAndReadDifferentProcesses("test4.dat");

		return 0;
	}
	catch(std::exception& exc) {
		std::cerr << "exception: " << exc.what();
	}
}
