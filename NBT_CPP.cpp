#include "nbt/NBT_All.hpp"

#include <stdio.h>

int main(void)
{
	NBT_Node n{};

	NBT_Type::Compound cpd;

	cpd.PutDouble(MU8STR("d"), 0.114514);
	NBT_Helper::Print(cpd);

	std::vector<uint8_t> v;
	NBT_Writer::WriteNBT({ v }, cpd);

	NBT_IO::WriteFile("test.nbt", v);

	return 0;
}