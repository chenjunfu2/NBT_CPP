#pragma once

#include <array>
#include <bit>
#include <stdint.h>

template<size_t N>
struct StaticHexArrayStaticStr
{
private:
	const char(&refStr)[N];

public:
	constexpr StaticHexArrayStaticStr(const char(&_refStr)[N]) noexcept :refStr(_refStr)
	{}
	constexpr ~StaticHexArrayStaticStr(void) noexcept = default;

	constexpr const char &operator[](size_t i) const
	{
		return refStr[i];
	}

	constexpr const char *Data(void) const
	{
		return +refStr;
	}

	constexpr size_t Size(void) const
	{
		return N;
	}
};

class StaticHexArray
{
	StaticHexArray(void) = delete;
	~StaticHexArray(void) = delete;

private:
	class Counter
	{
	private:
		size_t szCount = 0;

	public:
		constexpr Counter(void) noexcept = default;
		constexpr ~Counter(void) noexcept = default;

		constexpr void Push(uint8_t u8Byte) noexcept
		{
			++szCount;
		}

		constexpr auto Raw(void) const noexcept
		{
			return szCount;
		}
	};

	template<typename T, size_t N>
	class Array
	{
		static_assert(sizeof(T) == 1, "The sizeof(T) must be 1");

	private:
		std::array<T, N> arrData{ 0 };
		size_t szIndex = 0;

	public:
		constexpr Array(void) noexcept = default;
		constexpr ~Array(void) noexcept = default;

		constexpr void Push(uint8_t u8Byte) noexcept
		{
			arrData[szIndex] = std::bit_cast<T>(u8Byte);
			++szIndex;
		}

		constexpr auto Raw(void) const noexcept
		{
			return arrData;
		}
	};

private:
	consteval static uint8_t CharsToHex(char h, char l) noexcept
	{
		auto CharToHex = [](char c) -> uint8_t
		{
			switch (c)
			{
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				return (uint8_t)(c - '0') + (uint8_t)0x0;

			case 'A': case 'B': case 'C':
			case 'D': case 'E': case 'F':
				return (uint8_t)(c - 'A') + (uint8_t)0xA;

			case 'a': case 'b': case 'c':
			case 'd': case 'e': case 'f':
				return (uint8_t)(c - 'a') + (uint8_t)0xA;

			default:
				return (uint8_t)0x0;
			}
		};

		return (CharToHex(h) & 0x0F) << 4 | (CharToHex(l) & 0x0F) << 0;
	}

	consteval static bool IsSpace(char c)
	{
		switch (c)
		{
		case ' ':   // space
		case '\t':  // horizontal tab
		case '\n':  // newline
		case '\v':  // vertical tab
		case '\f':  // form feed
		case '\r':  // carriage return
			return true;
		default:
			return false;
		}
	}

	template<StaticHexArrayStaticStr sStr, typename T>
	consteval static auto Parse(void) noexcept
	{
		T tRet{};

		while (size_t i = 0; true)
		{
			auto GetNoSpaceChar = [&](char &c) -> bool
			{
				//跳过空白
				while (i < sStr.Size() && IsSpace(sStr[i]))
				{
					++i;
				}

				//如果是因为i < sStr.Size()不满足，而不是
				//遇到了一个非空白字符离开的while，则失败
				if (i >= sStr.Size())
				{
					return false;
				}

				c = sStr[i];
				return true;
			};

			char h, l;

			if (!GetNoSpaceChar(h))//没有高位，退出
			{
				break;
			}

			if (!GetNoSpaceChar(l))//仅有高位，低位补0
			{
				tRet.Push(CharsToHex(h, '0'));
				break;
			}

			//高低位合并
			tRet.Push(CharsToHex(h, l));
		}

		return tRet.Raw();
	}

public:
	template<StaticHexArrayStaticStr str, typename T = uint8_t>
	consteval static auto ToHexArr(void) noexcept
	{
		constexpr auto szArrSize = Parse<str, Counter>();
		return Parse < str, Array<T, szArrSize>();
	}
};





//template<my::StaticStr sStr>
//consteval auto operator""_Hex(void) noexcept
//{
//	//return std::array<uint8_t, >{};
//}