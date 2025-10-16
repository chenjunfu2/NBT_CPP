#include <nbt_cpp/NBT_Endian.hpp>
#include <my/MyAssert.hpp>

#define U64VAL (uint64_t)0x12'34'56'78'9A'BC'DE'F0
#define _U64VAL (uint64_t)0xF0'DE'BC'9A'78'56'34'12

#define U32VAL (uint32_t)0x12'34'56'78
#define _U32VAL (uint32_t)0x78'56'34'12

#define U16VAL (uint16_t)0x12'34
#define _U16VAL (uint16_t)0x34'12

#define U8VAL (uint8_t)0x12
#define _U8VAL (uint8_t)0x12

int main(void)
{
	MyAssert(NBT_Endian::ByteSwapAny(U8VAL) == _U8VAL);
	MyAssert(NBT_Endian::ByteSwapAny(U16VAL) == _U16VAL);
	MyAssert(NBT_Endian::ByteSwapAny(U32VAL) == _U32VAL);
	MyAssert(NBT_Endian::ByteSwapAny(U64VAL) == _U64VAL);

	MyAssert(NBT_Endian::ByteSwap16(U16VAL) == _U16VAL);
	MyAssert(NBT_Endian::ByteSwap32(U32VAL) == _U32VAL);
	MyAssert(NBT_Endian::ByteSwap64(U64VAL) == _U64VAL);
	
	return 0;
}