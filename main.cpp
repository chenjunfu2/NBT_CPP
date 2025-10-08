#include "nbt/NBT_All.hpp"

#include <stdio.h>
#include <locale.h>

bool WriteNbtFile(const char *pFileName, const NBT_Type::Compound &cpd)
{
	std::vector<uint8_t> data;
	return NBT_Writer::WriteNBT({ data }, cpd) && NBT_IO::WriteFile(pFileName, data);
}

bool ReadNbtFile(const char *pFileName, NBT_Type::Compound &cpd)
{
	std::vector<uint8_t> data;
	return NBT_IO::ReadFile(pFileName, data) && NBT_Reader::ReadNBT({ data }, cpd);
}

int main(void)
{
	setlocale(LC_ALL, "zh_CN.UTF-8");

	//插入各种示例数据
	NBT_Type::Compound cpd;
	cpd.PutByte(MU8STR("byte"), -128);
	cpd.PutShort(MU8STR("short"), 32767);
	cpd.PutInt(MU8STR("int"), -2147483648);
	cpd.PutLong(MU8STR("long"), 1145141919810LL);
	cpd.PutFloat(MU8STR("float"), -0.1145f);
	cpd.PutDouble(MU8STR("double"), 114514.1919810);
	cpd.PutString(MU8STR("string"), MU8STR("测试"));

	//插入数组
	NBT_Type::Compound cpd2;
	cpd2.PutByteArray(MU8STR("byte array"), { 1,2,3,4,5,6,7 });
	cpd2.PutByteArray(MU8STR("int array"), { 1,2,3,4,5,6,7 });
	cpd2.PutByteArray(MU8STR("long array"), { 1,2,3,4,5,6,7 });

	//转移所有权
	cpd.PutCompound(MU8STR("compound"), std::move(cpd2));
	
	NBT_Type::List list;
	list.AddBackString(MU8STR("test str0"));
	list.AddBackString(MU8STR("test str1"));
	list.AddBackString(MU8STR("test str2"));
	list.AddBackString(MU8STR("test str3"));
	list.AddBackString(MU8STR("test str4"));
	list.AddBackString(MU8STR("test str5"));

	//转移所有权
	cpd.PutList(MU8STR("list"), std::move(list));

	//打印并写入文件
	NBT_Helper::Print(cpd);

	//通过一个componud下挂载一个空名称根来创建符合Mojang NBT标准的输出
	bool bw = WriteNbtFile("test.nbt", NBT_Type::Compound{ {MU8STR(""),cpd} });
	if (!bw)
	{
		return -1;
	}


	//从文件中读入
	NBT_Type::Compound readCpd{};
	bool br = ReadNbtFile("test.nbt", readCpd);
	if (!br)
	{
		return -1;
	}

	//丢弃空根
	readCpd = std::move(readCpd.GetCompound(MU8STR("")));

	NBT_Print{}("cmp:{}\n", cpd == readCpd);

	return 0;
}