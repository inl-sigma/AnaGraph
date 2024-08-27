# CMakeのバージョンを設定
cmake_minimum_required(VERSION 3.11)

# プロジェクト名と使用する言語を設定
project(graph_summary CXX)

enable_testing()

include(FetchContent)
# spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        27cb4c76708608465c413f6d0e6b8d99a4d84302 # release-1.14.1
)
FetchContent_MakeAvailable(spdlog)

# googletest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        e39786088138f2749d64e9e90e0f9902daa77c40 # release-1.15.0
)
# 親プロジェクトのコンパイラ・リンカ設定を上書きするのを防ぐ（Windowsのみ）
if(WIN32)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

# MACかつgccの場合にはasanを使えないため、そのための変数を設定
if(APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(USE_ASAN OFF)
else()
  set(USE_ASAN ON)
endif()

add_subdirectory(src)
add_subdirectory(test)