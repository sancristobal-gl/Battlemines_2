include("cmake_utils/setup_visual_studio_filters.cmake")

function(setup_gtest project_name test_headers test_source_files extra_dependencies extra_libraries)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    set(test_target_name ${project_name}.tests)

    set(all_files ${test_source_files})
    list(APPEND all_files ${test_headers})
    add_executable(${test_target_name} ${all_files})
    target_link_libraries(${test_target_name} GTest::gtest GTest::gtest_main GTest::gmock)

    if (TARGET ${project_name})
        target_link_libraries(${test_target_name} ${project_name})
    endif()

    if (extra_dependencies)
        add_dependencies(${test_target_name} ${extra_dependencies})
        target_link_directories(${test_target_name} PUBLIC ${CMAKE_BINARY_DIR})
        target_link_libraries(${test_target_name} ${extra_dependencies})
    endif()

    if (extra_libraries)
        target_link_libraries(${test_target_name} ${extra_libraries})
    endif()
    
    enable_warnings(${test_target_name} false)

    setup_visual_studio_filters("src" test_headers "")
    setup_visual_studio_filters("src" test_source_files "")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${test_target_name})

    set_target_properties(gtest PROPERTIES FOLDER "_external/gtest")
    set_target_properties(gmock PROPERTIES FOLDER "_external/gtest")
    set_target_properties(gmock_main PROPERTIES FOLDER "_external/gtest")
    set_target_properties(gtest_main PROPERTIES FOLDER "_external/gtest")

    include(GoogleTest)
    gtest_discover_tests(${test_target_name})

    ######
    set(CMAKE_CXX_FLAGS_DEBUGANDTEST "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    set(CMAKE_C_FLAGS_DEBUGANDTEST "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUGANDTEST "${CMAKE_EXE_LINKER_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUGANDTEST "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUGANDTEST "${CMAKE_STATIC_LINKER_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASEANDTEST "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    set(CMAKE_C_FLAGS_RELEASEANDTEST "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASEANDTEST "${CMAKE_EXE_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASEANDTEST "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASEANDTEST "${CMAKE_STATIC_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)

    mark_as_advanced(
        CMAKE_CXX_FLAGS_DEBUGANDTEST
        CMAKE_C_FLAGS_DEBUGANDTEST
        CMAKE_EXE_LINKER_FLAGS_DEBUGANDTEST
        CMAKE_SHARED_LINKER_FLAGS_DEBUGANDTEST
        CMAKE_STATIC_LINKER_FLAGS_DEBUGANDTEST
        CMAKE_CXX_FLAGS_RELEASEANDTEST
        CMAKE_C_FLAGS_RELEASEANDTEST
        CMAKE_EXE_LINKER_FLAGS_RELEASEANDTEST
        CMAKE_SHARED_LINKER_FLAGS_RELEASEANDTEST
        CMAKE_STATIC_LINKER_FLAGS_RELEASEANDTEST
    )

    if(CMAKE_CONFIGURATION_TYPES)
      list(APPEND CMAKE_CONFIGURATION_TYPES DebugAndTest)
      list(APPEND CMAKE_CONFIGURATION_TYPES ReleaseAndTest)
      list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
      set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)
    endif()

    set(run_tests_target run_${PROJECT_NAME}_tests)
    add_custom_target(${run_tests_target} ALL COMMAND "$<$<CONFIG:DebugAndTest,ReleaseAndTest>:${PROJECT_NAME}.tests>" COMMENT "$<$<CONFIG:DebugAndTest,ReleaseAndTest>:Running ${PROJECT_NAME}.tests...>")
    add_dependencies(${run_tests_target} ${PROJECT_NAME}.tests)
    set_target_properties(${run_tests_target} PROPERTIES LINKER_LANGUAGE CXX FOLDER ${internals_project_folder})
endfunction()
