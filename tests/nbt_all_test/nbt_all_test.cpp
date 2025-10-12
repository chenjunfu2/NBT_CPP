#include <stdio.h>
#include <my/StaticHexArray.hpp>

template<typename T>
void PrintHex()
{






}

int main(void)
{
	StaticHexArrayStaticStr str("test");

	constexpr auto h = StaticHexArray::ToHexArr<"ABCDEF">();
	return 0;
}