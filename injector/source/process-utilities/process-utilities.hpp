#pragma once

#include <windows.h>
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <TlHelp32.h>

class process_utilities_t {
public:
	auto get_process_list_as_vector() -> std::vector<std::tuple<std::string, std::uintptr_t>>;
	auto get_process_id(std::vector<std::tuple<std::string, std::uintptr_t>> process_list, const std::string& process_name_search) -> std::uintptr_t;
	auto get_process_handle(const std::uintptr_t process_id) -> HANDLE;

	process_utilities_t() = default;
	process_utilities_t(const process_utilities_t&) = delete;
};

inline auto process_utilities = std::make_unique<process_utilities>();