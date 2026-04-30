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

	static Control ResultControlToControl(NBT_Visitor::ResultControl enResultControl)
	{
		switch (enResultControl)
		{
		case NBT_Visitor::ResultControl::Continue:	return Control::Continue;	break;
		case NBT_Visitor::ResultControl::Break:		return Control::Break;		break;
		case NBT_Visitor::ResultControl::Stop:		return Control::Stop;		break;
		default:									return Control::Error;		break;
		}
	}

	template<bool bNoCheck = false, typename T, typename InputStream>
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

		NBT_TAG_RAW_TYPE u8ListElementTag = 0;//b=byte
		if (!ReadBigEndian(tData, u8ListElementTag))
		{
			return Control::Error;
		}

		NBT_TAG enListElementTag = (NBT_TAG)u8ListElementTag;

		if (enListElementTag >= NBT_TAG::ENUM_END)
		{
			return Control::Error;
		}

		NBT_Type::ListLength iListLength = 0;//4byte
		if (!ReadBigEndian(tData, iListLength))
		{
			return Control::Error;
		}

		if (iListLength < 0)
		{
			return Control::Error;
		}

		size_t szListLength = (size_t)iListLength;
		if (enListElementTag == NBT_TAG::End && szListLength != 0)
		{
			return Control::Error;
		}

		if (szListLength == 0 && enListElementTag != NBT_TAG::End)
		{
			enListElementTag = NBT_TAG::End;
		}

		//遍历索引
		size_t i = 0;

		NBT_Visitor::ResultControl visitRet = tVisitor.VisitListBegin(enListElementTag, szListLength);
		switch (visitRet)
		{
		case NBT_Visitor::ResultControl::Continue://继续处理
			break;
		case NBT_Visitor::ResultControl::Break:
			goto skip_any;
			break;
		case NBT_Visitor::ResultControl::Stop:
			return Control::Stop;
			break;
		default:
			return Control::Error;
			break;
		}

		for (; i < szListLength; ++i)
		{
			NBT_Visitor::NestingControl visitRet = tVisitor.VisitListNextElement(enListElementTag, i);
			switch (visitRet)
			{
			case NBT_Visitor::NestingControl::Enter:
				break;
			case NBT_Visitor::NestingControl::Skip:
				{
					if (!SkipSwitch(tData, enListElementTag, szStackDepth - 1))
					{
						return Control::Error;
					}
					continue;
				}
				break;
			case NBT_Visitor::NestingControl::Break:
				//跳过剩余所有列表元素
				goto skip_any;
				break;
			case NBT_Visitor::NestingControl::Stop:
				return Control::Stop;
				break;
			default:
				return Control::Error;
				break;
			}

			Control visitSubRet = ScanSwitch(tData, enListElementTag, tVisitor, szStackDepth - 1);
			switch (visitSubRet)
			{
			case Control::Continue://继续（什么也不做）
			case Control::Break://跳过（从内部跳出）（什么也不做）
				break;
			case Control::Stop:
				return Control::Stop;
				break;
			case Control::Error:
			default:
				return Control::Error;
				break;
			}
		}

	skip_any:
		for (size_t j = i; j < szListLength; ++j)//从当前i开始跳到结束
		{
			if (!SkipSwitch(tData, enListElementTag, szStackDepth - 1))
			{
				return Control::Error;
			}
		}

		return ResultControlToControl(tVisitor.VisitListEnd());
	}

	template<typename InputStream>
	static bool SkipListType(InputStream &tData, size_t szStackDepth)
	{
		if (szStackDepth == 0)
		{
			return false;
		}

		NBT_TAG_RAW_TYPE u8ListElementTag = 0;//b=byte
		if (!ReadBigEndian(tData, u8ListElementTag))
		{
			return false;
		}

		NBT_TAG enListElementTag = (NBT_TAG)u8ListElementTag;

		if (enListElementTag >= NBT_TAG::ENUM_END)
		{
			return false;
		}

		NBT_Type::ListLength iListLength = 0;//4byte
		if (!ReadBigEndian(tData, iListLength))
		{
			return false;
		}

		if (iListLength < 0)
		{
			return false;
		}

		size_t szListLength = (size_t)iListLength;
		if (enListElementTag == NBT_TAG::End && szListLength != 0)
		{
			return false;
		}

		if (szListLength == 0 && enListElementTag != NBT_TAG::End)
		{
			enListElementTag = NBT_TAG::End;
		}

		size_t szSkipLength = szListLength;

		for (size_t i = 0; i < szSkipLength; ++i)
		{
			if (!SkipSwitch(tData, enListElementTag, szStackDepth - 1))
			{
				return false;
			}
		}

		return true;
	}

	template<bool bRoot, typename InputStream, typename Visitor>
	static Control ScanCompoundType(InputStream &tData, Visitor &tVisitor, size_t szStackDepth)
	{
		if (szStackDepth == 0)
		{
			return Control::Error;
		}

		NBT_Visitor::ResultControl visitBegRet = tVisitor.VisitCompoundBegin();
		switch (visitBegRet)//TODO
		{
		case NBT_Visitor::ResultControl::Continue://继续
			break;
		case NBT_Visitor::ResultControl::Break:
			goto skip_any;
			break;
		case NBT_Visitor::ResultControl::Stop:
			return Control::Stop;
			break;
		default:
			return Control::Error;
			break;
		}

		while (true)
		{
			//处理末尾情况
			if (!tData.HasAvailData(sizeof(NBT_TAG_RAW_TYPE)))
			{
				if constexpr (!bRoot)//非根部情况遇到末尾，则报错
				{
					return Control::Error;
				}

				return Control::Stop;//结束
			}

			//先读取一下类型
			NBT_TAG enCompoundEntryTag = (NBT_TAG)(NBT_TAG_RAW_TYPE)tData.GetNext();
			if (enCompoundEntryTag == NBT_TAG::End)//处理End情况
			{
				return ResultControlToControl(tVisitor.VisitCompoundEnd());
			}

			if (enCompoundEntryTag >= NBT_TAG::ENUM_END)
			{
				return Control::Error;
			}

			NBT_Visitor::NestingControl visitTypeRet = tVisitor.VisitCompoundNextEntryType(enCompoundEntryTag);
			switch (visitTypeRet)
			{
			case NBT_Visitor::NestingControl::Enter://继续
				break;
			case NBT_Visitor::NestingControl::Skip://跳过一个
				{
					if (!SkipName(tData))
					{
						return Control::Error;
					}

					if (!SkipSwitch(tData, enCompoundEntryTag, szStackDepth - 1))
					{
						return Control::Error;
					}
				}
				break;
			case NBT_Visitor::NestingControl::Break:
				{
					if (!SkipName(tData))//先跳过当前剩余
					{
						return Control::Error;
					}

					if (!SkipSwitch(tData, enCompoundEntryTag, szStackDepth - 1))
					{
						return Control::Error;
					}
					goto skip_any;
				}
				break;
			case NBT_Visitor::NestingControl::Stop:
				return Control::Stop;
				break;
			default:
				return Control::Error;
				break;
			}


			NBT_Type::String sName{};
			if (!GetName(tData, sName))
			{
				return Control::Error;
			}

			NBT_Visitor::NestingControl visitEntryRet = tVisitor.VisitCompoundNextEntry(enCompoundEntryTag, std::move(sName));
			switch (visitEntryRet)//TODO
			{
			case NBT_Visitor::NestingControl::Enter://继续
				break;
			case NBT_Visitor::NestingControl::Skip://跳过一个
				{
					if (!SkipSwitch(tData, enCompoundEntryTag, szStackDepth - 1))
					{
						return Control::Error;
					}
				}
				break;
			case NBT_Visitor::NestingControl::Break://跳过剩余
				{
					if (!SkipSwitch(tData, enCompoundEntryTag, szStackDepth - 1))//先跳过当前
					{
						return Control::Error;
					}
					goto skip_any;
				}
				break;
			case NBT_Visitor::NestingControl::Stop:
				return Control::Stop;
				break;
			default:
				return Control::Error;
				break;
			}

			Control visitSubRet = ScanSwitch(tData, enCompoundEntryTag, tVisitor, szStackDepth - 1);
			switch (visitSubRet)
			{
			case Control::Continue://啥也不做（继续）
			case Control::Break://从内部跳出（啥也不做）（继续）
				break;
			case Control::Stop:
				return Control::Stop;
				break;
			case Control::Error:
			default:
				return Control::Error;
				break;
			}
		}

	skip_any://跳过剩下所有数据
		while (true)
		{
			//跳过剩余的compound值
			//处理末尾情况
			if (!tData.HasAvailData(sizeof(NBT_TAG_RAW_TYPE)))
			{
				if constexpr (!bRoot)//非根部情况遇到末尾，则报错
				{
					return Control::Error;
				}

				return Control::Stop;//结束
			}

			NBT_TAG enCompoundEntryTag = (NBT_TAG)(NBT_TAG_RAW_TYPE)tData.GetNext();

			if (enCompoundEntryTag == NBT_TAG::End)
			{
				return ResultControlToControl(tVisitor.VisitCompoundEnd());
			}

			if (enCompoundEntryTag >= NBT_TAG::ENUM_END)
			{
				return Control::Error;
			}

			if (!SkipName(tData))
			{
				return Control::Error;
			}

			if (!SkipSwitch(tData, enCompoundEntryTag, szStackDepth - 1))
			{
				return Control::Error;
			}
		}

		return ResultControlToControl(tVisitor.VisitCompoundEnd());//返回
	}

	template<typename InputStream>
	static bool SkipCompoundType(InputStream &tData, size_t szStackDepth)
	{
		if (szStackDepth == 0)
		{
			return Control::Error;
		}

		while (true)
		{
			//跳过compound值
			if (!tData.HasAvailData(sizeof(NBT_TAG_RAW_TYPE)))//处理末尾情况
			{
				return false;//跳过必然不可能是根部调用
			}

			NBT_TAG enCompoundEntryTag = (NBT_TAG)(NBT_TAG_RAW_TYPE)tData.GetNext();

			if (enCompoundEntryTag == NBT_TAG::End)
			{
				return true;
			}

			if (enCompoundEntryTag >= NBT_TAG::ENUM_END)
			{
				return false;
			}

			if (!SkipName(tData))
			{
				return false;
			}

			if (!SkipSwitch(tData, enCompoundEntryTag, szStackDepth - 1))
			{
				return false;
			}
		}

		return true;
	}

	template<typename InputStream, typename Visitor>
	static Control ScanSwitch(InputStream &tData, NBT_TAG tagNbt, Visitor &tVisitor, size_t szStackDepth)
	{
		Control retControl;
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
				retControl = ScanListType(tData, tVisitor, szStackDepth);
			}
			break;
		case NBT_TAG::Compound:
			{
				retControl = ScanCompoundType<false>(tData, tVisitor, szStackDepth);
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

		return retControl;
	}

	template<typename InputStream>
	static bool SkipSwitch(InputStream &tData, NBT_TAG tagNbt, size_t szStackDepth)
	{
		bool bRet = false;
		switch (tagNbt)
		{
		case NBT_TAG::End:
			{
				bRet = SkipEndType(tData);
			}
			break;
		case NBT_TAG::Byte:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Byte>;
				bRet = SkipBuiltInType<CurType>(tData);
			}
			break;
		case NBT_TAG::Short:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Short>;
				bRet = SkipBuiltInType<CurType>(tData);
			}
			break;
		case NBT_TAG::Int:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Int>;
				bRet = SkipBuiltInType<CurType>(tData);
			}
			break;
		case NBT_TAG::Long:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Long>;
				bRet = SkipBuiltInType<CurType>(tData);
			}
			break;
		case NBT_TAG::Float:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Float>;
				bRet = SkipBuiltInType<CurType>(tData);
			}
			break;
		case NBT_TAG::Double:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::Double>;
				bRet = SkipBuiltInType<CurType>(tData);
			}
			break;
		case NBT_TAG::ByteArray:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::ByteArray>;
				bRet = SkipArrayType<CurType>(tData);
			}
			break;
		case NBT_TAG::String:
			{
				bRet = SkipStringType(tData);
			}
			break;
		case NBT_TAG::List:
			{
				bRet = SkipListType(tData, szStackDepth);
			}
			break;
		case NBT_TAG::Compound:
			{
				bRet = SkipCompoundType(tData, szStackDepth);
			}
			break;
		case NBT_TAG::IntArray:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::IntArray>;
				bRet = SkipArrayType<CurType>(tData);
			}
			break;
		case NBT_TAG::LongArray:
			{
				using CurType = NBT_Type::TagToType_T<NBT_TAG::LongArray>;
				bRet = SkipArrayType<CurType>(tData);
			}
			break;
		default:
			{
				bRet = false;
			}
			break;
		}

		return bRet;
	}

///@endcond

public:
	template<typename InputStream, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static bool Scan(InputStream &IptStream, Visitor &tVisitor, size_t szStackDepth = 512)
	{
		tVisitor.VisitBegin();
		ScanCompoundType<true>(IptStream, tVisitor, szStackDepth);
		tVisitor.VisitEnd();
	}
	
	template<typename DataType = std::vector<uint8_t>, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static bool Scan(const DataType &tDataInput, size_t szStartIdx, Visitor &tVisitor, size_t szStackDepth = 512)
	{
		NBT_IO::DefaultInputStream<DataType> IptStream(tDataInput, szStartIdx);

		tVisitor.VisitBegin();
		ScanCompoundType<true>(IptStream, tVisitor, szStackDepth);
		tVisitor.VisitEnd();
	}

};
//TODO: 添加使用visit输出报错

