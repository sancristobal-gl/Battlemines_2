function (enable_warnings target_name is_interface)
    if(MSVC)
        # /WX
        set(args /W4)
    else()
        # -Werror
        set(args -Wall -Wextra -Wpedantic)
    endif()

    if (is_interface)
        target_compile_options(${target_name} INTERFACE ${args})
    else ()
        target_compile_options(${target_name} PRIVATE ${args})
    endif()
endfunction()
