#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <stdio.h>
#include <fstream>
#include "crc.h"
using namespace std;

#pragma comment(lib, "PSAPI.LIB")
#pragma  comment(linker,  "/subsystem:\"windows\"  /entry:\"wmainCRTStartup\"")

char* dos2com(LPCTSTR lpszDosPath);
bool right();
void kill();

int crc32_kill[10];

fstream fd;
//#define load

int wmain()
{
#ifndef load
	fstream f;
	f.open("list.txt");
	char line[20];
	for(int i = 0;i<10;i++)
	{
		if(f.eof())
			break;
		f.getline(line,20);
		crc32_kill[i] = atoi(line);
	}
	f.close();


	
	fd.open("debug_list.txt",ios::out|ios::app);
	fd << "crc_list:" << endl;
	for(int i = 0;i<10;i++)
		fd << crc32_kill << endl;

	fd << "getRight:" << right() << endl;
	while(1)
	{
#endif
		kill();

#ifndef load
		for(int i=0;i<10;i++)
			Sleep(2000);
	}
#endif
	fd.close();
	return 0;
}

void kill()
{
	HANDLE shot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(shot,&pe);
	char ch[] = "notepad.exe";
	char path[MAX_PATH] = "";

	do
	{
		HANDLE process = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE,FALSE,pe.th32ProcessID);
		if(process)
		{
			GetProcessImageFileNameA(process,path,260);

			fstream fin;
			char *fp = dos2com(path);
			fin.open(fp, ios::binary | ios::in);
			delete[] fp;

			int crc32;
			if(fin)
			{
				fin.seekg(0,ios::end);
				long len = fin.tellg();
				fin.seekg(0,ios::beg);
				char *file = new char[len];
				fin.read(file,len);
				fin.close();

				crc32 = GetCRC32((unsigned char*)file,len);
#ifndef load
				delete[] file;
				bool find = false;
				for(int i=0;i<10;i++)
				{
					if(crc32==crc32_kill[i])
					{
						TerminateProcess(process,0);
						MessageBox(0,"ÄÚ´æ´íÎó",0,MB_ICONERROR|MB_SYSTEMMODAL);
						break;
					}
				}

#endif

#ifdef load
				fstream fo;
				fo.open("crc32list.txt",ios::out|ios::app);
				strcat(path,":");
				char tmp[50];
				itoa(crc32,tmp,10);
				strcat(path,tmp);
				fo << path << endl;
#endif
			}
		}
	}while(Process32Next(shot,&pe));
}

bool right()
{
	DWORD dwVersion = GetVersion();
	if(dwVersion < 0x80000000)
	{
		HANDLE hToken;
		TOKEN_PRIVILEGES tkp;
		OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken);
		LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);
		return (GetLastError()==ERROR_SUCCESS);
	}
	return true;
}

char* dos2com(LPCTSTR lpszDosPath)
{
	char *strResult = new char[MAX_PATH];

	TCHAR szTemp[MAX_PATH];
	szTemp[0] = '\0';
	if ( lpszDosPath==NULL || !GetLogicalDriveStrings(_countof(szTemp)-1, szTemp) ){  return strResult; }

	TCHAR szName[MAX_PATH];
	TCHAR szDrive[3] = TEXT(" :");
	BOOL bFound = FALSE;
	TCHAR* p = szTemp;

	do
	{
		*szDrive = *p;
		if ( QueryDosDevice(szDrive, szName, _countof(szName)) )
		{   
			UINT uNameLen = (UINT)strlen(szName);   
			if (uNameLen < MAX_PATH)    
			{    
				bFound = (strnicmp(lpszDosPath, szName,uNameLen) == 0);    
				if ( bFound )
				{        
					sprintf(strResult, TEXT("%s%s"), szDrive, lpszDosPath+uNameLen);
				}   
			} 
		}
		while (*p++);
	}while (!bFound && *p); // end of string

	return strResult;
}