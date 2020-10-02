// Last Update 08.03.2019
#ifndef FieldsizesH
#define FieldsizesH
//-------------------------------------------------------------------------------------------------------------------------------------------
constexpr unsigned char FT_EMPTY = 0;
constexpr unsigned char FT_SHORTSTR = 1;
constexpr unsigned char FT_LONGSTR = 2;
constexpr unsigned char FT_MEMOSTR = 3;
constexpr unsigned char FT_LISTSTR = 4;
constexpr unsigned char FT_BOOL = 5;
constexpr unsigned char FT_CHAR = 6;
constexpr unsigned char FT_BYTE = 7;
constexpr unsigned char FT_SHORT = 8;
constexpr unsigned char FT_USHORT = 9;
constexpr unsigned char FT_LONG = 10;
constexpr unsigned char FT_ULONG = 11;
constexpr unsigned char FT_LONGLONG = 12;
constexpr unsigned char FT_FLOAT = 13;
constexpr unsigned char FT_DOUBLE = 14;
constexpr unsigned char FT_COMMA4 = 15;
constexpr unsigned char FT_COMMA4_80 = 16;
constexpr unsigned char FT_TIME = 17;
constexpr unsigned char FT_SMALLIMAGE = 18;
constexpr unsigned char FT_BIGIMAGE = 19;
constexpr unsigned char FT_DOCUMENT = 20;
constexpr unsigned char FT_AUDIO = 21;
constexpr unsigned char FT_VIDEO = 22;
constexpr unsigned char FT_DATE = 23;
constexpr unsigned char FT_BIT128 = 24;
constexpr unsigned char FT_BIT256 = 25;

constexpr unsigned char FT_SORTDOWN = 32;
constexpr unsigned char FT_SORTUP = 64;
constexpr unsigned char FT_KEY = 128;
//-------------------------------------------------------------------------------------------------------------------------------------------
constexpr unsigned char AT_NOCRYPT = 0;
constexpr unsigned char AT_CRYPT = 1;

//-------------------------------------------------------------------------------------------------------------------------------------------
constexpr unsigned char BY_NULL = 0;
constexpr unsigned char BY_BOOL = 1;
constexpr unsigned char BY_CHAR = 1;
constexpr unsigned char BY_BYTE = 1;
constexpr unsigned char BY_USHORT = 2;
constexpr unsigned char BY_SHORT = 2;
constexpr unsigned char BY_INT = 4;
constexpr unsigned char BY_UINT = 4;
constexpr unsigned char BY_LONG = 4;
constexpr unsigned char BY_ULONG = 4;
constexpr unsigned char BY_FLOAT = 4;
constexpr unsigned char BY_COMMA4 = 6;
constexpr unsigned char BY_COMMA4_80 = 10;
constexpr unsigned char BY_TIME = 8;
constexpr unsigned char BY_DOUBLE = 8;
constexpr unsigned char BY_LONGLONG = 8;
constexpr unsigned char BY_DATE = 8;
constexpr unsigned char BY_BIT128 = 16;
constexpr unsigned char BY_BIT256 = 32;
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif