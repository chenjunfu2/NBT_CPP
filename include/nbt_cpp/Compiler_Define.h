#pragma once

//��Ԥ�������п��ܵı�������
#define __COMPILER_MSVC__ 0
#define __COMPILER_GCC__ 0
#define __COMPILER_CLANG__ 0

//Ȼ��ʵ���ж����ĸ����������Ǿ��滻���Լ��ĺ�Ϊ1
#if defined(_MSC_VER)
#undef  __COMPILER_MSVC__
#define __COMPILER_MSVC__ 1
#define __COMPILER_NAME__ "MSVC"
#elif defined(__GNUC__)
#undef  __COMPILER_GCC__
#define __COMPILER_GCC__ 1
#define __COMPILER_NAME__ "GCC"
#elif defined(__clang__)
#undef  __COMPILER_CLANG__
#define __COMPILER_CLANG__ 1
#define __COMPILER_NAME__ "Clang"
#else
#define __COMPILER_NAME__ "Unknown"
#endif