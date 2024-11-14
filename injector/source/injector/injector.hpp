#pragma once

#include <windows.h>
#include <string>
#include <thread>
#include <memory>

typedef enum {
	set_windows_hook_ex,
	create_remote_thread
	// will add more at some point maybe
} injection_type, *injection_type_ptr;

typedef struct {
	HANDLE process_handle;
	HWND process_window_handle;

	std::string dll_path;
	std::string next_hook_func;

	injection_type injection_method;
} injection_data, *injection_data_ptr;

class injector_t {
private:
	auto inject_dll_via_swhx(injection_data data) -> bool;
	auto inject_dll_via_crt(injection_data data) -> bool;
public:
	auto inject_dll(injection_data data) -> bool;

	injector_t() = default;
	injector_t(const injector_t&) = delete;
};

inline auto injector = std::make_unique<injector_t>();