#include <iostream>
#include <string>
#include <vector>
#include "menu_item.hpp"


class MenuItems {
	public:
		std::vector<MenuItem> entries;
		std::int8_t count;

		std::vector<std::string> toStrings() {
			
			std::vector<std::string> result;
			for(int i = 0; i < entries.size(); i++) {
			
				result.push_back(entries[i].toString());
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

		MenuItem* findByShortcut(std::string shortcut) {
			for(std::int8_t i{0}; i < count; i++) {
				if(shortcut == entries[i].shortcut) {
					return &entries[i];
				}
			}
			return nullptr;
		}

		static MenuItems fromYAMLNode(YAML::Node items, Errors* error) {
			
			MenuItems instance;
			int index = 0;
			for(std::int8_t i = 0; i < items.size(); i++) {
				std::int16_t errorLine{0};
				
				MenuItem entry;
				if(!items[i]["shortcut"].IsDefined()) {
					errorLine = items[i].Mark().line;
				}
				if(!items[i]["title"].IsDefined()) {
					errorLine = items[i].Mark().line;
				}
				if(!items[i]["username"].IsDefined()) {
					errorLine = items[i].Mark().line;
				}
				if(!items[i]["server"].IsDefined()) {
					errorLine = items[i].Mark().line;
				}
								
				if(errorLine) {
					std::string message = "Error in config file arround line " + std::to_string(errorLine);
					error->add(message);
					continue;
				}
				
				entry.index = index;
				entry.shortcut = items[i]["shortcut"].as<std::string>();
				entry.title = items[i]["title"].as<std::string>();
				entry.server = items[i]["server"].as<std::string>();;
				entry.username = items[i]["username"].as<std::string>();;
				entry.port = items[i]["port"].IsDefined() ? items[i]["port"].as<std::string>() : "22";
				index++;
				instance.entries.push_back(entry);
			}
			instance.count = index++;
			return instance;
		}
	
};