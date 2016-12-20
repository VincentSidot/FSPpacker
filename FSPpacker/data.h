#pragma once
#include <Windows.h> //windows api
#include <string.h>
#include <TlHelp32.h>
struct data
{
	bool error = false;
	byte* buffer = NULL;
	__int64 size = 0;
};

const LPSTR sign = "FSPpacker";
const DWORD signlen = 10;


data getData()
{
	data ret;
	LPWSTR CurrentFilePath; //name of program
	CurrentFilePath = new WCHAR[1024];
	LARGE_INTEGER fileSize; // file size
	GetModuleFileName(0, CurrentFilePath, 1024); // getting filename
	HANDLE hFile; //file
	hFile = CreateFile(CurrentFilePath,               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template
	delete CurrentFilePath;
	if (hFile == INVALID_HANDLE_VALUE)
	{
		ret.error = true;
		CloseHandle(hFile);
		return ret;
	} // if error opening file
	if (GetFileSizeEx(hFile, &fileSize) == 0)
	{
		ret.error = true;
		CloseHandle(hFile);
		return ret;
	} // if error reading file size
	LARGE_INTEGER pos; //fix position to read info of package
	pos.QuadPart = fileSize.QuadPart - (signlen + sizeof(__int64));
	SetFilePointerEx(hFile, pos, NULL, 0); // set hFile position to pos
	DWORD bytesRead; //count how much bytes are read
	{
		LPSTR buffer;
		buffer = new char[signlen];
		if (ReadFile(hFile, buffer, signlen, &bytesRead, 0) == false)
		{
			ret.error = true;
			CloseHandle(hFile);
			return ret;
		} //error cant read
		if (bytesRead != signlen)
		{
			ret.error = true;
			CloseHandle(hFile);
			return ret;
		} // error read less than needed
		if (strcmp(buffer, sign) != 0)
		{
			ret.error = true;
			CloseHandle(hFile);
			return ret;
		}
		delete buffer;
		buffer = NULL;
	} // testing fix string
	__int64 packerSize , appSize;
	if (ReadFile(hFile, &packerSize, sizeof(__int64), &bytesRead, 0) == 0)
	{
		ret.error = true;
		CloseHandle(hFile);
		return ret;
	} // error reading value
	if (bytesRead != sizeof(packerSize))
	{
		ret.error = true;
		CloseHandle(hFile);
		return ret;
	} // error read less than needed
	appSize = fileSize.QuadPart - (packerSize + sizeof(__int64) + signlen);
	ret.size = appSize; // updating ret
	pos.QuadPart = packerSize;
	SetFilePointerEx(hFile, pos, NULL, 0);
	byte* buffer = new byte[appSize];
	if (ReadFile(hFile, buffer, appSize, &bytesRead, 0) == 0)
	{
		ret.error = true;
		CloseHandle(hFile);
		return ret;
	} // error reading value
	if (bytesRead != appSize)
	{
		ret.error = true;
		CloseHandle(hFile);
		return ret;
	} // bla bla	
	ret.buffer = buffer;
	CloseHandle(hFile);
	return ret;
}