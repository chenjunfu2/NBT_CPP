vcpkg install nbt-cpp --overlay-ports=.\ports
pause
vcpkg remove nbt-cpp
pause

vcpkg install nbt-cpp[xxhash] --overlay-ports=.\ports
pause
vcpkg remove nbt-cpp
pause

vcpkg install nbt-cpp[zlib] --overlay-ports=.\ports
pause
vcpkg remove nbt-cpp
pause

vcpkg install nbt-cpp[zlib,xxhash] --overlay-ports=.\ports
pause
vcpkg remove nbt-cpp
pause