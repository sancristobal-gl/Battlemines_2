macro (setup_clang_tidy for_target)
    set_target_properties(${for_target} PROPERTIES
        VS_GLOBAL_RunCodeAnalysis false
        VS_GLOBAL_EnableMicrosoftCodeAnalysis false
        VS_GLOBAL_EnableClangTidyCodeAnalysis true
    )
endmacro()
