cmake_minimum_required(VERSION 3.13)
project(zlib LANGUAGES CXX)


add_executable(zlib
    adler32.c
    compress.c
    crc32.c
    deflate.c
    gzclose.c
    gzlib.c
    gzread.c
    gzwrite.c
    infback.c
    inffast.c
    inflate.c
    inftrees.c
    trees.c
    uncompr.c
    zutil.c
)

target_compile_features(zlib
    PUBLIC
        cxx_std_20
        c_std_17
)

target_compile_definitions(zlib
    PUBLIC
        _CONSOLE
        UNICODE
        _UNICODE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        $<$<STREQUAL:$<CMAKE_CXX_COMPILER_ARCHITECTURE_ID>,X86>:WIN32>
)

target_compile_options(zlib
    PUBLIC
        /W3
)

