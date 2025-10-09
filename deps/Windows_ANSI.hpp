#pragma once

#include <Windows.h>
#include <stdio.h>
#include <string>

template<typename U16T = char16_t, typename ANSIT = char>
std::basic_string<ANSIT> ConvertUtf16ToAnsi(const std::basic_string<U16T> &u16String)
{
	static_assert(sizeof(U16T) == sizeof(wchar_t), "U16T size must be same as wchar_t size");

	if (u16String.empty())
	{
		return std::basic_string<ANSIT>{};//���ؿ��ַ���
	}

	// ��ȡת��������Ļ�������С��������ֹ����
	const int lengthNeeded = WideCharToMultiByte(//ע��˺���u16�����ַ�������u8�����ֽ���
		CP_ACP,                // ʹ�õ�ǰANSI����ҳ
		WC_NO_BEST_FIT_CHARS,  // �滻�޷�ֱ��ӳ����ַ�
		(wchar_t*)u16String.data(),//static_assert��֤�ײ��С��ͬ
		u16String.size(),//���������С����ת�����������\0
		NULL,
		0,
		NULL,
		NULL
	);

	if (lengthNeeded == 0)
	{
		//ERROR_INSUFFICIENT_BUFFER//�� �ṩ�Ļ�������С�����󣬻��ߴ��������Ϊ NULL��
		//ERROR_INVALID_FLAGS//�� Ϊ��־�ṩ��ֵ��Ч��
		//ERROR_INVALID_PARAMETER//�� �κβ���ֵ����Ч��
		//ERROR_NO_UNICODE_TRANSLATION//�� ���ַ����з�����Ч�� Unicode��
		
		printf("\nfirst WideCharToMultiByte failed. Error code: %lu\n", GetLastError());
		return std::basic_string<ANSIT>{};
	}

	//����string��Ԥ�����С
	std::basic_string<ANSIT> ansiString;
	ansiString.resize(lengthNeeded);

	// ִ��ʵ��ת��
	int convertedSize = WideCharToMultiByte(
		CP_ACP,
		WC_NO_BEST_FIT_CHARS,
		(wchar_t *)u16String.data(),
		u16String.size(),
		(char *)ansiString.data(),
		lengthNeeded,
		NULL,
		NULL
	);

	if (convertedSize == 0)
	{
		printf("\nsecond WideCharToMultiByte failed. Error code: %lu\n", GetLastError());
		return std::basic_string<ANSIT>{};
	}

	return ansiString;
}

template<typename U16T = char16_t, typename U8T = char8_t>
std::basic_string<U8T> ConvertUtf16ToUtf8(const std::basic_string<U16T> &u16String)
{
	static_assert(sizeof(U16T) == sizeof(wchar_t), "U16T size must be same as wchar_t size");

	if (u16String.empty())
	{
		return std::basic_string<U8T>{};//���ؿ��ַ���
	}

	// ��ȡת��������Ļ�������С��������ֹ����
	const int lengthNeeded = WideCharToMultiByte(//ע��˺���u16�����ַ�������u8�����ֽ���
		CP_UTF8,                // ʹ��CP_UTF8����ҳ
		WC_NO_BEST_FIT_CHARS,  // �滻�޷�ֱ��ӳ����ַ�
		(wchar_t *)u16String.data(),//static_assert��֤�ײ��С��ͬ
		u16String.size(),//���������С����ת�����������\0
		NULL,
		0,
		NULL,
		NULL
	);

	if (lengthNeeded == 0)
	{
		//ERROR_INSUFFICIENT_BUFFER//�� �ṩ�Ļ�������С�����󣬻��ߴ��������Ϊ NULL��
		//ERROR_INVALID_FLAGS//�� Ϊ��־�ṩ��ֵ��Ч��
		//ERROR_INVALID_PARAMETER//�� �κβ���ֵ����Ч��
		//ERROR_NO_UNICODE_TRANSLATION//�� ���ַ����з�����Ч�� Unicode��

		printf("\nfirst WideCharToMultiByte failed. Error code: %lu\n", GetLastError());
		return std::basic_string<U8T>{};
	}

	//����string��Ԥ�����С
	std::basic_string<U8T> u8String;
	u8String.resize(lengthNeeded);

	// ִ��ʵ��ת��
	int convertedSize = WideCharToMultiByte(
		CP_UTF8,
		WC_NO_BEST_FIT_CHARS,
		(wchar_t *)u16String.data(),
		u16String.size(),
		(char *)u8String.data(),
		lengthNeeded,
		NULL,
		NULL
	);

	if (convertedSize == 0)
	{
		printf("\nsecond WideCharToMultiByte failed. Error code: %lu\n", GetLastError());
		return std::basic_string<U8T>{};
	}

	return u8String;
}

template<typename U8T = char8_t, typename ANSIT = char>
std::basic_string<ANSIT> ConvertUtf8ToAnsi(const std::basic_string<U8T> &u8String)
{
    static_assert(sizeof(U8T) == sizeof(ANSIT), "U8T size must be same as ANSIT size");

    if (u8String.empty())
    {
        return std::basic_string<ANSIT>{};
    }

    // UTF-8 -> UTF-16
    const int lengthNeeded = MultiByteToWideChar(
        CP_UTF8,
		0,
        (char *)u8String.data(),
		u8String.size(),
        NULL,
        0
    );

    if (lengthNeeded == 0)
    {
        printf("\nfirst MultiByteToWideChar failed. Error code: %lu\n", GetLastError());
        return std::basic_string<ANSIT>{};
    }

    std::basic_string<wchar_t> utf16Str;
    utf16Str.resize(lengthNeeded);

	int convertedSize = MultiByteToWideChar(
		CP_UTF8,
		0,
		(char *)u8String.data(),
		u8String.size(),
		(wchar_t *)utf16Str.data(),
		lengthNeeded
	);

	if (convertedSize == 0)
    {
        printf("\nsecond MultiByteToWideChar failed. Error code: %lu\n", GetLastError());
        return std::basic_string<ANSIT>{};
    }

    // UTF-16 -> ANSI
	return ConvertUtf16ToAnsi<wchar_t, ANSIT>(utf16Str);
}

template<typename ANSIT = char, typename U8T = char8_t>
std::basic_string<U8T> ConvertAnsiToUtf8(const std::basic_string<ANSIT> &ansiString)
{
	static_assert(sizeof(U8T) == sizeof(ANSIT), "U8T size must be same as ANSIT size");

	if (ansiString.empty())
	{
		return std::basic_string<U8T>{};
	}

	// ANSI -> UTF-16
	const int lengthNeeded = MultiByteToWideChar(
		CP_ACP,
		0,
		(char *)ansiString.data(),
		ansiString.size(),
		NULL,
		0
	);

	if (lengthNeeded == 0)
	{
		printf("\nfirst MultiByteToWideChar failed. Error code: %lu\n", GetLastError());
		return std::basic_string<U8T>{};
	}

	std::basic_string<wchar_t> utf16Str;
	utf16Str.resize(lengthNeeded);

	int convertedSize = MultiByteToWideChar(
		CP_ACP,
		0,
		(char *)ansiString.data(),
		ansiString.size(),
		(wchar_t *)utf16Str.data(),
		lengthNeeded
	);

	if (convertedSize == 0)
	{
		printf("\nsecond MultiByteToWideChar failed. Error code: %lu\n", GetLastError());
		return std::basic_string<U8T>{};
	}

	// UTF-16 -> UTF-8
	return ConvertUtf16ToUtf8<wchar_t, U8T>(utf16Str);
}