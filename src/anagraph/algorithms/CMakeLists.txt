add_library(graphalgorithms STATIC
    pagerank.cpp
    similarity.cpp
    shingle.cpp
)
target_link_libraries(graphalgorithms PUBLIC spdlog::spdlog graphcomponents)
target_include_directories(graphalgorithms PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(graphalgorithms PUBLIC cxx_std_20)
if (USE_ASAN)
    target_compile_options(graphalgorithms PUBLIC -Wall -fsanitize=address)
    target_link_options(graphalgorithms PUBLIC -fsanitize=address)
else()
    target_compile_options(graphalgorithms PUBLIC -Wall)
endif()