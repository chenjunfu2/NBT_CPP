#pragma once

#include "NBT_Reader.hpp"
#include "NBT_Visitor.hpp"

class NBT_Scanner
{
	template<typename T>
	using DefaultInputStream = NBT_Reader::DefaultInputStream<T>;







public:
	//返回值表示因为内部错误自行退出的，还是用户主动退出的（哪怕用户捕获到错误进行主动退出）
	//true:用户主动退出   false:内部错误自行退出

	template<typename InputStream, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static bool Scan(InputStream IptStream, Visitor &tVisitor)
	{
		//TODO
	}
	
	template<typename DataType = std::vector<uint8_t>, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static bool Scan(const DataType &tDataInput, size_t szStartIdx, Visitor &tVisitor)
	{
		DefaultInputStream<DataType> IptStream(tDataInput, szStartIdx);


		//TODO
	}
};
