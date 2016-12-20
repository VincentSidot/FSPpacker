#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <Windows.h>
#include <Strsafe.h>
#include <AtlConv.h>
#include <string.h>
#include "data.h"

#define ERROR_ALREADY_PACKAGE 0
#define ERROR_OPENING_FILE 1
#define ERROR_READING_FILE 2
#define ERROR_GETTING_FILESIZE 3
#define SUCCESS 4


const byte* packer = rawData;
const __int64 packerSize = sizeof(rawData);
const LPSTR sign = "FSPpacker";
const DWORD signlen = 10;

void backup(LPCWSTR file, void* buffer, DWORD size);
void fixPath(WCHAR* file);
bool test(HANDLE hFile);

WORD convertToPacker(LPCWSTR file,bool makeBackup = false)
{
	WCHAR *path;
	path = new WCHAR[2048];
	StringCbCopy(path, 2048, file);
	fixPath(path);
	LARGE_INTEGER fileSize;
	HANDLE hFile = CreateFile(path, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return ERROR_OPENING_FILE;
	}
	if (!test(hFile))
	{
		CloseHandle(hFile);
		return ERROR_ALREADY_PACKAGE;
	}
	if (GetFileSizeEx(hFile, &fileSize) == 0)
	{
		CloseHandle(hFile);
		return ERROR_GETTING_FILESIZE;
	}
	byte* buffer = new byte[fileSize.LowPart];
	DWORD bytesRead;
	if (ReadFile(hFile, buffer, fileSize.LowPart, &bytesRead, NULL) == 0)
	{
		CloseHandle(hFile);
		return ERROR_READING_FILE;
	}
	if (fileSize.QuadPart != bytesRead)
	{
		CloseHandle(hFile);
		return ERROR_READING_FILE;
	}
	if (makeBackup)
	{
		backup(path, buffer, fileSize.LowPart);
	}
	SetFilePointer(hFile, 0, NULL, 0);
	WriteFile(hFile, packer, packerSize, NULL, NULL);
	WriteFile(hFile, buffer, fileSize.LowPart, NULL, NULL);
	WriteFile(hFile, sign, signlen, NULL, NULL);
	WriteFile(hFile, &packerSize, sizeof(__int64), NULL, NULL);
	return SUCCESS;
}

void backup(LPCWSTR file, void* buffer, DWORD size)
{
	USES_CONVERSION;
	LPWSTR fileName = new WCHAR[1024];
	StringCbCopy(fileName,1024, file);
	StringCbCat(fileName,1024, L".bak");
	HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return;
	}
	DWORD bytesWrite;
	WriteFile(hFile, buffer, size, &bytesWrite, NULL);
	return;
}
bool test(HANDLE hFile)
{
	LARGE_INTEGER size;
	if (GetFileSizeEx(hFile, &size) == 0)
		return false;
	LARGE_INTEGER pos;
	pos.QuadPart = size.QuadPart - (signlen + sizeof(__int64));
	SetFilePointerEx(hFile, pos, NULL, 0);
	char buffer[signlen];
	ReadFile(hFile, buffer, signlen, NULL, NULL);
	if (strcmp(sign, buffer) == 0)
		return false;
	SetFilePointer(hFile, 0, NULL, 0);
	return true;
}
void fixPath(WCHAR* file)
{
	DWORD size = wcslen(file);
	if (file[size - 3] == '.' && file[size - 2] == 'e' && file[size - 1] == 'x' && file[size] == 'e')
		return;
	StringCbCat(file, 2048, L".exe");
	return;
}