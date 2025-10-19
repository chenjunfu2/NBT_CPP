vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO chenjunfu2/NBT_CPP
    REF "v${VERSION}"
    HEAD_REF master
    SHA512 29092779b0e5f2d159046686e85ad8c18309eacae5180b61d359c6b8024622f2c61f7e754895a2e7be83c3e2bcd8d59ebd7eb7191c7b97b80eaae29cd1111ac9
)

file(INSTALL "${SOURCE_PATH}/include/nbt_cpp/"
     DESTINATION "${CURRENT_PACKAGES_DIR}/include/nbt_cpp"
     FILES_MATCHING
     PATTERN "*.hpp"
     PATTERN "*.h"
)


if("tests" IN_LIST FEATURES)
    message(STATUS "Building tests for nbt-cpp")
    
    set(TEST_BUILD_DIR "${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}-test")
    file(MAKE_DIRECTORY "${TEST_BUILD_DIR}")
    
    vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}/test_package"
        OPTIONS
            -DCMAKE_PREFIX_PATH=${CURRENT_INSTALLED_DIR}
        BINARY_DIR "${TEST_BUILD_DIR}"
    )
    vcpkg_cmake_build(
        BINARY_DIR "${TEST_BUILD_DIR}"
        TARGET ALL
    )
    vcpkg_cmake_build(
        BINARY_DIR "${TEST_BUILD_DIR}"
        TARGET test
    )
    
    message(STATUS "Tests completed successfully")
endif()


if(EXISTS "${SOURCE_PATH}/LICENSE")
    file(INSTALL "${SOURCE_PATH}/LICENSE"
         DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
         RENAME copyright)
elseif(EXISTS "${SOURCE_PATH}/LICENSE.md")
    file(INSTALL "${SOURCE_PATH}/LICENSE.md"
         DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
         RENAME copyright)
endif()