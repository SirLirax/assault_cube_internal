#pragma once
#include <windows.h>

bool Detour32(BYTE* source, BYTE* destination, uintptr_t length);
BYTE* TrampHook32(BYTE* source, BYTE* destination, uintptr_t length);

struct Hook
{
	bool is_active_ = false;
	BYTE* source_ = nullptr;
	BYTE* destination_ = nullptr;
	BYTE* pointer_to_gateway_function_pointer_ = nullptr;
	uintptr_t length_ = 0;
	BYTE original_bytes_[10] = { 0 };

	Hook(BYTE* source, BYTE* destination, BYTE* pointer_to_gateway_function_pointer, uintptr_t length);
	Hook(char* export_name, char* mod_name, BYTE* destination, BYTE* pointer_to_gateway_function_pointer, uintptr_t length);

	void enableHook();
	void disableHook();
	void toggleHook();
};
