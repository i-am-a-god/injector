#include "injector.hpp"

/*
* we just gotta write error handling on swhx & crt
* should be fine w/o cleanup but if required we still do it on the inject_dll() function
* if you need the handle after injection then clone the handle you have and write use the cloned handle on the inject_dll() function
*/

auto injector_t::inject_dll_via_swhx(injection_data data) -> bool
{
	const auto thread_id = GetWindowThreadProcessId(data.process_window_handle, NULL);
	if (!thread_id)
		return false;

	const auto module = LoadLibraryEx(data.dll_path.c_str(), nullptr, DONT_RESOLVE_DLL_REFERENCES);
	if (!module)
		return false;

	const auto module_export = (HOOKPROC)GetProcAddress(module, data.next_hook_func.c_str());
	if (!module_export)
		return false;

	const auto module_hook = SetWindowsHookEx(WH_GETMESSAGE, module_export, module, thread_id);
	if (!module_hook)
		return false;

	if (!PostThreadMessage(thread_id, WM_NULL, 0, 0))
		return false;

	return true;
}

auto injector_t::inject_dll_via_crt(injection_data data) -> bool
{
	const auto kernel32 = GetModuleHandle("kernel32.dll");
	if (!kernel32)
		return false;

	const auto load_library = (LPVOID)GetProcAddress(kernel32, "LoadLibrary");
	if (!load_library)
		return false;

	const auto allocate_space = VirtualAllocEx(data.process_handle, 0, data.dll_path.length() + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!allocate_space)
		return false;

	const auto write_space = WriteProcessMemory(data.process_handle, (LPVOID)allocate_space, data.dll_path.c_str(), data.dll_path.length() + 1, 0);
	if (!write_space)
		return false;

	const auto crt = CreateRemoteThread(data.process_handle, 0, 0, (LPTHREAD_START_ROUTINE)load_library, (LPVOID)allocate_space, 0, 0);
	if (!crt)
		return false;

	return true;
}

auto injector_t::inject_dll(injection_data data) -> bool
{
	// ...

	switch (data.injection_method) {
	case injection_type::create_remote_thread:
		this->inject_dll_via_crt(data);
		break;
	case injection_type::set_windows_hook_ex:
		this->inject_dll_via_swhx(data);
		break;
	}

	CloseHandle(data.process_handle);
	CloseHandle(data.process_window_handle);

	return false;
}
