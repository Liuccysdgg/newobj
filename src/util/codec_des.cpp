#include "codec.h"
#include <string>
#include "public/exception.h"


#include <string>

class yxyDES2
{
public:
	//类构造函数
	yxyDES2();

	//类析构函数
	~yxyDES2();

	//功能:产生16个28位的key
	//参数:源8位的字符串(key),存放key的序号0-1
	//结果:函数将调用private CreateSubKey将结果存于char SubKeys[keyN][16][48]
	void InitializeKey(char* srcBytes, unsigned int keyN);

	//功能:加密8位字符串
	//参数:8位字符串,使用Key的序号0-1
	//结果:函数将加密后结果存放于private szCiphertext[16]
	//      用户通过属性Ciphertext得到
	void EncryptData(char* _srcBytes, unsigned int keyN);

	//功能:解密16位十六进制字符串
	//参数:16位十六进制字符串,使用Key的序号0-1
	//结果:函数将解密候结果存放于private szPlaintext[8]
	//      用户通过属性Plaintext得到
	void DecryptData(char* _srcBytes, unsigned int keyN);

	//功能:加密任意长度字符串
	//参数:任意长度字符串,长度,使用Key的序号0-1
	//结果:函数将加密后结果存放于private szFCiphertextAnyLength[8192]
	//      用户通过属性CiphertextAnyLength得到
	void EncryptAnyLength(char* _srcBytes, unsigned int _bytesLength, unsigned int keyN);

	//功能:解密任意长度十六进制字符串
	//参数:任意长度字符串,长度,使用Key的序号0-1
	//结果:函数将加密后结果存放于private szFPlaintextAnyLength[8192]
	//      用户通过属性PlaintextAnyLength得到
	void DecryptAnyLength(char* _srcBytes, unsigned int _bytesLength, unsigned int keyN);

	//功能:Bytes到Bits的转换,
	//参数:待变换字符串,处理后结果存放缓冲区指针,Bits缓冲区大小
	void Bytes2Bits(char* srcBytes, char* dstBits, unsigned int sizeBits);

	//功能:Bits到Bytes的转换,
	//参数:待变换字符串,处理后结果存放缓冲区指针,Bits缓冲区大小
	void Bits2Bytes(char* dstBytes, char* srcBits, unsigned int sizeBits);

	//功能:Int到Bits的转换,
	//参数:待变换字符串,处理后结果存放缓冲区指针
	void Int2Bits(unsigned int srcByte, char* dstBits);

	//功能:Bits到Hex的转换
	//参数:待变换字符串,处理后结果存放缓冲区指针,Bits缓冲区大小
	void Bits2Hex(char* dstHex, char* srcBits, unsigned int sizeBits);

	//功能:Bits到Hex的转换
	//参数:待变换字符串,处理后结果存放缓冲区指针,Bits缓冲区大小
	void Hex2Bits(char* srcHex, char* dstBits, unsigned int sizeBits);

	//szCiphertextInBinary的get函数
	char* GetCiphertextInBinary();

	//szCiphertextInHex的get函数
	char* GetCiphertextInHex();

	//Ciphertext的get函数
	char* GetCiphertextInBytes();

	//Plaintext的get函数
	char* GetPlaintext();

	//CiphertextAnyLength的get函数
	newobj::buffer GetCiphertextAnyLength();

	//PlaintextAnyLength的get函数
	newobj::buffer GetPlaintextAnyLength();

	int GetLength();
private:
	char szSubKeys[2][16][48];//储存2个16组48位密钥,第2个用于3DES
	char szCiphertextRaw[64]; //储存二进制密文(64个Bits) int 0,1
	char szPlaintextRaw[64]; //储存二进制密文(64个Bits) int 0,1
	char szCiphertextInBytes[8];//储存8位密文
	char szPlaintextInBytes[8];//储存8位明文字符串

	char szCiphertextInBinary[65]; //储存二进制密文(64个Bits) char '0','1',最后一位存'\0'
	char szCiphertextInHex[17]; //储存十六进制密文,最后一位存'\0'
	char szPlaintext[9];//储存8位明文字符串,最后一位存'\0'

	newobj::buffer szFCiphertextAnyLength;//任意长度密文
	newobj::buffer szFPlaintextAnyLength;//任意长度明文字符串

	int m_len;
	//功能:生成子密钥
	//参数:经过PC1变换的56位二进制字符串,生成的szSubKeys编号0-1
	//结果:将保存于char szSubKeys[16][48]
	void CreateSubKey(char* sz_56key, unsigned int keyN);

	//功能:DES中的F函数,
	//参数:左32位,右32位,key序号(0-15),使用的szSubKeys编号0-1
	//结果:均在变换左右32位
	void FunctionF(char* sz_Li, char* sz_Ri, unsigned int iKey, unsigned int keyN);

	//功能:IP变换
	//参数:待处理字符串,处理后结果存放指针
	//结果:函数改变第二个参数的内容
	void InitialPermuteData(char* _src, char* _dst);

	//功能:将右32位进行扩展位48位,
	//参数:原32位字符串,扩展后结果存放指针
	//结果:函数改变第二个参数的内容
	void ExpansionR(char* _src, char* _dst);

	//功能:异或函数,
	//参数:待异或的操作字符串1,字符串2,操作数长度,处理后结果存放指针
	//结果: 函数改变第四个参数的内容
	void XOR(char* szParam1, char* szParam2, unsigned int uiParamLength, char* szReturnValueBuffer);

	//功能:S-BOX , 数据压缩,
	//参数:48位二进制字符串,
	//结果:返回结果:32位字符串
	void CompressFuncS(char* _src48, char* _dst32);

	//功能:IP逆变换,
	//参数:待变换字符串,处理后结果存放指针
	//结果:函数改变第二个参数的内容
	void PermutationP(char* _src, char* _dst);

};

// permuted choice table (PC1)
const static char PC1_Table[56] = {
	57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};
// permuted choice key (PC2)
const static char PC2_Table[48] = {
	14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};
// number left rotations of pc1 
const static char Shift_Table[16] = {
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
// initial permutation (IP)
const static char IP_Table[64] = {
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};
// expansion operation matrix (E)
const static char E_Table[48] = {
	32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
};
// The (in)famous S-boxes 
const static char S_Box[8][4][16] = {
	// S1
	14,  4,	13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
	// S2 
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
	// S3 
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
	// S4 
	7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
	// S5 
	2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
	// S6 
	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
	// S7 
	4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
	// S8 
	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};
// 32-bit permutation function P used on the output of the S-boxes 
const static char P_Table[32] = {
	16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
	2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25
};
// final permutation IP^-1 
const static char IPR_Table[64] = {
	40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25
};
int yxyDES2::GetLength()
{
	return m_len;
}
yxyDES2::yxyDES2()
{
	m_len = 0;
	memset(szCiphertextRaw, 0, 64);
	memset(szPlaintextRaw, 0, 64);
	memset(szCiphertextInBytes, 0, 8);
	memset(szPlaintextInBytes, 0, 8);
	memset(szCiphertextInBinary, 0, 65);
	memset(szCiphertextInHex, 0, 17);
	memset(szPlaintext, 0, 9);
	//memset(szFCiphertextAnyLength, 0, 8192);
	//memset(szCiphertextInHex, 0, 8192);
}
yxyDES2::~yxyDES2()
{
}

void yxyDES2::InitializeKey(char* srcBytes, unsigned int keyN)
{
	//convert 8 char-bytes key to 64 binary-bits
	char sz_64key[64] = { 0 };
	Bytes2Bits(srcBytes, sz_64key, 64);
	//PC 1
	char sz_56key[56] = { 0 };
	for (int k = 0; k < 56; k++)
	{
		sz_56key[k] = sz_64key[PC1_Table[k] - 1];
	}
	CreateSubKey(sz_56key, keyN);
}

void yxyDES2::CreateSubKey(char* sz_56key, unsigned int keyN)
{
	char szTmpL[28] = { 0 };
	char szTmpR[28] = { 0 };
	char szCi[28] = { 0 };
	char szDi[28] = { 0 };
	memcpy(szTmpL, sz_56key, 28);
	memcpy(szTmpR, sz_56key + 28, 28);

	for (int i = 0; i < 16; i++)
	{
		//shift to left
		//Left 28 bits
		memcpy(szCi, szTmpL + Shift_Table[i], 28 - Shift_Table[i]);
		memcpy(szCi + 28 - Shift_Table[i], szTmpL, Shift_Table[i]);
		//Right 28 bits
		memcpy(szDi, szTmpR + Shift_Table[i], 28 - Shift_Table[i]);
		memcpy(szDi + 28 - Shift_Table[i], szTmpR, Shift_Table[i]);

		//permuted choice 48 bits key
		char szTmp56[56] = { 0 };
		memcpy(szTmp56, szCi, 28);
		memcpy(szTmp56 + 28, szDi, 28);
		for (int j = 0; j < 48; j++)
		{
			szSubKeys[keyN][i][j] = szTmp56[PC2_Table[j] - 1];
		}
		//Evaluate new szTmpL and szTmpR
		memcpy(szTmpL, szCi, 28);
		memcpy(szTmpR, szDi, 28);
	}
}

void yxyDES2::EncryptData(char* _srcBytes, unsigned int keyN)
{
	char szSrcBits[64] = { 0 };
	char sz_IP[64] = { 0 };
	char sz_Li[32] = { 0 };
	char sz_Ri[32] = { 0 };
	char sz_Final64[64] = { 0 };

	Bytes2Bits(_srcBytes, szSrcBits, 64);
	//IP
	InitialPermuteData(szSrcBits, sz_IP);
	memcpy(sz_Li, sz_IP, 32);
	memcpy(sz_Ri, sz_IP + 32, 32);

	for (int i = 0; i < 16; i++)
	{
		FunctionF(sz_Li, sz_Ri, i, keyN);
	}
	//so D=LR

	memcpy(sz_Final64, sz_Ri, 32);
	memcpy(sz_Final64 + 32, sz_Li, 32);

	//~IP
	for (int j = 0; j < 64; j++)
	{
		szCiphertextRaw[j] = sz_Final64[IPR_Table[j] - 1];
	}
	Bits2Bytes(szCiphertextInBytes, szCiphertextRaw, 64);
}

void yxyDES2::DecryptData(char* _srcBytes, unsigned int keyN)
{
	char szSrcBits[64] = { 0 };
	char sz_IP[64] = { 0 };
	char sz_Li[32] = { 0 };
	char sz_Ri[32] = { 0 };
	char sz_Final64[64] = { 0 };
	Bytes2Bits(_srcBytes, szSrcBits, 64);
	//IP --- return is sz_IP
	InitialPermuteData(szSrcBits, sz_IP);
	//divide the 64 bits data to two parts
	memcpy(sz_Ri, sz_IP, 32); //exchange L to R
	memcpy(sz_Li, sz_IP + 32, 32);  //exchange R to L
	//16 rounds F and xor and exchange
	for (int i = 0; i < 16; i++)
	{
		FunctionF(sz_Ri, sz_Li, 15 - i, keyN);
	}
	memcpy(sz_Final64, sz_Li, 32);
	memcpy(sz_Final64 + 32, sz_Ri, 32);
	// ~IP
	for (int j = 0; j < 64; j++)
	{
		szPlaintextRaw[j] = sz_Final64[IPR_Table[j] - 1];
	}
	Bits2Bytes(szPlaintextInBytes, szPlaintextRaw, 64);
}

void yxyDES2::FunctionF(char* sz_Li, char* sz_Ri, unsigned int iKey, unsigned int keyN)
{
	char sz_48R[48] = { 0 };
	char sz_xor48[48] = { 0 };
	char sz_P32[32] = { 0 };
	char sz_Rii[32] = { 0 };
	char sz_Key[48] = { 0 };
	char s_Compress32[32] = { 0 };
	memcpy(sz_Key, szSubKeys[keyN][iKey], 48);
	ExpansionR(sz_Ri, sz_48R);
	XOR(sz_48R, sz_Key, 48, sz_xor48);

	CompressFuncS(sz_xor48, s_Compress32);
	PermutationP(s_Compress32, sz_P32);
	XOR(sz_P32, sz_Li, 32, sz_Rii);
	memcpy(sz_Li, sz_Ri, 32);
	memcpy(sz_Ri, sz_Rii, 32);
}

void yxyDES2::InitialPermuteData(char* _src, char* _dst)
{
	//IP
	for (int i = 0; i < 64; i++)
	{
		_dst[i] = _src[IP_Table[i] - 1];
	}
}

void yxyDES2::ExpansionR(char* _src, char* _dst)
{
	for (int i = 0; i < 48; i++)
	{
		_dst[i] = _src[E_Table[i] - 1];
	}
}

void yxyDES2::XOR(char* szParam1, char* szParam2, unsigned int uiParamLength, char* szReturnValueBuffer)
{
	for (unsigned int i = 0; i < uiParamLength; i++)
	{
		szReturnValueBuffer[i] = szParam1[i] ^ szParam2[i];
	}
}

void yxyDES2::CompressFuncS(char* _src48, char* _dst32)
{
	char bTemp[8][6] = { 0 };
	char dstBits[4] = { 0 };
	for (int i = 0; i < 8; i++)
	{
		memcpy(bTemp[i], _src48 + i * 6, 6);
		int iX = (bTemp[i][0]) * 2 + (bTemp[i][5]);
		int iY = 0;
		for (int j = 1; j < 5; j++)
		{
			iY += bTemp[i][j] << (4 - j);
		}
		Int2Bits(S_Box[i][iX][iY], dstBits);
		memcpy(_dst32 + i * 4, dstBits, 4);
	}

}

void yxyDES2::PermutationP(char* _src, char* _dst)
{
	for (int i = 0; i < 32; i++)
	{
		_dst[i] = _src[P_Table[i] - 1];
	}
}

void yxyDES2::Bytes2Bits(char* srcBytes, char* dstBits, unsigned int sizeBits)
{
	for (unsigned int i = 0; i < sizeBits; i++)
		dstBits[i] = ((srcBytes[i >> 3] << (i & 7)) & 128) >> 7;
}

void yxyDES2::Bits2Bytes(char* dstBytes, char* srcBits, unsigned int sizeBits)
{
	memset(dstBytes, 0, sizeBits >> 3);
	for (unsigned int i = 0; i < sizeBits; i++)
		dstBytes[i >> 3] |= (srcBits[i] << (7 - (i & 7)));
}

void yxyDES2::Int2Bits(unsigned int _src, char* dstBits)
{
	for (unsigned int i = 0; i < 4; i++)
		dstBits[i] = ((_src << i) & 8) >> 3;
}

void yxyDES2::Bits2Hex(char* dstHex, char* srcBits, unsigned int sizeBits)
{
	memset(dstHex, 0, sizeBits >> 2);
	for (unsigned int i = 0; i < sizeBits; i++) //convert to int 0-15
		dstHex[i >> 2] += (srcBits[i] << (3 - (i & 3)));
	for (unsigned int j = 0; j < (sizeBits >> 2); j++)
		dstHex[j] += dstHex[j] > 9 ? 55 : 48; //convert to char '0'-'F'
}

void yxyDES2::Hex2Bits(char* srcHex, char* dstBits, unsigned int sizeBits)
{
	memset(dstBits, 0, sizeBits);
	for (unsigned int i = 0; i < (sizeBits >> 2); i++)
		srcHex[i] -= srcHex[i] > 64 ? 55 : 48; //convert to char int 0-15
	for (unsigned int j = 0; j < sizeBits; j++)
		dstBits[j] = ((srcHex[j >> 2] << (j & 3)) & 15) >> 3;

}

char* yxyDES2::GetCiphertextInBinary()
{
	for (unsigned int i = 0; i < 64; i++)
	{
		szCiphertextInBinary[i] = szCiphertextRaw[i] + 48; // from int(0) to char('0') and int1 to char('1')
	}
	szCiphertextInBinary[64] = '\0';
	return szCiphertextInBinary;
}

char* yxyDES2::GetCiphertextInHex()
{
	Bits2Hex(szCiphertextInHex, szCiphertextRaw, 64);
	szCiphertextInHex[16] = '\0';
	return szCiphertextInHex;
}

char* yxyDES2::GetCiphertextInBytes()
{
	return szCiphertextInBytes;
}

char* yxyDES2::GetPlaintext()
{
	memcpy(szPlaintext, szPlaintextInBytes, 8);
	szPlaintext[8] = '\0';
	return szPlaintext;
}

newobj::buffer yxyDES2::GetCiphertextAnyLength()
{
	return szFCiphertextAnyLength;
}

newobj::buffer yxyDES2::GetPlaintextAnyLength()
{
	return szFPlaintextAnyLength;
}

void yxyDES2::EncryptAnyLength(char* _srcBytes, unsigned int _bytesLength, unsigned int keyN)
{
	//	memset(szFCiphertextAnyLength, 0, 8192);
	szFCiphertextAnyLength.resize((_bytesLength % 8 == 0 ? _bytesLength : _bytesLength + _bytesLength % 8) + 16);
	if (_bytesLength == 8)
	{
		EncryptData(_srcBytes, keyN);
		memcpy((void*)szFCiphertextAnyLength.data(), szCiphertextInBytes, 8);
		szFCiphertextAnyLength[8] = '\0';
		m_len = 8;
	}
	else if (_bytesLength < 8)
	{
		char _temp8bytes[8] = { 0 };
		memcpy(_temp8bytes, _srcBytes, _bytesLength);
		EncryptData(_temp8bytes, keyN);
		memcpy((void*)szFCiphertextAnyLength.data(), szCiphertextInBytes, 8);
		szFCiphertextAnyLength[8] = '\0';
		m_len = 8;
	}
	else if (_bytesLength > 8)
	{
		int iParts = _bytesLength >> 3;
		int iResidue = _bytesLength % 8;
		char szLast8Bits[8] = { 0 };
		for (int i = 0; i < iParts; i++)
		{
			memcpy(szLast8Bits, _srcBytes + (i << 3), 8);
			EncryptData(szLast8Bits, keyN);
			memcpy((char*)szFCiphertextAnyLength.data() + (i << 3), szCiphertextInBytes, 8);
		}
		memset(szLast8Bits, 0, 8);
		memcpy(szLast8Bits, _srcBytes + (iParts << 3), iResidue);

		EncryptData(szLast8Bits, keyN);
		memcpy((char*)szFCiphertextAnyLength.data() + (iParts << 3), szCiphertextInBytes, 8);
		szFCiphertextAnyLength[((iParts + 1) << 3)] = '\0';

		m_len = ((iParts + 1) << 3);
	}
}
 
void yxyDES2::DecryptAnyLength(char* _srcBytes, unsigned int _bytesLength, unsigned int keyN)
{
	szFPlaintextAnyLength.resize((_bytesLength % 8 == 0 ? _bytesLength : _bytesLength + _bytesLength % 8) + 16);
	if (_bytesLength == 8)
	{
		DecryptData(_srcBytes, keyN);
		memcpy((char*)szFPlaintextAnyLength.data(), szPlaintextInBytes, 8);

		szFPlaintextAnyLength[8] = '\0';
		m_len = 8;
	}
	else if (_bytesLength < 8)
	{
		char _temp8bytes[8] = { 0 };
		memcpy(_temp8bytes, _srcBytes, 8);
		DecryptData(_temp8bytes, keyN);
		memcpy((char*)szFPlaintextAnyLength.data(), szPlaintextInBytes, _bytesLength);
		szFPlaintextAnyLength[_bytesLength] = '\0';
		m_len = 8;
	}
	else if (_bytesLength > 8)
	{
		int iParts = _bytesLength >> 3;
		int iResidue = _bytesLength % 8;
		char szLast8Bits[8] = { 0 };
		for (int i = 0; i < iParts; i++)
		{
			memcpy(szLast8Bits, _srcBytes + (i << 3), 8);
			DecryptData(szLast8Bits, keyN);
			memcpy((char*)szFPlaintextAnyLength.data() + (i << 3), szPlaintextInBytes, 8);
		}
		if (iResidue != 0)
		{
			memset(szLast8Bits, 0, 8);
			memcpy(szLast8Bits, _srcBytes + (iParts << 3), 8);
			DecryptData(szLast8Bits, keyN);
			memcpy((char*)szFPlaintextAnyLength.data() + (iParts << 3), szPlaintextInBytes, iResidue);
		}
		szFPlaintextAnyLength[_bytesLength] = '\0';

		m_len = _bytesLength;
	}
}


buffer  newobj::codec::des::en(const buffer& src, const nstring& key_param)
{
	if (src.length() == 0)
		return newobj::buffer();
	if (key_param.length() > 1024)
	{
		trw_str("the key is too long");
	}
	buffer ret;
	char key[1024];
	memset(key, 0, 1024);
	memcpy(key, key_param.c_str(), key_param.length());
	yxyDES2 des;
	des.InitializeKey((char*)key, 0);
	des.EncryptAnyLength((char*)src.data(), (uint32)src.length(), 0);

	return des.GetCiphertextAnyLength();
}

buffer  newobj::codec::des::de(const buffer& src, const nstring& key_param)
{
	if (src.length() == 0)
		return newobj::buffer();
	if (key_param.length() > 1024)
	{
		trw_str("the key is too long");
	}
	buffer ret;
	//const char * key = VMProtectDecryptStringA("2DA517EB346C");
	char key[1024];
	memset(key, 0, 1024);
	memcpy(key, key_param.c_str(), key_param.length());
	yxyDES2 des;
	des.InitializeKey((char*)key, 0);
	des.DecryptAnyLength((char*)src.data(), (uint32)src.length(), 0);
	return des.GetPlaintextAnyLength();
}
