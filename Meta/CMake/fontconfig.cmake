include_guard()

if (NOT ANDROID)
    find_package(Fontconfig REQUIRED)
    set(HAS_FONTCONFIG ON CACHE BOOL "" FORCE)
    add_compile_definitions(USE_FONTCONFIG=1)
endif()
