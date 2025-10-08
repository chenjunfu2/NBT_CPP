#include "nbt/NBT_All.hpp"

#include <stdio.h>

int main(void)
{
	NBT_Node n{};

	NBT_Type::Compound cpd;

	cpd.PutDouble(MU8STR("d"), 0.114514);
	NBT_Helper::Print(cpd);


	return 114514;
}