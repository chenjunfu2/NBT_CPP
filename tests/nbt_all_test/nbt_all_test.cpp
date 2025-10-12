#include <stdio.h>
#include <my/StaticHexArray.hpp>

template<typename T>
void PrintHex(T data) noexcept
{
	constexpr uint8_t u8CountOfLine = 16;
	uint8_t u8CountCur = 0;
	
	for (size_t iAddr = 0; iAddr < data.size(); ++iAddr)
	{
		if (u8CountCur == 0)
		{
			printf("0x%0*zX: ", (int)sizeof(iAddr) * 2, iAddr);
		}

		printf("%02X ", (uint8_t)data[iAddr]);

		if (++u8CountCur == u8CountOfLine)
		{
			putchar('\n');
			u8CountCur = 0;
		}
	}
}

int main(void)
{
	constexpr auto h = StaticHexArray::ToHexArr<"ABCDEF">();
	PrintHex(h);

	return 0;
}