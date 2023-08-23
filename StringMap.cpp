#include <Windows.h>
#include <fstream>

#include "json.hpp"
using nlohmann::json;

#include "StringMap.h"

unordered_map<string, string> cht_mapping;

void FetchStringMap()
{
	std::fstream file("cht_mapping.json");
	json data = json::parse(file);

	for (auto elm : data.items()) {
		cht_mapping.emplace(elm.key(), elm.value().get<string>());
	}
}