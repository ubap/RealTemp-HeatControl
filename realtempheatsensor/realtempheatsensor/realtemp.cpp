#include "realtemp.h"

bool ReadTemperature(unsigned int *temperature)
{
	unsigned int temp1, temp2;
	static HANDLE hProcess;

	// core 1
	// handle invalid 
	if(!ReadProcessMemory(hProcess, CORE_0, &temp1, sizeof(int), 0))
	{
		DWORD processId = _GetProcessId("RealTemp.exe");
		// nie znaleziono procesu
		if (!processId)
			return false;
		hProcess = OpenProcess(PROCESS_VM_READ, false, processId);

		if(!ReadProcessMemory(hProcess, CORE_0, &temp1, sizeof(int), 0))
			return false;
	}
	// core 2
	ReadProcessMemory(hProcess, CORE_1, &temp2, sizeof(int), 0); 

	//CloseHandle(hProcess);

	// temperature chyba nie bedzie powyzej MAX_TEMP 
	if (temp1 > MAX_TEMP || temp2 > MAX_TEMP)
			return false;

	*temperature =  temp1 > temp2 ? temp1 : temp2;
	return true;
}