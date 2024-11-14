#include "process-utilities.hpp"

auto process_utilities_t::get_process_list_as_vector() -> std::vector<std::tuple<std::string, std::uintptr_t>>
{
	std::vector<std::tuple<std::string, std::uintptr_t>> process_list{  };

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!snapshot)
		return process_list;

	PROCESSENTRY32 process_entry{  };
	process_entry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(snapshot, &process_entry)) {
		do {
			const auto process_id = process_entry.th32ProcessID;
			const auto process_name = process_entry.szExeFile;

			if (process_id && process_name)
				process_list.emplace_back(process_name, process_id);
		} while (Process32Next(snapshot, &process_entry));
	}

	return process_list;
}

auto process_utilities_t::get_process_id(std::vector<std::tuple<std::string, std::uintptr_t>> process_list, const std::string& process_name_search) -> std::uintptr_t
{
	for (const auto [process_name, process_id] : process_list) {
		if (process_name == process_name_search || strcmp(process_name.c_str(), process_name_search.c_str()))
			return process_id;
	}

	return 0;
}

auto process_utilities_t::get_process_handle(const std::uintptr_t process_id) -> HANDLE
{
	if (!process_id)
		return 0;

	const auto handle = OpenProcess(PROCESS_ALL_ACCESS, 0, process_id);
	if (!handle)
		return 0;

	return handle;
}
