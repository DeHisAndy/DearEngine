#include "..\..\..\Core\Helper\FileHelper.h"
#include<stdio.h>
#include<io.h>
#include <windows.h>
void FFileHelper::FindFiles(const FString& path, const FString& format, TArray<FString>& file)
{
	//文件句柄  
	intptr_t   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	std::string stdStr = path.GetString();
	std::string stdFormat = format.GetString();
	std::string p;
	if ((hFile = _findfirst(p.assign(stdStr).append("\\*" + stdFormat).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
					FFileHelper::FindFiles(p.assign(stdStr).append("/").append(fileinfo.name), stdFormat, file);
				}
			}
			else
			{
				file.push_back(p.assign(stdStr).append("/")+fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}

