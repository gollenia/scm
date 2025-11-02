#include <iostream>
#include <string>
#include <vector>
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

		MenuItem* find(std::uint8_t index) {
			if(index >= entries.size()) {
				std::cout << "Menu index not found";
				return nullptr;
			}
			return &entries[index];
		}

		void filter(const std::string& searchQuery) {
			filteredEntries.clear();

			if (searchQuery.empty() || searchQuery.length() < 2) {
				filteredEntries = entries;
				return;
			}

			for (const auto& entry : entries) {
				if (entry.title_.find(searchQuery) != std::string::npos ||  // 🔍 Suche im Titel
					entry.server_.find(searchQuery) != std::string::npos || // 🔍 Suche im Server
					entry.username_.find(searchQuery) != std::string::npos || // 🔍 Suche im Username
					entry.port_.find(searchQuery) != std::string::npos) { // 🔍 Suche im Port
					filteredEntries.push_back(entry);
				}
			}
    	}

		static MenuItems fromYAMLNode(YAML::Node items, Errors* error) {
			
			MenuItems instance;
			std::int8_t index = 0;
			for(std::int8_t i = 0; i < items.size(); i++) {
				std::int16_t errorLine = items[i].Mark().line;
				
				MenuItem entry;

				if (!items[i]["server"].IsDefined()) {
					std::string message = "Fehler in der Konfigurationsdatei (Zeile " + 
										std::to_string(errorLine) + "): Kein Server angegeben!";
					error->push(message);
					continue;
        		}
				
				entry.index_ = index;
				index++;
				
				entry.server_ = items[i]["server"].as<std::string>();
				entry.title_ = items[i]["title"].IsDefined() ? items[i]["title"].as<std::string>() : items[i]["server"].as<std::string>();
				entry.username_ = items[i]["username"].IsDefined() ? items[i]["username"].as<std::string>() : ""; // get user from os here
				entry.port_ = items[i]["port"].IsDefined() ? items[i]["port"].as<std::string>() : "22";
				
				instance.entries.push_back(entry);
			}

			return instance;
		}
	
};