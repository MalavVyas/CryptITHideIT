#include<iostream>
#include<fstream>
#include<iostream>
#include<string>
#include<windows.h>
#include<fstream>
#include<virtdisk.h>
#include<tchar.h>
#include<vds.h>
#include<cstring>
#include<InitGuid.h>
#include<atlbase.h>
#include<stdio.h>
#include<strsafe.h>

#define BUFSIZE 255

#ifndef UNICODE  
typedef std::string String;
#else
typedef std::wstring String;
#endif

#define newVHD 1

#pragma comment(lib, "virtdisk.lib")
#pragma comment( lib, "ole32.lib" )
#pragma comment( lib, "rpcrt4.lib" )

#define _SafeRelease(x) {if (NULL != x) { x->Release(); x = NULL; } }

static const GUID VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT = { 0xEC984AEC, 0xA0F9, 0x47e9, 0x90, 0x1F, 0x71, 0x41, 0x5A, 0x66, 0x34, 0x5B };

using namespace std;

DWORD CreateDisk(PCWSTR ISOPath, HANDLE* handle) {
	VIRTUAL_STORAGE_TYPE storageType = { VIRTUAL_STORAGE_TYPE_DEVICE_VHD, VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT };

	CREATE_VIRTUAL_DISK_PARAMETERS parameters2 = {};
	parameters2.Version = CREATE_VIRTUAL_DISK_VERSION_1;
	parameters2.Version1.MaximumSize = 1024 * 1024 * 1024;
	parameters2.Version1.BlockSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_BLOCK_SIZE;
	parameters2.Version1.SectorSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_SECTOR_SIZE;
	parameters2.Version1.SourcePath = NULL;

	int result = ::CreateVirtualDisk(&storageType, ISOPath, VIRTUAL_DISK_ACCESS_ALL, NULL, CREATE_VIRTUAL_DISK_FLAG_NONE, 0, &parameters2, NULL, handle);

	return result;
}


DWORD OpenDisk(PCWSTR ISOPATH, HANDLE* handle) {
	VIRTUAL_STORAGE_TYPE storageType = { VIRTUAL_STORAGE_TYPE_DEVICE_VHD, VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT };
	OPEN_VIRTUAL_DISK_PARAMETERS parameter = {};
	parameter.Version = OPEN_VIRTUAL_DISK_VERSION_1;
	return ::OpenVirtualDisk(&storageType, ISOPATH, VIRTUAL_DISK_ACCESS_ALL, OPEN_VIRTUAL_DISK_FLAG_NONE, &parameter, handle);
}

DWORD attachDisk(HANDLE handle) {
	ATTACH_VIRTUAL_DISK_PARAMETERS parameter = {};
	parameter.Version = ATTACH_VIRTUAL_DISK_VERSION_1;
	return ::AttachVirtualDisk(handle, NULL, ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME, 0, 0, NULL);
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

int create(std::wstring string2) {
	BOOL ok = TRUE;
	HANDLE hStdInPipeRead = NULL;
	HANDLE hStdInPipeWrite = NULL;
	HANDLE hStdOutPipeRead = NULL;
	HANDLE hStdOutPipeWrite = NULL;

	// Create two pipes.
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	ok = CreatePipe(&hStdInPipeRead, &hStdInPipeWrite, &sa, 0);
	if (ok == FALSE) return -1;
	ok = CreatePipe(&hStdOutPipeRead, &hStdOutPipeWrite, &sa, 0);
	if (ok == FALSE) return -1;

	// Create the process.
	STARTUPINFO si = { };
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdError = hStdOutPipeWrite;
	si.hStdOutput = hStdOutPipeWrite;
	si.hStdInput = hStdInPipeRead;
	PROCESS_INFORMATION pi = { };
	LPCWSTR lpApplicationName = L"C:\\Windows\\System32\\cmd.exe";
	std::wstring string1 = L"C:\\Windows\\System32\\cmd.exe /c ";
	//std::wstring string2 = L"dir";
	std::wstring string3 = string1 + string2;
	wcout << "Executing command " << string3 << endl;
	LPWSTR lpCommandLine = const_cast<LPWSTR>(string3.c_str());
	LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL;
	LPSECURITY_ATTRIBUTES lpThreadAttribute = NULL;
	BOOL bInheritHandles = TRUE;
	DWORD dwCreationFlags = 0;
	LPVOID lpEnvironment = NULL;
	LPCWSTR lpCurrentDirectory = NULL;
	ok = CreateProcess(
		lpApplicationName,
		lpCommandLine,
		lpProcessAttributes,
		lpThreadAttribute,
		bInheritHandles,
		dwCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		&si,
		&pi);
	if (ok == FALSE) return -1;

	// Close pipes we do not need.
	CloseHandle(hStdOutPipeWrite);
	CloseHandle(hStdInPipeRead);

	// The main loop for reading output from the DIR command.
	char buf[1024 + 1] = { };
	DWORD dwRead = 0;
	DWORD dwAvail = 0;
	ok = ReadFile(hStdOutPipeRead, buf, 1024, &dwRead, NULL);
	while (ok == TRUE)
	{
		buf[dwRead] = '\0';
		OutputDebugStringA(buf);
		puts(buf);
		ok = ReadFile(hStdOutPipeRead, buf, 1024, &dwRead, NULL);
	}

	// Clean up and exit.
	CloseHandle(hStdOutPipeRead);
	CloseHandle(hStdInPipeWrite);
	DWORD dwExitCode = 0;
	GetExitCodeProcess(pi.hProcess, &dwExitCode);
	return dwExitCode;
}

int main() {

	/*const wchar_t* stri = L"c:\\file.vhd";
	LPTSTR ISOPath = const_cast<wchar_t *>(stri); */

	PCWSTR ISOPath = _T("c:\\Users\\malav\\desktop\\file12.vhd");
	HANDLE handle;
	DWORD result;
	ULONG bytesUsed;
	bool isoCreated = false;
	string driveStr;
	bool letterSelected = false;
	cout << "Testing if drive exists or not " << endl;
	while (!letterSelected) {
		cout << "Enter the drive you want to test" << endl;
		cin >> driveStr;

		std::wstring stemp = s2ws(driveStr);
		LPCWSTR driveLetter = stemp.c_str();

		UINT result = ::GetDriveType(driveLetter);
		if (result != 1) {
			cout << "Enter a different letter " << endl;
		}
		else {
			cout << "Driver letter " << (char*)driveLetter << " Selected" << endl;
			letterSelected = true;
		}
	}

	result = CreateDisk(ISOPath, &handle);
//	CloseHandle(handle);
	if (result == ERROR_FILE_EXISTS) {
		result = OpenDisk(ISOPath, &handle);
		if (result != ERROR_SUCCESS) {
			cout << "Can't Open" << endl;
		}
		else {
			cout << "result is" << endl;
			cout << result << endl;
		}
	}
	else if (result != ERROR_SUCCESS) {
		cout << "Unable to create" << endl;
		cout << "result is " << result << endl;
		cout << "error is " << GetLastError() << endl;

	}
	else {
		isoCreated = true;
		cout << "Creation done";
		//result = OpenDisk(ISOPath, &handle);
	}
	result = attachDisk(handle);
	if (result == ERROR_SUCCESS && isoCreated == true) {
		int disk;
		std::wstring command = TEXT("diskpart.exe /s C:\\Users\\Malav\\Desktop\\part.scr");
		cout << create(command);
		cout << endl << "Select the Disk number" << endl;
		cout << "e.g. 1" << endl;
		cin >> disk;
		cout << "Formatting Disk" << endl;


		HANDLE hFile = INVALID_HANDLE_VALUE;
		HANDLE hTempFile = INVALID_HANDLE_VALUE;

		BOOL fSuccess = FALSE;
		DWORD dwRetVal = 0;
		UINT uRetVal = 0;

		DWORD dwBytesRead = 0;
		//DWORD uRetVal = 0;

		TCHAR szTempFileName[MAX_PATH];
		TCHAR lpTempPathBuffer[MAX_PATH];
		char chBuffer[BUFSIZE];
		LPCWSTR filename;
		LPCTSTR errMsg;
		LPTSTR next_token;
		filename = L"bleh-";
		DWORD numBytesWritten;
		dwRetVal = GetTempPath(MAX_PATH, lpTempPathBuffer);

		string write = "SELECT DISK=" + to_string(disk) + '\n' + "CREATE PARTITION PRIMARY" + '\n' + "ASSIGN LETTER=" + driveStr + '\n' + "FORMAT FS=FAT32 LABEL=""FD"" QUICK" + '\n' + "EXIT" + '\n';
		wstring diskpart = L"diskpart.exe /s ";
		if (dwRetVal > MAX_PATH || (dwRetVal == 0)) {
			cout << "Get Temp Path failed" << endl;
			cout << dwRetVal << endl;
			system("pause");
			return 7;
		}
		else {
			uRetVal = GetTempFileName(lpTempPathBuffer, TEXT("bleh"), 0, szTempFileName);
			wchar_t* token = _tcstok(szTempFileName, (const wchar_t*)".");
			_tcscat(szTempFileName, L".srt");
			wcout << szTempFileName;
			hTempFile = CreateFile((LPCWSTR)szTempFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			WriteFile(hTempFile, write.c_str(), strlen(write.c_str()) + 1, &numBytesWritten, NULL);
			//WriteFile(hTempFile, (char)disk, strlen("select disk"), &numBytesWritten, NULL);
			wcout << "Written data to file " << szTempFileName << endl;
			cout << "Number of bytes written were" << numBytesWritten;
			wstring cmd = (wstring)diskpart + (wstring)szTempFileName;
			wcout << "command" << cmd << endl;
			CloseHandle(hTempFile);
			create(cmd);
		}

		cout << "Attach result " << result << endl;
	}
	cout << "Do you want to detach y or n?" << endl;
	char choice;
	cin >> choice;
	if (choice != 'y' && choice != 'n') {
		cout << "enter y or n only" << endl;
	}
	else if (choice == 'y') {
		cout << "Detaching" << endl;
		DetachVirtualDisk(handle, DETACH_VIRTUAL_DISK_FLAG_NONE, 0);
	}
	else if (choice == 'n') {
		cout << "ok" << endl;
	}
	cout << "tata" << endl;
	system("pause");
}