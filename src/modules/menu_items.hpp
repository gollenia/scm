#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "menu_item.hpp"


class MenuItems {
	public:
		std::vector<MenuItem> entries;
		std::vector<MenuItem> filteredEntries;
		std::int8_t count;

		std::vector<std::string> toStrings() const {
			std::vector<std::string> result;

			result.reserve(filteredEntries.size());
			for(int i = 0; i < filteredEntries.size(); i++) {
				result.push_back(filteredEntries[i].ToString());
			}
			return result;
		}

		int size() {
			return entries.size();
		}

		MenuItem* find(int index) {
			if(index < 0 || static_cast<std::size_t>(index) >= filteredEntries.size()) {
				return nullptr;
			}
			return &filteredEntries[index];
		}

		void filter(const std::string& searchQuery) {
			filteredEntries.clear();

			if (searchQuery.empty() || searchQuery.length() < 2) {
				filteredEntries = entries;
				return;
			}

			std::string query = StringHelpers::to_lower(searchQuery);
			for (const auto& entry : entries) {
				if (StringHelpers::to_lower(entry.title_).find(query) != std::string::npos ||  // 🔍 Suche im Titel
					StringHelpers::to_lower(entry.server_).find(query) != std::string::npos || // 🔍 Suche im Server
					StringHelpers::to_lower(entry.username_).find(query) != std::string::npos || // 🔍 Suche im Username
					StringHelpers::to_lower(entry.port_).find(query) != std::string::npos) { // 🔍 Suche im Port
					filteredEntries.push_back(entry);
				}
			}
    	}

		static MenuItems fromYAMLNode(YAML::Node items, Errors* error) {
			
			MenuItems instance;
			int index = 0;
			for(std::size_t i = 0; i < items.size(); i++) {
				std::int16_t errorLine = items[i].Mark().line;
				
				MenuItem entry;

				if (!items[i]["server"].IsDefined()) {
					std::string message = "Fehler in der Konfigurationsdatei (Zeile " + 
										std::to_string(errorLine) + "): Kein Server angegeben!";
					error->push(message);
					continue;
        		}
				
				entry.index_ = index;
				entry.yaml_index_ = i;
				index++;
				
				entry.server_ = items[i]["server"].as<std::string>();
				entry.title_ = items[i]["title"].IsDefined() ? items[i]["title"].as<std::string>() : items[i]["server"].as<std::string>();
				entry.username_ = items[i]["username"].IsDefined() ? items[i]["username"].as<std::string>() : ""; // get user from os here
				entry.port_ = items[i]["port"].IsDefined() ? items[i]["port"].as<std::string>() : "22";
				if (items[i]["color"].IsDefined()) {
					const auto color = items[i]["color"];
					const std::string value = color.IsScalar() ? color.Scalar() : "";
					if (value.size() == 7 && value[0] == '#' &&
						std::all_of(value.begin() + 1, value.end(), [](unsigned char c) { return std::isxdigit(c); })) {
						entry.color_ = static_cast<unsigned int>(std::stoul(value.substr(1), nullptr, 16));
					} else {
						error->push("Ungueltige Farbe in der Konfigurationsdatei (Zeile " +
							std::to_string(color.Mark().line + 1) + "): Erwartet wird #RRGGBB.");
					}
				}
				
				instance.entries.push_back(entry);
			}

			return instance;
		}
	
};
