#pragma once

#include <bit>//�ֽ���
#include <stdint.h>//����
#include <stddef.h>//size_t
#include <utility>//std::index_sequence
#include <type_traits>//std::make_unsigned_t

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

class NBT_Endian
{
	NBT_Endian(void) = delete;
	~NBT_Endian(void) = delete;
	
private:
	constexpr static bool IsLittleEndian(void) noexcept
	{
		return std::endian::native == std::endian::little;
	}

	constexpr static bool IsBigEndian(void) noexcept
	{
		return std::endian::native == std::endian::big;
	}

public:
	template<typename T>
	requires std::integral<T>
	constexpr static T ByteSwapAny(T data) noexcept
	{
		//������2�ı���������ȷִ��byteswap
		static_assert(sizeof(T) % 2 == 0 || sizeof(T) == 1, "The size of T is not a multiple of 2 or equal to 1");

		//�����С��1ֱ�ӷ���
		if constexpr (sizeof(T) == 1)
		{
			return data;
		}

		//ͳһ���޷�������
		using UT = std::make_unsigned_t<T>;
		static_assert(sizeof(UT) == sizeof(T), "Unsigned type size mismatch");

		//��ȡ��̬��С
		constexpr size_t szSize = sizeof(T);
		constexpr size_t szHalf = sizeof(T) / 2;

		//��ʱ������
		UT tmp = 0;

		//(i < sizeof(T) / 2)ǰ�룬����
		[&] <size_t... i>(std::index_sequence<i...>) -> void
		{
			((tmp |= ((UT)data & ((UT)0xFF << (8 * i))) << 8 * (szSize - (i * 2) - 1)), ...);
		}(std::make_index_sequence<szHalf>{});

		//(i + szHalf >= sizeof(T) / 2)��룬����
		[&] <size_t... i>(std::index_sequence<i...>) -> void
		{
			((tmp |= ((UT)data & ((UT)0xFF << (8 * (i + szHalf)))) >> 8 * (i * 2 + 1)), ...);
		}(std::make_index_sequence<szHalf>{});

		//ת����ԭ�ȵ����Ͳ�����
		return (T)tmp;
	}

	static uint16_t ByteSwap16(uint16_t data) noexcept
	{
		//���ݱ������л��ڽ�ָ���ʹ��Ĭ��λ��ʵ��
#if COMPILER_MSVC
		return _byteswap_ushort(data);
#elif COMPILER_GCC || COMPILER_CLANG
		return __builtin_bswap16(data);
#else
		return ByteSwapAny(data);
#endif
	}

	static uint32_t ByteSwap32(uint32_t data) noexcept
	{
		//���ݱ������л��ڽ�ָ���ʹ��Ĭ��λ��ʵ��
#if COMPILER_MSVC
		return _byteswap_ulong(data);
#elif COMPILER_GCC || COMPILER_CLANG
		return __builtin_bswap32(data);
#else
		return ByteSwapAny(data);
#endif
	}

	static uint64_t ByteSwap64(uint64_t data) noexcept
	{
		//���ݱ������л��ڽ�ָ���ʹ��Ĭ��λ��ʵ��
#if COMPILER_MSVC
		return _byteswap_uint64(data);
#elif COMPILER_GCC || COMPILER_CLANG
		return __builtin_bswap64(data);
#else
		return ByteSwapAny(data);
#endif
	}

	template<typename T>
	requires std::integral<T>
	constexpr static T AutoByteSwap(T data) noexcept
	{
		//�������֪��С�����������أ���Ϊ���ظ��п�����ָ�֧�ֵĸ�Чʵ��
		//������λ����ʵ�֣�Ч�ʸ��͵��Ǽ����Ը���
		if constexpr (sizeof(T) == sizeof(uint8_t))
		{
			return data;
		}
		else if constexpr (sizeof(T) == sizeof(uint16_t))
		{
			return (T)ByteSwap16((uint16_t)data);
		}
		else if constexpr (sizeof(T) == sizeof(uint32_t))
		{
			return (T)ByteSwap32((uint32_t)data);
		}
		else if constexpr (sizeof(T) == sizeof(uint64_t))
		{
			return (T)ByteSwap64((uint64_t)data);
		}
		else
		{
			return ByteSwapAny(data);
		}
	}

	//------------------------------------------------------//

	template<typename T>
	requires std::integral<T>
	static T NativeToBigAny(T data) noexcept
	{
		if constexpr (IsBigEndian())//��ǰҲ��big
		{
			return data;
		}

		//��ǰ��little��littleת����big
		return AutoByteSwap(data);
	}

	template<typename T>
	requires std::integral<T>
	static T NativeToLittleAny(T data) noexcept
	{
		if constexpr (IsLittleEndian())//��ǰҲ��little
		{
			return data;
		}

		//��ǰ��big��bigת����little
		return AutoByteSwap(data);
	}

	template<typename T>
	requires std::integral<T>
	static T BigToNativeAny(T data) noexcept
	{
		if constexpr (IsBigEndian())//��ǰҲ��big
		{
			return data;
		}

		//��ǰ��little��bigת����little
		return AutoByteSwap(data);
	}

	template<typename T>
	requires std::integral<T>
	static T LittleToNativeAny(T data) noexcept
	{
		if constexpr (IsLittleEndian())//��ǰҲ��little
		{
			return data;
		}

		//��ǰ��big��littleת����big
		return AutoByteSwap(data);
	}
};