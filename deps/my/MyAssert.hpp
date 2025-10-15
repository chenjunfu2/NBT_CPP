#pragma once

#include <stdio.h>
#include <stdarg.h>

#if defined(_MSC_VER)
#define COMPILER_MSVC 1
#define COMPILER_GCC 0
#define COMPILER_CLANG 0
#define COMPILER_NAME "MSVC"
#elif defined(__clang__)
#define COMPILER_MSVC 0
#define COMPILER_GCC 0
#define COMPILER_CLANG 1
#define COMPILER_NAME "Clang"
#elif defined(__GNUC__)
#define COMPILER_MSVC 0
#define COMPILER_GCC 1
#define COMPILER_CLANG 0
#define COMPILER_NAME "GCC"
#else
#define COMPILER_MSVC 0
#define COMPILER_GCC 0
#define COMPILER_CLANG 0
#define COMPILER_NAME "Unknown"
#endif


#if COMPILER_MSVC
#define PRINTF_FORMAT_ARGS _Printf_format_string_
#define PRINTF_FORMAT_ATTR
#elif COMPILER_GCC || COMPILER_CLANG
#define PRINTF_FORMAT_ARGS
#define PRINTF_FORMAT_ATTR __attribute__((__format__ (__printf__, 5, 6)))
#else
#define PRINTF_FORMAT_ARGS
#define PRINTF_FORMAT_ATTR
#endif


PRINTF_FORMAT_ATTR
inline void __MyAssert_Function__(const char *pFileName, size_t szLine, const char *pFunctionName, bool bValid, PRINTF_FORMAT_ARGS const char *pInfo = NULL, ...)
{
	if (bValid)
	{
		return;//校验成功返回
	}

	printf("Assertion Failure!\n    in file: %s\n    in line: %zu\n    in func: %s\n    in info: ", pFileName, szLine, pFunctionName);
	
	if (pInfo != NULL)
	{
		va_list vl;
		va_start(vl, pInfo);
		vprintf(pInfo, vl);
		va_end(vl);
	}
	else
	{
		printf("[No Info]");
	}
	
	//帮忙换行
	putchar('\n');

	exit(-114514);
}

//代理宏，延迟展开
#define __MY_ASSERT_FILE__ __FILE__
#define __MY_ASSERT_LINE__ __LINE__
#define __MY_ASSERT_FUNC__ __FUNCTION__

//cpp20的__VA_OPT__(,)，仅在__VA_ARGS__不为空时添加','以防止编译错误
//msvc需启用"/Zc:preprocessor"以使得预处理器识别此宏关键字（哎呀msvc你怎么这么坏呀）
#define MyAssert(v, ...) __MyAssert_Function__(__MY_ASSERT_FILE__, __MY_ASSERT_LINE__, __MY_ASSERT_FUNC__, (v) __VA_OPT__(,) __VA_ARGS__)

#undef PRINTF_FORMAT_ATTR
#undef PRINTF_FORMAT_ARGS

#undef COMPILER_NAME
#undef COMPILER_CLANG
#undef COMPILER_GCC
#undef COMPILER_MSVC