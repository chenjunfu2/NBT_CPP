#pragma once

#include "NBT_Reader.hpp"
#include "NBT_Visitor.hpp"

class NBT_Scanner
{
	template<typename T>
	using DefaultInputStream = NBT_Reader::DefaultInputStream<T>;







public:
	//返回值表示因为错误退出的，还是用正常退出的
	//true:正常退出   false:错误退出

	template<typename InputStream, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static bool Scan(InputStream IptStream, Visitor &tVisitor)
	{
		tVisitor.VisitBegin();
		//TODO
		tVisitor.VisitEnd();
	}
	
	template<typename DataType = std::vector<uint8_t>, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static bool Scan(const DataType &tDataInput, size_t szStartIdx, Visitor &tVisitor)
	{
		DefaultInputStream<DataType> IptStream(tDataInput, szStartIdx);

		tVisitor.VisitBegin();
		//TODO

		tVisitor.VisitEnd();
	}
};
