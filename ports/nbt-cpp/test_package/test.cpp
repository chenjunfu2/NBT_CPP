#include <nbt_cpp/NBT_All.hpp>

#include <vector>

int main(void)
{
	//插入各种示例数据
	NBT_Type::Compound cpd;
	cpd.PutByte(MU8STR("byte"), -128);
	cpd.PutShort(MU8STR("short"), 32767);
	cpd.PutInt(MU8STR("int"), -2147483648);
	cpd.PutLong(MU8STR("long"), 1145141919810LL);
	cpd.PutFloat(MU8STR("float"), -0.1145f);
	cpd.PutDouble(MU8STR("double"), 114514.1919810);
	cpd.PutString(MU8STR("string"), MU8STR("测试"));

	//插入数组（换一种方式）
	NBT_Type::Compound cpd2;
	cpd2.Put(MU8STR("byte array"), NBT_Type::ByteArray{ 1,2,3,4,5,6,7 });
	cpd2.Put(MU8STR("int array"), NBT_Type::IntArray{ 1,2,3,4,5,6,7 });
	cpd2.Put(MU8STR("long array"), NBT_Type::LongArray{ 1,2,3,4,5,6,7 });

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

	//cpdWrite写入到data
	std::vector<uint8_t> data{};
	NBT_Type::Compound cpdWrite{ { MU8STR(""),std::move(cpd)} };
	NBT_Writer::WriteNBT(data, 0, cpdWrite);

	//再次读入到cpdRead
	NBT_Type::Compound cpdRead{};
	NBT_Reader::ReadNBT(data, 0, cpdRead);

	//确认相等
	return cpdWrite == cpdRead ? 0 : -1;
}