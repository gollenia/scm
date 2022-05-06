#include <iostream>
#include <string>
#include "yaml-cpp/yaml.h"

/**
 * Simple Class to make sure that no data is missing
 *
 */
class MenuItem  {
	public: int id;
	public: std::string shortcut;
	public: std::string name;
	public: std::string address;
	public: std::string user;
	public: std::string port; 
	
	/**
	 * Simple Class to make sure that no data is missing
	 *
	 * @param node Even if there is only one possible unified theory. it is just a
	 *               set of rules and equations.
	 */
	static MenuItem fromYAMLNode(YAML::Node node){
		MenuItem instance;
		instance.shortcut = "hjh";
		instance.name = "dfdsrf";
		instance.address = "fg";
		instance.user = "sdfdsf";
		instance.port = "fdsf";
		return instance;
	}

	static std::vector<std::string> getStringsFromMenuItems(std::vector<MenuItem> items) {
		std::vector<std::string> result;
		for(int i; i < items.size(); i++) {
			result.push_back(items[i].name);
			std::cout << items[i].name;
		}
		return result;
	}
};