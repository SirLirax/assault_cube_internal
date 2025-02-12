#include "hook.h"
#include "entity.h"
#include "mem.h"

bool Detour32(BYTE* source, BYTE* destination, uintptr_t length)
{
	if (length < 5)
	{
		return false;
	}

	DWORD old_protection;
	VirtualProtect(source, length, PAGE_EXECUTE_READWRITE, &old_protection);
	uintptr_t relative_address = destination - source - 5;

	*source = 0xE9;

	*(uintptr_t*)(source + 1) = relative_address;
	VirtualProtect(source, length, old_protection, &old_protection);
	return true;
}

BYTE* TrampHook32(BYTE* source, BYTE* destination, uintptr_t length)
{
	if (length < 5)
	{
		return nullptr;
	}

	BYTE* gateway = (BYTE*)VirtualAlloc(0, length, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	memcpy_s(gateway, length, source, length);

	uintptr_t gateway_relative_address = source - gateway - 5;

	*(gateway + length) = 0xE9;

	*(uintptr_t*)((uintptr_t)gateway + length + 1) = gateway_relative_address;

	Detour32(source, destination, length);

	return gateway;
}

Hook::Hook(BYTE* source, BYTE* destination, BYTE* pointer_to_gateway_function_pointer, uintptr_t length)
{
	this->source_ = source;
	this->destination_ = destination;
	this->length_ = length;
	this->pointer_to_gateway_function_pointer_ = pointer_to_gateway_function_pointer;
}

Hook::Hook(char* export_name, char* mod_name, BYTE* destination, BYTE* pointer_to_gateway_function_pointer, uintptr_t length)
{
	HMODULE module_handle = GetModuleHandleA(mod_name);
	if (module_handle == 0)
	{
		this->source_ = NULL;
	}
	else
	{
		this->source_ = (BYTE*)GetProcAddress(module_handle, export_name);
	}
	this->destination_ = destination;
	this->length_ = length;
	this->pointer_to_gateway_function_pointer_ = pointer_to_gateway_function_pointer;
}

void Hook::enableHook()
{
	memcpy(original_bytes_, source_, length_);
	*(uintptr_t*)pointer_to_gateway_function_pointer_ = (uintptr_t)TrampHook32(source_, destination_, length_);
	is_active_ = true;
}

void Hook::disableHook()
{
	mem::patchBytes(source_, original_bytes_, length_);
	is_active_ = false;
}

void Hook::toggleHook()
{
	if (!is_active_)
	{
		enableHook();
	}
	else
	{
		disableHook();
	}
}