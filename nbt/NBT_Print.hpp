#pragma once

#include <format>
#include <stdio.h>

class NBT_Print
{
#if defined(_MSC_VER) && _MSC_VER < 1935 //�ɰ汾MSVC 1935-��֧��
#define FMT_STR _Fmt_string //ʹ��MSVC���ڲ�����
#else
#define FMT_STR format_string //MSVC 19.35+��GCC��Clang ��ʹ�ñ�׼��汾
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