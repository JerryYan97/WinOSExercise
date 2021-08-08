#define SECURITY_WIN32

#include <stdio.h>
#include <vector>
#include <Windows.h>
#include <comdef.h>
#include <Security.h>
#include <secext.h>
#include <string>


void FindActiveProcessorIDs(const SYSTEM_INFO& sInfo, std::vector<unsigned int>& ids);
void WinTypeID2Str(const DWORD& type, std::string& rStr);

int main() {
	printf("****************************\n");
	printf("Display system info\n");
	printf("****************************\n");

	// Get and print Native system info
	SYSTEM_INFO sysInfo;
	::GetNativeSystemInfo(&sysInfo);

	printf("Processor architecture is:");
	switch (sysInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		printf("x64 (AMD or Intel)");
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		printf("ARM");
		break;
	case PROCESSOR_ARCHITECTURE_ARM64:
		printf("ARM64");
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		printf("Intel Itanium-based");
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		printf("x86");
		break;
	case PROCESSOR_ARCHITECTURE_UNKNOWN:
		printf("Unknown structure");
		break;
	default:
		exit(1);
	}
	printf("\n");

	printf("The Page size is: %d Bytes\n", sysInfo.dwPageSize);
	printf("Min process address: 0x%p\n", sysInfo.lpMinimumApplicationAddress);
	printf("Max process address: 0x%p\n", sysInfo.lpMaximumApplicationAddress);

	// Show active processors' ids:
	printf("Active processors' ids are: ");
	std::vector<unsigned int> proIds;
	FindActiveProcessorIDs(sysInfo, proIds);
	for (auto proId : proIds) {
		printf("%d ", proId);
	}
	printf("\n");

	printf("The number of logical processors is: %d\n", sysInfo.dwNumberOfProcessors);

	// Show Computer name
	wchar_t nameBuffer[MAX_COMPUTERNAME_LENGTH + 1]{};
	DWORD nSize = MAX_COMPUTERNAME_LENGTH + 1;
	BOOL success = ::GetComputerNameW(nameBuffer, &nSize);
	if (!success) {
		printf("Failed to get the computer name.\n");
	}
	else {
		printf("The computer name is: %ws\n", nameBuffer);
	}

	// Show Windows directory
	wchar_t dirBuffer[MAX_PATH + 1]{};
	UINT rVal = ::GetWindowsDirectoryW(dirBuffer, MAX_PATH);
	if (rVal) {
		printf("The Windows director is: %ws\n", dirBuffer);
	}
	else {
		BOOL e = ::GetLastError();
		HRESULT hr = HRESULT_FROM_WIN32(e);
		_com_error err(hr);
		printf("Failed to get the windows directory. Error is: %ws\n", err.ErrorMessage());
	}
	
	// Show Performance stamp
	LARGE_INTEGER ticks;
	if (::QueryPerformanceCounter(&ticks)) {
		printf("The tick is: %lld \n", ticks.QuadPart);
	}
	else {
		BOOL e = ::GetLastError();
		HRESULT hr = HRESULT_FROM_WIN32(e);
		_com_error err(hr);
		printf("Failed to get the performance counter. Error is: %ws\n", err.ErrorMessage());
	}

	// Show product info
	DWORD type = 0;
	BOOL res = ::GetProductInfo(1000, 1000, 1000, 1000, &type);
	if (res) {
		printf("The type of the product is %d \n", type);
	}
	else {
		BOOL e = ::GetLastError();
		HRESULT hr = HRESULT_FROM_WIN32(e);
		_com_error err(hr);
		printf("Failed to get the windows product type. Error is: %ws\n", err.ErrorMessage());
	}

	// Show Computer Object name:
	/* ERROR: Invalid or corrupt file: cannot read secur32.dll;
	ULONG nCOSize = 0;
	::GetComputerObjectNameW(NameSamCompatible, nullptr, &nCOSize);
	wchar_t* accNameBuffer = new wchar_t[(int)(nCOSize + 1UL)];
	if (::GetComputerObjectName(NameSamCompatible, accNameBuffer, &nCOSize)) {
		printf("The account name of the OS is: %ws\n", accNameBuffer);
	}
	else {
		BOOL e = ::GetLastError();
		HRESULT hr = HRESULT_FROM_WIN32(e);
		_com_error err(hr);
		printf("Failed to get the OS account name. Error is: %ws\n", err.ErrorMessage());
	}

	delete[] accNameBuffer;
	*/
	printf("\n\n");
}


void FindActiveProcessorIDs(const SYSTEM_INFO& sInfo, std::vector<unsigned int>& ids) {
	DWORD mask = 1;
	for (size_t i = 0; i < 32; i++)
	{
		if (mask & sInfo.dwActiveProcessorMask) {
			ids.push_back(i);
		}
		mask <<= 1;
	}
}

void WinTypeID2Str(const DWORD& type, std::string& rStr) {

}
