include("cmake_utils/setup_visual_studio_filters.cmake")

function(setup_benchmark project_name benchmark_headers benchmark_source_files extra_dependencies extra_libraries)
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

    include(FetchContent)
    FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    FetchContent_MakeAvailable(benchmark)

    set(benchmark_target_name ${project_name}.benchmark)

    set(all_files ${benchmark_source_files})
    list(APPEND all_files ${benchmark_headers})
    add_executable(${benchmark_target_name} ${all_files})
    target_link_libraries(${benchmark_target_name} benchmark::benchmark benchmark::benchmark_main)

    if (TARGET ${project_name})
        target_link_libraries(${benchmark_target_name} ${project_name})
    endif()

    if (extra_dependencies)
        add_dependencies(${benchmark_target_name} ${extra_dependencies})
        target_link_directories(${benchmark_target_name} PUBLIC ${CMAKE_BINARY_DIR})
        target_link_libraries(${benchmark_target_name} ${extra_dependencies})
    endif()

    if (extra_libraries)
        target_link_libraries(${benchmark_target_name} ${extra_libraries})
    endif()
    
    enable_warnings(${benchmark_target_name} false)

    setup_visual_studio_filters("src" benchmark_headers "")
    setup_visual_studio_filters("src" benchmark_source_files "")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${benchmark_target_name})

    set_target_properties(benchmark PROPERTIES FOLDER "_external/benchmark")
    set_target_properties(benchmark_main PROPERTIES FOLDER "_external/benchmark")

    ######
    set(CMAKE_CXX_FLAGS_DEBUGBENCHMARK "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    set(CMAKE_C_FLAGS_DEBUGBENCHMARK "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUGBENCHMARK "${CMAKE_EXE_LINKER_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUGBENCHMARK "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUGBENCHMARK "${CMAKE_STATIC_LINKER_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASEBENCHMARK "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    set(CMAKE_C_FLAGS_RELEASEBENCHMARK "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASEBENCHMARK "${CMAKE_EXE_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASEBENCHMARK "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASEBENCHMARK "${CMAKE_STATIC_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)

    mark_as_advanced(
        CMAKE_CXX_FLAGS_DEBUGBENCHMARK
        CMAKE_C_FLAGS_DEBUGBENCHMARK
        CMAKE_EXE_LINKER_FLAGS_DEBUGBENCHMARK
        CMAKE_SHARED_LINKER_FLAGS_DEBUGBENCHMARK
        CMAKE_STATIC_LINKER_FLAGS_DEBUGBENCHMARK
        CMAKE_CXX_FLAGS_RELEASEBENCHMARK
        CMAKE_C_FLAGS_RELEASEBENCHMARK
        CMAKE_EXE_LINKER_FLAGS_RELEASEBENCHMARK
        CMAKE_SHARED_LINKER_FLAGS_RELEASEBENCHMARK
        CMAKE_STATIC_LINKER_FLAGS_RELEASEBENCHMARK
    )

    if(CMAKE_CONFIGURATION_TYPES)
      list(APPEND CMAKE_CONFIGURATION_TYPES DebugBenchmark)
      list(APPEND CMAKE_CONFIGURATION_TYPES ReleaseBenchmark)
      list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
      set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)
    endif()

    set(run_benchmark_target run_${PROJECT_NAME}_benchmark)
    add_custom_target(${run_benchmark_target} ALL COMMAND "$<$<CONFIG:DebugBenchmark,ReleaseBenchmark>:${PROJECT_NAME}.benchmark>" COMMENT "$<$<CONFIG:DebugBenchmark,ReleaseBenchmark>:Running ${PROJECT_NAME}.benchmark...>")
    add_dependencies(${run_benchmark_target} ${PROJECT_NAME}.benchmark)
    set_target_properties(${run_benchmark_target} PROPERTIES LINKER_LANGUAGE CXX FOLDER ${internals_project_folder})
endfunction()
