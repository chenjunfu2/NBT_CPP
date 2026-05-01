#pragma once

#include "NBT_Visitor.hpp"//鸭子类与部分实现
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

		return ResultControlToControl(tVisitor.VisitNumericResult<T>(std::bit_cast<T>(tTmpRawData)));
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

		//先进性合法性检查
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
			ReadBigEndian<true>(tData, tTmpData);//调用需要确保范围安全（已在前面检查）
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

		tData.AddIndex(szSkipSize);//直接递增索引跳过
		return true;
	}

	template<typename InputStream, typename Visitor>
	static Control ScanStringType(InputStream &tData, Visitor &tVisitor)
	{
		NBT_Type::String tString;
		if (!GetName(tData, tString))//转发调用
		{
			return Control::Error;
		}

		return ResultControlToControl(tVisitor.VisitStringResult(std::move(tString)));
	}

	template<typename InputStream>
	static bool SkipStringType(InputStream &tData)
	{
		return SkipName(tData);//转发调用
	}

	template<typename InputStream, typename Visitor>
	static Control ScanListType(InputStream &tData, Visitor &tVisitor, size_t szStackDepth)
	{
		if (szStackDepth == 0)//栈深度
		{
			return Control::Error;
		}

		//读取列表标签
		NBT_TAG_RAW_TYPE u8ListElementTag = 0;//b=byte
		if (!ReadBigEndian(tData, u8ListElementTag))
		{
			return Control::Error;
		}

		//标签验证
		if (u8ListElementTag >= NBT_TAG::ENUM_END)
		{
			return Control::Error;
		}

		//类型转换
		NBT_TAG enListElementTag = (NBT_TAG)u8ListElementTag;

		//读取列表长度
		NBT_Type::ListLength iListLength = 0;//4byte
		if (!ReadBigEndian(tData, iListLength))
		{
			return Control::Error;
		}

		//验证
		if (iListLength < 0)
		{
			return Control::Error;
		}

		//类型转换
		size_t szListLength = (size_t)iListLength;

		//大小&类型判断
		if (enListElementTag == NBT_TAG::End && szListLength != 0)
		{
			return Control::Error;
		}

		//类型设置
		if (szListLength == 0 && enListElementTag != NBT_TAG::End)
		{
			enListElementTag = NBT_TAG::End;
		}

		//遍历索引
		size_t i = 0;//声明在外面，用于跳过剩余的起始位置

		//访问器开始回调
		switch (tVisitor.VisitListBegin(enListElementTag, szListLength))
		{
		case NBT_Visitor::ResultControl::Continue:	/*继续（什么也不做）*/	break;
		case NBT_Visitor::ResultControl::Break:		goto skip_any;			break;//这时候索引从0开始，跳过所有
		case NBT_Visitor::ResultControl::Stop:		return Control::Stop;	break;
		default:									return Control::Error;	break;
		}

		//遍历读取
		for (; i < szListLength; ++i)
		{
			//访问器元素回调
			switch (tVisitor.VisitListNextElement(enListElementTag, i))
			{
			case NBT_Visitor::NestingControl::Enter:	/*进入值（什么也不做）*/	break;
			case NBT_Visitor::NestingControl::Skip:		//跳过当前元素
				{
					if (!SkipSwitch(tData, enListElementTag, szStackDepth - 1))
					{
						return Control::Error;
					}
					continue;//跳过当前并继续
				}
				break;
			case NBT_Visitor::NestingControl::Break:	goto skip_any;			break;//跳过剩余所有列表元素，注意当前元素还未读取，所以下面依旧从i开始跳
			case NBT_Visitor::NestingControl::Stop:		return Control::Stop;	break;
			default:									return Control::Error;	break;
			}

			//元素递归访问
			switch (ScanSwitch(tData, enListElementTag, tVisitor, szStackDepth - 1))
			{
			case Control::Continue:	/*继续（什么也不做）*/	break;
			case Control::Break:	/*跳过（什么也不做）*/	break;//（从内部跳过之后传出的标签不具有传递性）
			case Control::Stop:		return Control::Stop;	break;
			case Control::Error:	return Control::Error;	break;
			default:				return Control::Error;	break;//非法标签
			}
		}

	skip_any://跳过剩余，如果没有则不跳过
		for (size_t j = i; j < szListLength; ++j)//从当前i开始跳到结束
		{
			if (!SkipSwitch(tData, enListElementTag, szStackDepth - 1))
			{
				return Control::Error;
			}
		}

		//返回控制标签
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

		if (u8ListElementTag >= NBT_TAG::ENUM_END)
		{
			return false;
		}

		NBT_TAG enListElementTag = (NBT_TAG)u8ListElementTag;

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
		//栈深度检查
		if (szStackDepth == 0)
		{
			return Control::Error;
		}

		if constexpr (!bRoot)//非根部才进行compound调用
		{
			switch (tVisitor.VisitCompoundBegin())
			{
			case NBT_Visitor::ResultControl::Continue:	/*继续（什么也不做）*/	break;
			case NBT_Visitor::ResultControl::Break:		goto skip_any;			break;//跳过所有
			case NBT_Visitor::ResultControl::Stop:		return Control::Stop;	break;
			default:									return Control::Error;	break;
			}
		}
		else//根部调用访问器起始回调
		{
			tVisitor.VisitBegin();
		}

		//循环直到内部退出
		while (true)
		{
			//处理末尾情况
			if (!tData.HasAvailData(sizeof(NBT_TAG_RAW_TYPE)))
			{
				if constexpr (!bRoot)//非根部情况遇到末尾，则报错
				{
					return Control::Error;
				}
				else//根部遇到末尾，正常结束
				{
					return Control::Stop;
				}
			}

			//先读取一下类型
			NBT_TAG_RAW_TYPE u8CompoundEntryTag = (NBT_TAG_RAW_TYPE)tData.GetNext();
			if (u8CompoundEntryTag == NBT_TAG::End)//处理End情况
			{
				goto end_return;//遇到end tag，正常离开
			}

			//范围验证
			if (u8CompoundEntryTag >= NBT_TAG::ENUM_END)
			{
				return Control::Error;
			}

			//验证完成，类型转换
			NBT_TAG enCompoundEntryTag = u8CompoundEntryTag;

			//访问器条目回调（仅类型）
			switch (tVisitor.VisitCompoundNextEntryType(enCompoundEntryTag))
			{
			case NBT_Visitor::NestingControl::Enter:	/*进入（什么也不做）*/	break;
			case NBT_Visitor::NestingControl::Skip:		//跳过一个
				{
					//类型已被读取
					//跳过名称
					if (!SkipName(tData))
					{
						return Control::Error;
					}

					//跳过数据
					if (!SkipSwitch(tData, enCompoundEntryTag, szStackDepth - 1))
					{
						return Control::Error;
					}
				}
				break;
			case NBT_Visitor::NestingControl::Break:	//跳过所有（离开）
				{
					//类型已被读取
					//跳过名称
					if (!SkipName(tData))
					{
						return Control::Error;
					}

					//跳过数据
					if (!SkipSwitch(tData, enCompoundEntryTag, szStackDepth - 1))
					{
						return Control::Error;
					}

					//前面跳过当前剩余未完成的部分，然后到尾部跳过所有
					goto skip_any;
				}
				break;
			case NBT_Visitor::NestingControl::Stop:	return Control::Stop;	break;
			default:								return Control::Error;	break;
			}

			//读取名称
			NBT_Type::String sName{};
			if (!GetName(tData, sName))
			{
				return Control::Error;
			}

			//调用访问器（类型名称）
			switch (tVisitor.VisitCompoundNextEntry(enCompoundEntryTag, std::move(sName)))
			{
			case NBT_Visitor::NestingControl::Enter:	/*进入（什么也不做）*/	break;
			case NBT_Visitor::NestingControl::Skip:		//跳过一个
				{
					//类型、名称已被读取
					//跳过数据
					if (!SkipSwitch(tData, enCompoundEntryTag, szStackDepth - 1))
					{
						return Control::Error;
					}
					continue;//继续上面的循环
				}
				break;
			case NBT_Visitor::NestingControl::Break://跳过剩余
				{
					//类型、名称已被读取
					//跳过数据
					if (!SkipSwitch(tData, enCompoundEntryTag, szStackDepth - 1))//先跳过当前
					{
						return Control::Error;
					}

					//前面跳过当前剩余未完成的部分，然后到尾部跳过所有
					goto skip_any;
				}
				break;
			case NBT_Visitor::NestingControl::Stop:	return Control::Stop;	break;
			default:								return Control::Error;	break;
			}

			//元素递归访问
			switch (ScanSwitch(tData, enCompoundEntryTag, tVisitor, szStackDepth - 1))
			{
			case Control::Continue:	/*继续（什么也不做）*/	break;
			case Control::Break:	/*跳过（什么也不做）*/	break;//（从内部跳过之后传出的标签不具有传递性）
			case Control::Stop:		return Control::Stop;	break;
			case Control::Error:	return Control::Error;	break;
			default:				return Control::Error;	break;//非法标签
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

			NBT_TAG_RAW_TYPE u8CompoundEntryTag = (NBT_TAG_RAW_TYPE)tData.GetNext();

			if (u8CompoundEntryTag == NBT_TAG::End)
			{
				goto end_return;//遇到end tag，正常离开
			}

			if (u8CompoundEntryTag >= NBT_TAG::ENUM_END)
			{
				return Control::Error;
			}

			NBT_TAG enCompoundEntryTag = u8CompoundEntryTag;

			if (!SkipName(tData))
			{
				return Control::Error;
			}

			if (!SkipSwitch(tData, enCompoundEntryTag, szStackDepth - 1))
			{
				return Control::Error;
			}
		}

	end_return://结束返回
		if constexpr (!bRoot)
		{
			return ResultControlToControl(tVisitor.VisitCompoundEnd());//返回
		}
		else
		{
			tVisitor.VisitEnd();//根部调用结束
			return Control::Stop;//根部直接返回
		}
	}

	template<typename InputStream>
	static bool SkipCompoundType(InputStream &tData, size_t szStackDepth)
	{
		if (szStackDepth == 0)
		{
			return false;
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
		return ScanCompoundType<true>(IptStream, tVisitor, szStackDepth) != Control::Error;
	}
	
	template<typename DataType = std::vector<uint8_t>, typename Visitor>
	requires(IsLookLike_NBT_Visitor<Visitor>)
	static bool Scan(const DataType &tDataInput, size_t szStartIdx, Visitor &tVisitor, size_t szStackDepth = 512)
	{
		NBT_IO::DefaultInputStream<DataType> IptStream(tDataInput, szStartIdx);

		return ScanCompoundType<true>(IptStream, tVisitor, szStackDepth) != Control::Error;
	}

};
//TODO: 添加使用visit输出报错

