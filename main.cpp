#include <stdio.h>
#include <Windows.h>
#include <vector>

void FindActiveProcessorIDs(const SYSTEM_INFO& sInfo, std::vector<unsigned int>& ids);

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

	// Find active processors' ids:
	printf("Active processors' ids are: ");
	std::vector<unsigned int> proIds;
	FindActiveProcessorIDs(sysInfo, proIds);
	for (auto proId : proIds) {
		printf("%d ", proId);
	}
	printf("\n");

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