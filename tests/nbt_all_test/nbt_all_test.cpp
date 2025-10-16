#include <my/StrHexArray.hpp>
#include <my/MyAssert.hpp>
#include <nbt_cpp/NBT_All.hpp>

#include <stdio.h>

template<typename T>
void PrintHex(T data) noexcept
{
	constexpr uint8_t u8CountOfLine = 16;
	uint8_t u8CountCur = 0;
	
	for (size_t iAddr = 0; iAddr < data.size(); ++iAddr)
	{
		if (u8CountCur == 0)
		{
			printf("%0*zX: ", (int)sizeof(iAddr) * 2, iAddr);
		}

		printf("%02X ", (uint8_t)data[iAddr]);

		if (++u8CountCur == u8CountOfLine)
		{
			putchar('\n');
			u8CountCur = 0;
		}
	}
}


void StrHexArrayTest(void)
{
	constexpr auto h0 = StrHexArray::ToHexArr<R"(
	00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
	10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
	20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
	30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
	40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F
	50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F
	60 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F
	70 71 72 73 74 75 76 77 78 79 7A 7B 7C 7D 7E 7F
	80 81 82 83 84 85 86 87 88 89 8A 8B 8C 8D 8E 8F
	90 91 92 93 94 95 96 97 98 99 9A 9B 9C 9D 9E 9F
	A0 A1 A2 A3 A4 A5 A6 A7 A8 A9 AA AB AC AD AE AF
	B0 B1 B2 B3 B4 B5 B6 B7 B8 B9 BA BB BC BD BE BF
	C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CE CF
	D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF
	E0 E1 E2 E3 E4 E5 E6 E7 E8 E9 EA EB EC ED EE EF
	F0 F1 F2 F3 F4 F5 F6 F7 F8 F9 FA FB FC FD FE FF
)">();

	auto h1 = StrHexArray::ToHexArr(R"(
	00'01|02.03,04"05\06/07`08-09_0A;0B:0C 0D		0E0F			
	10'11|12.13,14"15\16/17`18-19_1A;1B:1C 1D		1E1F			
	20'21|22.23,24"25\26/27`28-29_2A;2B:2C 2D		2E2F			
	30'31|32.33,34"35\36/37`38-39_3A;3B:3C 3D		3E3F			
	40'41|42.43,44"45\46/47`48-49_4A;4B:4C 4D		4E4F			
	50'51|52.53,54"55\56/57`58-59_5A;5B:5C 5D		5E5F			
	60'61|62.63,64"65\66/67`68-69_6A;6B:6C 6D		6E6F			
	70'71|72.73,74"75\76/77`78-79_7A;7B:7C 7D		7E7F			
	80'81|82.83,84"85\86/87`88-89_8A;8B:8C 8D		8E8F			
	90'91|92.93,94"95\96/97`98-99_9A;9B:9C 9D		9E9F			
	A0'A1|A2.A3,A4"A5\A6/A7`A8-A9_AA;AB:AC AD		AEAF			
	B0'B1|B2.B3,B4"B5\B6/B7`B8-B9_BA;BB:BC BD		BEBF			
	C0'C1|C2.C3,C4"C5\C6/C7`C8-C9_CA;CB:CC CD		CECF			
	D0'D1|D2.D3,D4"D5\D6/D7`D8-D9_DA;DB:DC DD		DEDF			
	E0'E1|E2.E3,E4"E5\E6/E7`E8-E9_EA;EB:EC ED		EEEF			
	F0'F1|F2.F3,F4"F5\F6/F7`F8-F9_FA;FB:FC FD		FEFF			
)");

	auto h2 = StrHexArray::ToHexArr(R"(
	00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
	10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
	20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
	30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
	40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F
	50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F
	60 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F
	70 71 72 73 74 75 76 77 78 79 7A 7B 7C 7D 7E 7F
	80 81 82 83 84 85 86 87 88 89 8A 8B 8C 8D 8E 8F
	90 91 92 93 94 95 96 97 98 99 9A 9B 9C 9D 9E 9F
	A0 A1 A2 A3 A4 A5 A6 A7 A8 A9 AA AB AC AD AE AF
	B0 B1 B2 B3 B4 B5 B6 B7 B8 B9 BA BB BC BD BE BF
	C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CE CF
	D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF
	E0 E1 E2 E3 E4 E5 E6 E7 E8 E9 EA EB EC ED EE EF
	F0 F1 F2 F3 F4 F5 F6 F7 F8 F9 FA FB FC FD FE FF
)", 786);

	//PrintHex(h0);
	//printf("\n\n");
	//PrintHex(h1);
	//printf("\n\n");
	//PrintHex(h2);

	//构造目标序列
	std::vector<uint8_t> vt{};
	for (uint8_t i = 0; i < 255; ++i)
	{
		vt.push_back(i);
	}
	vt.push_back(255);

	//静态h0转换到vector进行测试
	std::vector<uint8_t> v0(h0.begin(), h0.end());

	//验证
	MyAssert(vt == v0);
	MyAssert(vt == h1);
	MyAssert(vt == h2);
}

void NBT_ReadWrite_Test(void)
{
	constexpr auto NbtRawData = StrHexArray::ToHexArr<R"(
	0A 00 00 01 00 04 62 79 74 65 80 05 00 05 66 6C
	6F 61 74 BD EA 7E FA 02 00 05 73 68 6F 72 74 7F
	FF 03 00 03 69 6E 74 80 00 00 00 04 00 04 6C 6F
	6E 67 00 00 01 0A 9F C7 00 42 08 00 06 73 74 72
	69 6E 67 00 06 E6 B5 8B E8 AF 95 06 00 06 64 6F
	75 62 6C 65 40 FB F5 23 12 5A AB 47 0A 00 08 63
	6F 6D 70 6F 75 6E 64 07 00 0A 62 79 74 65 20 61
	72 72 61 79 00 00 00 07 01 02 03 04 05 06 07 07
	00 09 69 6E 74 20 61 72 72 61 79 00 00 00 07 01
	02 03 04 05 06 07 07 00 0A 6C 6F 6E 67 20 61 72
	72 61 79 00 00 00 07 01 02 03 04 05 06 07 00 09
	00 04 6C 69 73 74 08 00 00 00 06 00 09 74 65 73
	74 20 73 74 72 30 00 09 74 65 73 74 20 73 74 72
	31 00 09 74 65 73 74 20 73 74 72 32 00 09 74 65
	73 74 20 73 74 72 33 00 09 74 65 73 74 20 73 74
	72 34 00 09 74 65 73 74 20 73 74 72 35 00
)">();

	NBT_Type::Compound cpd{};
	NBT_Reader::ReadNBT(NbtRawData, 0, cpd);

	//NBT_Writer::WriteNBT();






}

int main(void)
{
	StrHexArrayTest();

	NBT_ReadWrite_Test();






	return 0;
}