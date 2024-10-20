cmake_minimum_required(VERSION 3.24)

project(memmap_test VERSION 1.0)

add_executable(memmap_test
  main.cpp

  include/mac_memory_mapping/base_file_mmapper.hpp
  include/mac_memory_mapping/base_file_mmapper_ext.hpp
  include/mac_memory_mapping/file_descriptor_guard.hpp
  include/mac_memory_mapping/mmap_private.hpp
  include/mac_memory_mapping/mmap_private_ext.hpp
  include/mac_memory_mapping/mmap_reader.hpp
  include/mac_memory_mapping/mmap_reader_ext.hpp
  include/mac_memory_mapping/mmap_writer.hpp
  include/mac_memory_mapping/mmap_writer_ext.hpp
  include/mac_memory_mapping/mmap_rwriter.hpp
  include/mac_memory_mapping/mmap_rwriter_ext.hpp
  include/mac_memory_mapping/types.hpp
  include/mac_memory_mapping/utils.hpp
  include/mac_memory_mapping/utils.hpp

  internal_include/example.hpp

  src/base_file_mmapper.cpp
  src/base_file_mmapper_ext.cpp
  src/file_descriptor_guard.cpp
  src/mmap_private.cpp
  src/mmap_private_ext.cpp
  src/mmap_reader.cpp
  src/mmap_reader_ext.cpp
  src/mmap_writer.cpp
  src/mmap_writer_ext.cpp
  src/mmap_rwriter.cpp
  src/mmap_rwriter_ext.cpp
  src/utils.cpp
  src/example.cpp
)

if (MSVC)
  # warning level 4
  target_compile_options(memmap_test PRIVATE /W4)
else()
  # additional warnings
  target_compile_options(memmap_test PRIVATE -Wall -Wextra -Werror -Wpedantic)
endif()

target_compile_features(memmap_test PRIVATE cxx_std_17)

target_include_directories(memmap_test PUBLIC include)
target_include_directories(memmap_test PUBLIC internal_include)

