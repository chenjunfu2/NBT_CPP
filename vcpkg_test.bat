vcpkg install nbt-cpp --overlay-ports=.\ports
pause
vcpkg remove nbt-cpp
pause

vcpkg install nbt-cpp[xxhash] --overlay-ports=.\ports
pause
vcpkg remove nbt-cpp
pause

vcpkg install nbt-cpp[gzip] --overlay-ports=.\ports
pause
vcpkg remove nbt-cpp
pause