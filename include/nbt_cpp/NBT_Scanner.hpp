#pragma once

#include "NBT_Reader.hpp"//友元
#include "NBT_Visitor.hpp"//鸭子类
#include "NBT_Print.hpp"//打印输出
#include "NBT_IO.hpp"//IO流

#include <stdint.h>

class NBT_Scanner
{
	/// @brief 禁止构造
	NBT_Scanner(void) = delete;
	/// @brief 禁止析构
	~NBT_Scanner(void) = delete;

protected:
	enum class Control : uint8_t
	{
		Continue,	///< 继续处理（继续迭代）
		Break,		///< 跳过剩余值（离开当前结构层级回到父层级）
		Stop,		///< 停止处理（终止解析）
	};

	template<bool bNoCheck = false, typename T, typename InputStream, typename InfoFunc>
	requires std::integral<T>
	static inline std::conditional_t<bNoCheck, void, bool> ReadBigEndian(InputStream &tData, T &tVal)
	{
		if constexpr (!bNoCheck)
		{
			if (!tData.HasAvailData(sizeof(T)))
			{
				return false;
			}
		}

		T BigEndianVal{};
		tData.GetRange((uint8_t *)&BigEndianVal, sizeof(BigEndianVal));
		tVal = NBT_Endian::BigToNativeAny(BigEndianVal);

		if constexpr (!bNoCheck)
		{
			return true;
		}
	}

	//TODO
	template<typename InputStream, typename Visitor>
	static Control ScanSwitch(InputStream &tData, Visitor &tVisitor, size_t szStackDepth)
	{
	MYTRY;
		ErrCode eRet = AllOk;

		if (!tData.HasAvailData(sizeof(NBT_TAG_RAW_TYPE)))
		{
			return eRet;
		}

		
		NBT_TAG tagNbt = (NBT_TAG)(NBT_TAG_RAW_TYPE)tData.GetNext();
		switch (tagNbt)
		{
		case NBT_TAG::End:
			switch (c)
			{
			case NBT_Visitor::ResultControl::Continue:
				break;
			case NBT_Visitor::ResultControl::Break:
				break;
			case NBT_Visitor::ResultControl::Stop:
				break;
			case NBT_Visitor::ResultControl::Error:
				break;
			default:
				break;
			}
			break;
		case NBT_TAG::Byte:
			break;
		case NBT_TAG::Short:
			break;
		case NBT_TAG::Int:
			break;
		case NBT_TAG::Long:
			break;
		case NBT_TAG::Float:
			break;
		case NBT_TAG::Double:
			break;
		case NBT_TAG::ByteArray:
			break;
		case NBT_TAG::String:
			break;
		case NBT_TAG::List:
			break;
		case NBT_TAG::Compound:
			break;
		case NBT_TAG::IntArray:
			break;
		case NBT_TAG::LongArray:
			break;
		case NBT_TAG::ENUM_END:
			break;
		default:
			break;
		}

		return eRet;
	MYCATCH;
	}







public:
	template<typename InputStream, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static void Scan(InputStream &IptStream, Visitor &tVisitor, size_t szStackDepth = 512)
	{
		tVisitor.VisitBegin();
		//TODO
		tVisitor.VisitEnd();
	}
	
	template<typename DataType = std::vector<uint8_t>, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static void Scan(const DataType &tDataInput, size_t szStartIdx, Visitor &tVisitor, size_t szStackDepth = 512)
	{
		NBT_IO::DefaultInputStream<DataType> IptStream(tDataInput, szStartIdx);

		tVisitor.VisitBegin();
		//TODO
		tVisitor.VisitEnd();
	}

};
