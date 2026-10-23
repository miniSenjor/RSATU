#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

const std::pair<DWORD, const char*> protectFlags[] = {
	{PAGE_EXECUTE, "PAGE_EXECUTE"},
	{PAGE_EXECUTE_READ, "PAGE_EXECUTE_READ"},
	{PAGE_EXECUTE_READWRITE, "PAGE_EXECUTE_READWRITE"},
	{PAGE_EXECUTE_WRITECOPY, "PAGE_EXECUTE_WRITECOPY"},
	{PAGE_NOACCESS, "PAGE_NOACCESS"},
	{PAGE_READONLY, "PAGE_READONLY"},
	{PAGE_READWRITE, "PAGE_READWRITE"},
	{PAGE_WRITECOPY, "PAGE_WRITECOPY"},
	{PAGE_TARGETS_INVALID, "PAGE_TARGETS_INVALID"},
	{PAGE_TARGETS_NO_UPDATE, "PAGE_TARGETS_NO_UPDATE"},
	{PAGE_GUARD, "PAGE_GUARD"},
	{PAGE_NOCACHE, "PAGE_NOCACHE"},
	{PAGE_WRITECOMBINE, "PAGE_WRITECOMBINE"},
	{PAGE_ENCLAVE_DECOMMIT, "PAGE_ENCLAVE_DECOMMIT"},
	{PAGE_ENCLAVE_THREAD_CONTROL, "PAGE_ENCLAVE_THREAD_CONTROL"},
	{PAGE_ENCLAVE_UNVALIDATED, "PAGE_ENCLAVE_UNVALIDATED"}
};

void getProtect(DWORD& Protect)
{
	string s = "";
	bool first = true;
	for (const auto& p : protectFlags) {
		if (Protect & p.first) {
			if (!first) s+= " | ";
			s += p.second;
			first = false;
		}
	}
	if (first)
		s+= "No protect";
	cout << s;
	for (int i = 27; i> s.length(); i--)
		cout << " ";
}

void getState(DWORD& State)
{
	string s;
	switch (State)
	{
	case MEM_COMMIT:
		s = "MEM_COMMIT";
		break;
	case MEM_FREE:
		s = "MEM_FREE";
		break;
	case MEM_RESERVE:
		s = "MEM_RESERVE";
		break;
	default:
		s = "no state";
		break;
	}
	cout << s;
	for (int i = 11; i > s.length(); i--)
		cout << " ";
}

void getType(DWORD& Type)
{
	string s;
	switch (Type)
	{
	case MEM_IMAGE:
		s = "MEM_IMAGE";
		break;
	case MEM_MAPPED:
		s = "MEM_MAPPED";
		break;
	case MEM_PRIVATE:
		s = "MEM_PRIVATE";
		break;
	default:
		s = "no type";
		break;
	}
	cout << s;
	for (int i = 11; i > s.length(); i--)
		cout << " ";
}

int main()
{
	SetConsoleOutputCP(1251);
	char* curAddr = 0;
	MEMORY_BASIC_INFORMATION mem;
	while (VirtualQuery(curAddr, &mem, sizeof(mem)))
	{
		cout << mem.BaseAddress << " " << mem.RegionSize << " ";
		string sRegionSize = to_string(mem.RegionSize);
		for (int i = 10; i> sRegionSize.length(); i--)
			cout << " ";
		
		getProtect(mem.Protect);
		cout << " ";
		
		getState(mem.State);
		cout << " ";
		
		getType(mem.Type);
		long long addr = (long long)mem.BaseAddress;
		bool isAlig = addr % 65536 == 0;
		bool isRead = mem.Protect != 0 && mem.Protect != 1;
		bool isCommit = mem.State == MEM_COMMIT;
		bool isImage = mem.Type == MEM_IMAGE;

		if (isAlig && isRead && isCommit && isImage)
			if (*curAddr == 'M' && *(curAddr + 1) == 'Z')
			{
				char fileName[65536];
				DWORD nameLength = GetModuleFileNameA((HMODULE)mem.BaseAddress, fileName, MAX_PATH);
				if (nameLength > 0)
					cout << fileName;
			}
		cout << endl;
		curAddr += mem.RegionSize;
	}
	
}
