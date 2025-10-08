#pragma once

#include <format>
#include <stdio.h>

class NBT_Print
{
#ifdef _MSC_VER
#if _MSC_VER >= 1935 //msvc 19.35+ 支持标准 format_string
#define FMT_STR format_string
#else
#define FMT_STR _Fmt_string //旧版本 MSVC
#endif
#else
#define FMT_STR format_string //GCC、Clang 等使用标准库版本
#endif

public:
	template<typename... Args>
	void operator()(const std::FMT_STR<Args...> fmt, Args&&... args) noexcept
	{
		try
		{
			auto tmp = std::format(std::move(fmt), std::forward<Args>(args)...);
			fwrite(tmp.data(), sizeof(tmp.data()[0]), tmp.size(), stderr);
		}
		catch (const std::exception &e)
		{
			printf("ErrInfo Exception: \"%s\"\n", e.what());
		}
		catch (...)
		{
			printf("ErrInfo Exception: \"Unknown Error\"\n");
		}
	}
};