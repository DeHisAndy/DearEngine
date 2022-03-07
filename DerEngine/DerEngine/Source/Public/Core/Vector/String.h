#pragma once
#include <atlstr.h>
#include <string>
#include <vector>
class FString
{
public:
	typedef CString Type;
	typedef const wchar_t TCHAR;
public:
	FString();
	FString(const FString& str);
	FString(const Type& str);
	FString(const char* str);
	FString(const wchar_t* str);
	FString(const std::string str);
	FString& operator = (const FString& str);
	FString& operator+=(const TCHAR* Str);
	FString& operator+=(const char* Str);
	FString& operator+=(const FString& Str);
	const FString& operator+(const FString& Str)const;
	const FString& operator+(const TCHAR* Str)const;
	FString operator+(const TCHAR* Str);
	bool operator==(FString& str);
	bool operator < (const FString& temp) const;
	friend FString operator+(const TCHAR* Lhs, const FString& Rhs)
	{
		FString temp = FString(Lhs) + Rhs;
		return temp;
	}
	friend FString operator+(const char* Lhs, const FString& Rhs)
	{
		FString temp = FString(Lhs) + Rhs;
		return temp;
	}
	bool operator==(const FString& Str);
	TCHAR* operator*() const;
	virtual ~FString();
public:
	Type& GetData();
	FString Mid(int star,int cout);
	bool Empty()const;
	int  Len() const;
	int Find(FString division,int star);
	//utf-8
	std::string GetString()const;
public:
	static FString FormatIntToString(int d);
	static FString FormatIntToString(unsigned int d);
	static FString FormatIntToString(size_t d);
	static FString FormatFloatToString(float d);
	static FString FormatFVector2DToString(class FVector2D d);
	static FString FormatFVectorToString(class FVector d);
	static FString FormatFVector4DToString(class FVector4D d);
	static void	Split( FString source, FString division, std::vector<FString>& dest);
private:
	//utf-8
	mutable Type data;
};