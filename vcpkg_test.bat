vcpkg install chenjunfu2-nbt-cpp --overlay-ports=.\ports
pause
vcpkg remove chenjunfu2-nbt-cpp
pause

vcpkg install chenjunfu2-nbt-cpp[xxhash] --overlay-ports=.\ports
pause
vcpkg remove chenjunfu2-nbt-cpp
pause

vcpkg install chenjunfu2-nbt-cpp[zlib] --overlay-ports=.\ports
pause
vcpkg remove chenjunfu2-nbt-cpp
pause

vcpkg install chenjunfu2-nbt-cpp[zlib,xxhash] --overlay-ports=.\ports
pause
vcpkg remove chenjunfu2-nbt-cpp
pause