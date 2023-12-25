////////////////////////////////////////////
//      Use UNICODE Character Set     //
//////////////////////////////////////////



#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <Windows.h>

#define SeDebugPriv 20
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004)
#define NtCurrentProcess ( (HANDLE)(LONG_PTR) -1 ) 
#define ProcessHandleType 0x7
#define SystemHandleInformation 16 






typedef struct كلاينت
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
} كلاينايد, * PCLIENT_ID;

typedef struct سترينق {
	USHORT Length;
	USHORT MaximumLength;
	PWCH   Buffer;
} ستر, * بستر;

typedef struct _تيبلسيستم_هادندل_اينفو
{
	ULONG ProcessId;
	BYTE ObjectTypeName;
	BYTE Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} سيستم_هادندل_اينفو_هاندل, * PSYSTEM_HANDLE;


typedef struct اوبج {
	ULONG           Length;
	HANDLE          RootDirectory;
	بستر            ObjectName;
	ULONG           Attributes;
	PVOID           SecurityDescriptor;
	PVOID           SecurityQualityOfService;
}  OBJECT_ATTRIBUTES, * بوب;



typedef NTSTATUS(NTAPI* بريف)(
	ULONG Privilege,
	BOOLEAN Enable,
	BOOLEAN CurrentThread,
	PBOOLEAN Enabled
	);


typedef NTSTATUS(NTAPI* دوب)(
	HANDLE SourceProcessHandle,
	HANDLE SourceHandle,
	HANDLE TargetProcessHandle,
	PHANDLE TargetHandle,
	ACCESS_MASK DesiredAccess,
	ULONG Attributes,
	ULONG Options
	);

typedef struct سيستم_هاندل
{
	ULONG HandleCount;
	سيستم_هادندل_اينفو_هاندل Handles[1];
} سيستم_هاندل_اينفو, * بسيستم_هاندل_اينفو;



typedef NTSYSAPI NTSTATUS(NTAPI* اوبن)(
	PHANDLE            ProcessHandle,
	ACCESS_MASK        DesiredAccess,
	بوب                ObjectAttributes,
	PCLIENT_ID         ClientId
	);

typedef NTSTATUS(NTAPI* كويري)(
	ULONG SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
	);


اوبن Get_NtOpenProcess(const char* name)
{
	HMODULE ntdll = GetModuleHandleA("ntdll");
	اوبن NtOpenProcess = (اوبن)GetProcAddress(ntdll, name);

	return (اوبن)NtOpenProcess;
}

اوبج ObjectAttr(بستر name, HANDLE hRoot, ULONG attributes, PSECURITY_DESCRIPTOR security)
{
	OBJECT_ATTRIBUTES object;

	object.Length = sizeof(OBJECT_ATTRIBUTES);

	object.Attributes = attributes;
	object.RootDirectory = hRoot;
	object.SecurityDescriptor = security;
	object.ObjectName = name;


	return object;
}

دوب Get_NtDuplicateObject(const char* name)
{
	HMODULE ntdll = GetModuleHandleA("ntdll");
	دوب NtDuplicateObject = (دوب)GetProcAddress(ntdll, name);

	return (دوب)NtDuplicateObject;
}



بريف Get_RtlAdjustPrivilege(const char* name)
{
	HMODULE ntdll = GetModuleHandleA("ntdll");

	بريف RtlAdjustPrivilege = (بريف)GetProcAddress(ntdll, name);

	return (بريف)RtlAdjustPrivilege;
}


كويري Get_NtQuerySystemInformation(const char* name)
{
	HMODULE ntdll = GetModuleHandleA("ntdll");
	كويري NtQuerySystemInformation = (كويري)GetProcAddress(ntdll, name);

	return (كويري)NtQuerySystemInformation;
}







سيستم_هاندل_اينفو* hInfo;

HANDLE هاندل_تجارب = NULL;
HANDLE hProcess = NULL;
HANDLE هايجاكد_هاندل = NULL;

void CleanAndExit(const char* ErrorMessage)
{
	delete[] hInfo;

	هاندل_تجارب ? CloseHandle(هاندل_تجارب) : 0;

	std::cout << ErrorMessage << '\n';

}
bool IsHandleValid(HANDLE handle)
{
	if (handle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else if (handle)
		return true;
}

HANDLE GethijackedHandle(DWORD pID)
{

	بريف                      بريفلول = Get_RtlAdjustPrivilege("RtlAdjustPrivilege");
	دوب                       دوبليكيت = Get_NtDuplicateObject("NtDuplicateObject");
	اوبن                      اوبن = Get_NtOpenProcess("NtOpenProcess");
	كويري                     كويريسيستم = Get_NtQuerySystemInformation("NtQuerySystemInformation");



	اوبج                      اوبكتااتر = ObjectAttr(NULL, NULL, NULL, NULL);
	كلاينايد                   كلاينتايد = { 0 };
	boolean                   OldPriv;



	بريفلول(SeDebugPriv, TRUE, FALSE, &OldPriv);

	DWORD size = sizeof(سيستم_هاندل_اينفو);

	NTSTATUS ntRet = NULL;

	do
	{
		delete[] hInfo;

		size *= 1.5;

		try
		{
			hInfo = (بسيستم_هاندل_اينفو) new byte[size];
		}
		catch (std::bad_alloc)
		{

			CleanAndExit("Bad AlloC");
		}
		Sleep(1);

	} while ((ntRet = كويريسيستم(SystemHandleInformation, hInfo, size, NULL)) == STATUS_INFO_LENGTH_MISMATCH);



	if (!NT_SUCCESS(ntRet))
	{
		CleanAndExit("NtQuery Didnt Success");
	}

	for (unsigned int i = 0; i < hInfo->HandleCount; ++i)
	{
		static int NumOfHandles = i;




		if (NumOfHandles > 100)
		{
			CleanAndExit("Handle Leakage");
		}


		if (hInfo->Handles[i].ObjectTypeName != ProcessHandleType)
			continue;

		if (!IsHandleValid((HANDLE)hInfo->Handles[i].Handle))
			continue;



		كلاينتايد.UniqueProcess = (DWORD*)hInfo->Handles[i].ProcessId;



		ntRet = اوبن(&هاندل_تجارب, PROCESS_DUP_HANDLE, &اوبكتااتر, &كلاينتايد);

		if (!IsHandleValid(هاندل_تجارب) || !NT_SUCCESS(ntRet))
			continue;

		ntRet = دوبليكيت(هاندل_تجارب, (HANDLE)hInfo->Handles[i].Handle, NtCurrentProcess, &هايجاكد_هاندل, PROCESS_ALL_ACCESS, 0, 0);




		if (!IsHandleValid(هايجاكد_هاندل) || !NT_SUCCESS(ntRet))
			continue;

		if (GetProcessId(هايجاكد_هاندل) == pID)
		{
			hProcess = هايجاكد_هاندل;
			break;
		}
		else
		{
			CloseHandle(هايجاكد_هاندل);
			CloseHandle(هاندل_تجارب);
			continue;
		}
	}
	CleanAndExit("Success");
	return hProcess;

}




HANDLE hProc;
DWORD pID;
//uintptr_t client;

bool AttachProcess(const wchar_t* proc)
{
	PROCESSENTRY32 ProcEntry32;

	ProcEntry32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	while (Process32Next(hSnap, &ProcEntry32))
	{
		if (!wcscmp(ProcEntry32.szExeFile, proc))
		{
			std::cout << "Found Process!\n";

			pID = ProcEntry32.th32ProcessID;
			hProc = GethijackedHandle(pID);

			break;
		}
	}
	CloseHandle(hSnap);
	return true;
}
uintptr_t ModuleAddress(const wchar_t* proc)
{
	MODULEENTRY32 ModEntry32;

	ModEntry32.dwSize = sizeof(MODULEENTRY32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);

	uintptr_t result;

	while (Module32Next(hSnap, &ModEntry32))
	{
		if (!wcscmp(ModEntry32.szModule, proc))
		{
			result = (uintptr_t)ModEntry32.modBaseAddr;
			std::cout << "Found: " << result << std::endl;
			break;
		}
	}
	CloseHandle(hSnap);
	return result;
}
template <class T>
T rpm(const uintptr_t address) noexcept
{
	T بفر;
	ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(address), &بفر, sizeof(T), nullptr);
	return بفر;
}

template <typename ت>
void wpm(const std::uintptr_t address, const ت& value) noexcept
{
	WriteProcessMemory(hProc, reinterpret_cast<void*>(address), &value, sizeof(ت), NULL);
}