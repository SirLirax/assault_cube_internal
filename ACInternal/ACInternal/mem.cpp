#include <Windows.h>
#include <vector>
#include "mem.h"

void mem::patchBytes(BYTE* destination, BYTE* source, unsigned int size)
{
	DWORD old_protect;
	VirtualProtect(destination, size, PAGE_EXECUTE_READWRITE, &old_protect); // Gets Page_Execute_Readwrite for the page
	memcpy(destination, source, size); // replaces the destination bytes with our bytes.
	VirtualProtect(destination, size, old_protect, &old_protect); // Restores old protection.
}

void mem::nopBytes(BYTE* destination, unsigned int size)
{
	DWORD old_protect;
	VirtualProtect(destination, size, PAGE_EXECUTE_READWRITE, &old_protect); // Gets Page_Execute_Readwrite for the page
	// 0x90 is NOP operation.
	memset(destination, 0x90, size); // replaces the destination bytes with our bytes.
	VirtualProtect(destination, size, old_protect, &old_protect); // Restores old protection.
}

uintptr_t mem::resolveOffsets(uintptr_t pointer, std::vector<unsigned int> offsets)
{
	uintptr_t current_address = pointer;
	for (int i = 0; i < offsets.size(); i++)
	{
		current_address = *(uintptr_t*)current_address; // dereference current pointer;
		current_address += offsets[i]; // add offset
	}
	return current_address;
}
