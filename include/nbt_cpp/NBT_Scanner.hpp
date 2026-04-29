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
///@cond
	enum class Control : uint8_t
	{
		Continue,	///< 继续处理（继续迭代）
		Break,		///< 跳过剩余值（离开当前结构层级回到父层级）
		Stop,		///< 停止处理（终止解析）
		Error,		///< 出现错误（终止解析）
	};

	Control ResultControlToControl(NBT_Visitor::ResultControl enResultControl)
	{
		switch (enResultControl)
		{
		case NBT_Visitor::ResultControl::Continue:	return Control::Continue;
		case NBT_Visitor::ResultControl::Break:		return Control::Break;
		case NBT_Visitor::ResultControl::Stop:		return Control::Stop;
		default:									return Control::Error;
		}
	}

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

	template<typename InputStream>
	static bool GetName(InputStream &tData, NBT_Type::String &tName)
	{
		//读取长度
		NBT_Type::StringLength wStringLength = 0;//w->word=2*byte
		if (!ReadBigEndian(tData, wStringLength))
		{
			return false;
		}

		using ValueType = NBT_Type::String::value_type;
		size_t szStringLength = (size_t)wStringLength;
		size_t szStringSize = szStringLength * sizeof(ValueType);

		//检查长度
		if (!tData.HasAvailData(szStringSize))
		{
			return false;
		}

		//解析数据
		tName.reserve(szStringLength);//提前分配
		tName.assign((const ValueType *)tData.CurData(), szStringLength);//构造string（如果长度为0则构造0长字符串，合法行为）
		
		tData.AddIndex(szStringSize);//移动下标

		return true;
	}

	template<typename InputStream>
	static bool SkipName(InputStream &tData)
	{
		//读取长度
		NBT_Type::StringLength wStringLength = 0;//w->word=2*byte
		if (!ReadBigEndian(tData, wStringLength))
		{
			return false;
		}

		size_t szSkipSize = (size_t)wStringLength * sizeof(NBT_Type::String::value_type);

		//检查长度
		if (!tData.HasAvailData(szSkipSize))
		{
			return false;
		}

		//跳过数据
		tData.AddIndex(szSkipSize);
		return true;
	}

	template<typename InputStream, typename Visitor>
	static Control ScanEndType(InputStream &tData, Visitor &tVisitor)
	{
		return ResultControlToControl(tVisitor.VisitListEnd());
	}

	template<typename InputStream>
	static bool SkipEndType(InputStream &tData)
	{
		//EndType无数据负载
		return true;
	}

	template<typename T, typename InputStream, typename Visitor>
	static Control ScanBuiltInType(InputStream &tData, Visitor &tVisitor)
	{
		using RAW_DATA_T = NBT_Type::BuiltinRawType_T<T>;//类型映射
		RAW_DATA_T tTmpRawData = 0;
		if (!ReadBigEndian(tData, tTmpRawData))
		{
			return Control::Error;
		}

		return ResultControlToControl(tVisitor.VisitNumericResult<T>(tTmpRawData));
	}

	template<typename T, typename InputStream>
	static bool SkipBuiltInType(InputStream &tData)
	{
		using RAW_DATA_T = NBT_Type::BuiltinRawType_T<T>;//类型映射
		size_t szSkipSize = sizeof(RAW_DATA_T);

		if (!tData.HasAvailData(szSkipSize))
		{
			return false;
		}

		//跳过数据
		tData.AddIndex(szSkipSize);
		return true;
	}

	template<typename T, typename InputStream, typename Visitor>
	static Control ScanArrayType(InputStream &tData, Visitor &tVisitor)
	{
		//获取4字节有符号数，代表数组元素个数
		NBT_Type::ArrayLength iArrayLength = 0;//4byte
		if (!ReadBigEndian(tData, iArrayLength))
		{
			return Control::Error;
		}

		//检查有符号数大小范围
		if (iArrayLength < 0)
		{
			return Control::Error;
		}

		using ValueType = typename T::value_type;
		size_t szArrayLength = (size_t)iArrayLength;
		size_t szArraySize = szArrayLength * sizeof(ValueType);

		if (!tData.HasAvailData(szArraySize))
		{
			return Control::Error;
		}

		//数组保存
		T tArray{};
		tArray.reserve(szArrayLength);
		//读取dElementCount个元素
		for (size_t i = 0; i < szArrayLength; ++i)
		{
			ValueType tTmpData{};
			ReadBigEndian<true>(tData, tTmpData);//调用需要确保范围安全
			tArray.emplace_back(std::move(tTmpData));//读取一个插入一个
		}

		return ResultControlToControl(tVisitor.VisitArrayResult<T>(std::move(tArray)));
	}

	template<typename T, typename InputStream>
	static bool SkipArrayType(InputStream &tData)
	{
		//获取4字节有符号数，代表数组元素个数
		NBT_Type::ArrayLength iArrayLength = 0;//4byte
		if (!ReadBigEndian(tData, iArrayLength))
		{
			return false;
		}

		//检查有符号数大小范围
		if (iArrayLength < 0)
		{
			return false;
		}

		size_t szSkipSize = (size_t)iArrayLength * sizeof(T::value_type);

		if (!tData.HasAvailData(szSkipSize))
		{
			return false;
		}

		tData.AddIndex(szSkipSize);
		return true;
	}

	template<typename InputStream, typename Visitor>
	static Control ScanStringType(InputStream &tData, Visitor &tVisitor)
	{
		NBT_Type::String tString;
		if (!GetName(tData, tString))
		{
			return Control::Error;
		}

		return ResultControlToControl(tVisitor.VisitStringResult(std::move(tString)));
	}

	template<typename InputStream>
	static bool SkipStringType(InputStream &tData)
	{
		return SkipName(tData);
	}

	template<typename InputStream, typename Visitor>
	static Control ScanListType(InputStream &tData, Visitor &tVisitor, size_t szStackDepth)
	{
		if (szStackDepth == 0)
		{
			return Control::Error;
		}

		NBT_TAG_RAW_TYPE enListElementTag = 0;//b=byte
		if (!ReadBigEndian(tData, enListElementTag))
		{
			return Control::Error;
		}

		if (enListElementTag >= NBT_TAG::ENUM_END)
		{

		}




	}

	template<typename InputStream>
	static bool SkipListType(InputStream &tData, size_t szStackDepth)
	{
		//TODO
	}

	template<typename InputStream, typename Visitor>
	static Control ScanCompoundType(InputStream &tData, Visitor &tVisitor, size_t szStackDepth)
	{
		//TODO
	}

	template<typename InputStream>
	static bool SkipCompoundType(InputStream &tData, size_t szStackDepth)
	{
		//TODO
	}

	template<typename InputStream, typename Visitor>
	static Control ScanSwitch(InputStream &tData, Visitor &tVisitor, size_t szStackDepth)
	{
		if (!tData.HasAvailData(sizeof(NBT_TAG_RAW_TYPE)))
		{
			return Control::Error;
		}

		Control retControl;
		NBT_TAG tagNbt = (NBT_TAG)(NBT_TAG_RAW_TYPE)tData.GetNext();
		switch (tagNbt)
		{
		case NBT_TAG::End:
			{
				retControl = ScanEndType(tData, tVisitor);
			}
			break;
		case NBT_TAG::Byte:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Byte>;
				retControl = ScanBuiltInType<CurType>(tData, tVisitor);
			}
			break;
		case NBT_TAG::Short:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Short>;
				retControl = ScanBuiltInType<CurType>(tData, tVisitor);
			}
			break;
		case NBT_TAG::Int:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Int>;
				retControl = ScanBuiltInType<CurType>(tData, tVisitor);
			}
			break;
		case NBT_TAG::Long:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Long>;
				retControl = ScanBuiltInType<CurType>(tData, tVisitor);
			}
			break;
		case NBT_TAG::Float:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Float>;
				retControl = ScanBuiltInType<CurType>(tData, tVisitor);
			}
			break;
		case NBT_TAG::Double:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Double>;
				retControl = ScanBuiltInType<CurType>(tData, tVisitor);
			}
			break;
		case NBT_TAG::ByteArray:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::ByteArray>;
				retControl = ScanArrayType<CurType>(tData, tVisitor);
			}
			break;
		case NBT_TAG::String:
			{
				retControl = ScanStringType(tData, tVisitor);
			}
			break;
		case NBT_TAG::List:
			{
				retControl = ScanListType(tData, tVisitor);
			}
			break;
		case NBT_TAG::Compound:
			{
				retControl = ScanCompoundType(tData, tVisitor);
			}
			break;
		case NBT_TAG::IntArray:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::IntArray>;
				retControl = ScanArrayType<CurType>(tData, tVisitor);
			}
			break;
		case NBT_TAG::LongArray:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::LongArray>;
				retControl = ScanArrayType<CurType>(tData, tVisitor);
			}
			break;
		default:
			{
				retControl = Control::Error;
			}
			break;
		}

		//TODO
		switch (retControl)
		{
		case NBT_Scanner::Control::Continue:
			break;
		case NBT_Scanner::Control::Break:
			break;
		case NBT_Scanner::Control::Stop:
			break;
		case NBT_Scanner::Control::Error:
			break;
		default:
			break;
		}

		return retControl;
	}

	template<typename InputStream>
	static bool SkipSwitch(InputStream &tData, size_t szStackDepth)
	{
		//TODO
	}

///@endcond

public:
	template<typename InputStream, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static bool Scan(InputStream &IptStream, Visitor &tVisitor, size_t szStackDepth = 512)
	{
		tVisitor.VisitBegin();
		//TODO
		tVisitor.VisitEnd();
	}
	
	template<typename DataType = std::vector<uint8_t>, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static bool Scan(const DataType &tDataInput, size_t szStartIdx, Visitor &tVisitor, size_t szStackDepth = 512)
	{
		NBT_IO::DefaultInputStream<DataType> IptStream(tDataInput, szStartIdx);

		tVisitor.VisitBegin();
		//TODO
		tVisitor.VisitEnd();
	}

};
