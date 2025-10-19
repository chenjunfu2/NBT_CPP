vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO chenjunfu2/NBT_CPP
    REF "v${VERSION}"
    SHA512 0
    HEAD_REF main
)

file(INSTALL "${SOURCE_PATH}/include/"
     DESTINATION "${CURRENT_PACKAGES_DIR}/include"
     FILES_MATCHING
     PATTERN "*.hpp"
     PATTERN "*.h"
)

if(EXISTS "${SOURCE_PATH}/LICENSE")
    file(INSTALL "${SOURCE_PATH}/LICENSE"
         DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
         RENAME copyright)
elseif(EXISTS "${SOURCE_PATH}/LICENSE.md")
    file(INSTALL "${SOURCE_PATH}/LICENSE.md"
         DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
         RENAME copyright)
endif()