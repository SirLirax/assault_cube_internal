#pragma once
#include <windows.h>
#include <vector>

namespace mem
{
	void patchBytes(BYTE* destination, BYTE* source, unsigned int size);
	void nopBytes(BYTE* destination, unsigned int size);

	uintptr_t resolveOffsets(uintptr_t pointer, std::vector<unsigned int> offsets);
}