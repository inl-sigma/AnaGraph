function(configure_library TARGET)
    target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
    target_compile_features(${TARGET} PUBLIC cxx_std_20)
    if (USE_ASAN)
        target_compile_options(${TARGET} PUBLIC -Wall -fsanitize=address)
        target_link_options(${TARGET} PUBLIC -fsanitize=address)
    else()
        target_compile_options(${TARGET} PUBLIC -Wall)
    endif()
endfunction()

# Configures the "graphutils" library
add_library(graphutils STATIC 
    graph_parser.cpp
    graph_writer.cpp
)
target_link_libraries(graphutils PUBLIC spdlog::spdlog)
configure_library(graphutils)

# Configures the "unweightedgraph" library
add_library(unweightedgraph STATIC 
    unweighted_digraph.cpp
    unweighted_graph.cpp
    unweighted_node.cpp
)
target_link_libraries(unweightedgraph PUBLIC spdlog::spdlog graphutils)
configure_library(unweightedgraph)

# Configures the "weightedgraph" library
add_library(weightedgraph STATIC 
    weighted_digraph.cpp
    weighted_graph.cpp
    weighted_node.cpp
)
target_link_libraries(weightedgraph PUBLIC spdlog::spdlog graphutils)
configure_library(weightedgraph)

# Configures the "unweightedheterograph" library
add_library(unweightedheterograph STATIC 
    unweighted_hetero_node.cpp
    unweighted_hetero_digraph.cpp
    unweighted_hetero_graph.cpp
)
target_link_libraries(unweightedheterograph PUBLIC 
    spdlog::spdlog
    unweightedgraph
)
configure_library(unweightedheterograph)

# Configures the "weightedheterograph" library
add_library(weightedheterograph STATIC 
    weighted_hetero_node.cpp
    weighted_hetero_digraph.cpp
    weighted_hetero_graph.cpp
)
target_link_libraries(weightedheterograph PUBLIC
    spdlog::spdlog
    weightedgraph
)
configure_library(weightedheterograph)

# Configures the "supergraph" library
add_library(supergraph STATIC 
    weighted_supernode.cpp
    weighted_directed_supergraph.cpp
    weighted_supergraph.cpp
)
target_link_libraries(supergraph PUBLIC
    spdlog::spdlog
    weightedgraph
)
configure_library(supergraph)

# Configures the "graphcomponents" library
add_library(graphcomponents STATIC
    $<TARGET_OBJECTS:graphutils>
    $<TARGET_OBJECTS:unweightedgraph>
    $<TARGET_OBJECTS:weightedgraph>
    $<TARGET_OBJECTS:unweightedheterograph>
    $<TARGET_OBJECTS:weightedheterograph>
    $<TARGET_OBJECTS:supergraph>
)
target_link_libraries(graphcomponents PUBLIC
    spdlog::spdlog
    graphutils
    unweightedgraph
    weightedgraph
    unweightedheterograph
    weightedheterograph
    supergraph
)