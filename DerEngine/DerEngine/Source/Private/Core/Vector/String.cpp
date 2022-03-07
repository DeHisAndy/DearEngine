#include "../../../Public/Core/Vector/String.h"
#include <atlsimpstr.h>
#include "../../../Public/Core/Vector/Vector2D.h"
#include "../../../Public/Core/Vector/Vector.h"
#include "../../../Public/Core/Vector/Vector4D.h"
#include "../../../Public/Core/Vector/Rotator.h"

FString::FString()
{

}

FString::FString(const std::string str)
{
	data = str.c_str();
}

FString::FString(const Type& str)
{
	data = str;
}

FString::~FString()
{

}


FString::Type& FString::GetData()
{
	return data;
}

FString FString::Mid(int star, int cout)
{
	return FString(data.Mid(star, cout));
}

FString::FString(const FString& str)
{
	data = str.data;
}

FString::FString(const char* str)
{
	data= str;
}

FString::FString(const wchar_t* str)
{
	data = str;
}

FString& FString::operator=(const FString& str)
{
	data = str.data;
	return *this;
}

bool FString::operator<(const FString& temp) const
{
	return  Len() < temp.Len();	
}

FString& FString::operator+=(const FString& Str)
{
	data = data+Str.data;
	return *this;
}

FString& FString::operator+=(const TCHAR* Str)
{
	this->data = this->data + Str;
	return *this;
}

FString& FString::operator+=(const char* Str)
{
	this->data = this->data + Str;
	return *this;
}

const FString& FString::operator+(const FString& Str)const
{

	this->data = this->data + Str.data;
	return	*this;
}

const FString& FString::operator+(const TCHAR* Str)const
{
	this->data = this->data + Str;
	return	*this;
}

FString FString::operator+(const TCHAR* Str)
{
	this->data = this->data + Str;
	return	*this;
}


bool FString::operator==(FString& temp)
{
	if (temp.GetData()= GetData())
	{
		return true;
	}
	return false;
}
bool FString::operator==(const FString& Str)
{
	return	data == Str.data;
}

FString::TCHAR* FString::operator*() const
{
	return data.GetString();
}

bool FString::Empty() const
{
	if (data.GetLength()>0)
	{
		return false;
	}
	return true;
}

int FString::Len()const
{
	return data.GetLength();
}

int FString::Find(FString division, int star)
{
	return data.Find(division.GetData(), star);
}

std::string FString::GetString()const
{
	CT2A ascii(data, CP_UTF8);
	std::string utf_8String(ascii.m_psz);
	return utf_8String;
}


FString FString::FormatIntToString(int d)
{
	return FString(std::to_string(d).c_str());
}

FString FString::FormatIntToString(size_t d)
{
	return FString(std::to_string(d).c_str());
}

FString FString::FormatIntToString(unsigned int d)
{
	return FString(std::to_string(d).c_str());
}

FString FString::FormatFloatToString(float d)
{
	return FString(std::to_string(d).c_str());
}

FString FString::FormatFVector2DToString(FVector2D d)
{
	return FString("( ") + FString::FormatFloatToString(d.X) + FString(" , ") + FString::FormatFloatToString(d.Y) + FString(" )");
}

FString FString::FormatFVectorToString( FVector d)
{
	return FString("( ") + FString::FormatFloatToString(d.X) + FString(" , ") + FString::FormatFloatToString(d.Y) + FString(" , ") + FString::FormatFloatToString(d.Z) + FString(" )");
}

FString FString::FormatFVector4DToString( FVector4D d)
{
	return FString("( ") + FString::FormatFloatToString(d.X) + FString(" , ") + FString::FormatFloatToString(d.X) + FString(" , ") + FString::FormatFloatToString(d.X) + FString::FormatFloatToString(d.Z) + FString(" )");
}

void FString::Split(FString source, FString division, std::vector<FString>& dest)
{
	dest.clear();
	int pos = 0;
	int pre_pos = 0;
	while (-1 != pos) {
		pre_pos = pos;
		pos = source.Find(division, (pos + 1));
		if (pos==-1&& source.Len()> pre_pos +2)
		{
			dest.push_back(source.Mid(pre_pos+1, source.Len()));
		}
		else
		{
			dest.push_back(source.Mid(pre_pos, (pos - pre_pos)));
		}
	}
	
}


