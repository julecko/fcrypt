cmake_minimum_required(VERSION 3.10)
project(fcrypt C)

set(FCRYPT_VERSION "1.0.0")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

message(STATUS "Compiler: ${CMAKE_C_COMPILER_ID}")

if(WIN32)
    find_package(unofficial-sodium CONFIG REQUIRED)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SODIUM REQUIRED libsodium)
endif()

add_executable(fcrypt
    src/main.c
    src/cli.c
    src/cli_args.c
    src/util.c
    src/crypt/crypt.c
    src/crypt/file_format.c
    src/cli_hide.c
    src/file_process.c
)

if(WIN32)
    target_link_libraries(fcrypt PRIVATE unofficial-sodium::sodium)
else()
    target_include_directories(fcrypt PRIVATE ${SODIUM_INCLUDE_DIRS})
    target_link_libraries(fcrypt PRIVATE ${SODIUM_LIBRARIES})
endif()

if(NOT WIN32)
    target_compile_options(fcrypt PRIVATE ${SODIUM_CFLAGS_OTHER})
endif()

target_include_directories(fcrypt PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_definitions(fcrypt PRIVATE
    FCRYPT_VERSION=\"${FCRYPT_VERSION}\"
)

# ==============================
# Compiler-specific configuration
# ==============================

if (MSVC)

    # Debug
    target_compile_options(fcrypt PRIVATE
        $<$<CONFIG:Debug>:/Zi /Od>
    )
    target_link_options(fcrypt PRIVATE
        $<$<CONFIG:Debug>:/DEBUG>
    )
    target_compile_definitions(fcrypt PRIVATE
        $<$<CONFIG:Debug>:DEBUG_MODE=1>
    )

    # Release
    target_compile_options(fcrypt PRIVATE
        $<$<CONFIG:Release>:/O2>
    )
    target_compile_definitions(fcrypt PRIVATE
        $<$<CONFIG:Release>:NDEBUG>
    )

else() # GCC / Clang

    # Debug
    target_compile_options(fcrypt PRIVATE
        $<$<CONFIG:Debug>:-g -O0 -fno-omit-frame-pointer>
    )
    target_compile_definitions(fcrypt PRIVATE
        $<$<CONFIG:Debug>:DEBUG_MODE=1>
    )

    # Optional ASAN (ONLY here)
    target_compile_options(fcrypt PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address>
    )
    target_link_options(fcrypt PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address>
    )

    # Release
    target_compile_options(fcrypt PRIVATE
        $<$<CONFIG:Release>:-O3>
    )
    target_compile_definitions(fcrypt PRIVATE
        $<$<CONFIG:Release>:NDEBUG>
    )

endif()
