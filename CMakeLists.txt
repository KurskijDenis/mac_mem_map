cmake_minimum_required(VERSION 3.24)

project(memmap_test VERSION 1.0)

add_executable(postgres_client
  main.cpp
)

if (MSVC)
  # warning level 4
  target_compile_options(postgres_client PRIVATE /W4)
else()
  # additional warnings
  target_compile_options(postgres_client PRIVATE -Wall -Wextra -Werror -Wpedantic)
endif()

target_compile_features(postgres_client PRIVATE cxx_std_17)

