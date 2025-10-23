vcpkg remove cjf2-nbt-cpp --recurse
pause

vcpkg install cjf2-nbt-cpp --overlay-ports=.\ports
pause
vcpkg remove cjf2-nbt-cpp
pause

vcpkg install cjf2-nbt-cpp[xxhash] --overlay-ports=.\ports
pause
vcpkg remove cjf2-nbt-cpp
pause

vcpkg install cjf2-nbt-cpp[zlib] --overlay-ports=.\ports
pause
vcpkg remove cjf2-nbt-cpp
pause

vcpkg install cjf2-nbt-cpp[zlib,xxhash] --overlay-ports=.\ports
pause
vcpkg remove cjf2-nbt-cpp
pause