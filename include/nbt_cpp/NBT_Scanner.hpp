#pragma once

#include "NBT_Reader.hpp"
#include "NBT_Visitor.hpp"

class NBT_Scanner
{
	template<typename T>
	using DefaultInputStream = NBT_Reader::DefaultInputStream<T>;







public:
	template<typename InputStream, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static void Scan(InputStream IptStream, Visitor &tVisitor)
	{
		//TODO
	}
	
	template<typename DataType = std::vector<uint8_t>, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static void Scan(const DataType &tDataInput, size_t szStartIdx, Visitor &tVisitor)
	{
		DefaultInputStream<DataType> IptStream(tDataInput, szStartIdx);


		//TODO
	}
};
