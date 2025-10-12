#include <stdio.h>

#include <my/StaticHexArray.hpp>
#include "..\..\StaticHexArray.hpp"

int main(void)
{
	constexpr auto h = StaticHexArray::ToHexArr<"ABCDEF", uint8_t>();
	return 0;
}