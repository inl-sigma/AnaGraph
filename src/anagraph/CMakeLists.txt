add_subdirectory(components)
add_subdirectory(algorithms)

add_dependencies(graphalgorithms graphcomponents) # todo: interface依存にする


add_library(anagraph STATIC
    $<TARGET_OBJECTS:graphcomponents>
    $<TARGET_OBJECTS:graphalgorithms>
)
target_link_libraries(anagraph PUBLIC spdlog::spdlog)
target_include_directories(anagraph PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(anagraph PUBLIC cxx_std_20)
if (USE_ASAN)
    target_compile_options(anagraph PUBLIC -Wall -fsanitize=address)
    target_link_options(anagraph PUBLIC -fsanitize=address)
else()
    target_compile_options(anagraph PUBLIC -Wall)
endif()