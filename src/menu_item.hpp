#include <iostream>
#include <string>
#include "yaml-cpp/yaml.h"

using namespace std;

/**
 * Simple Class to make sure that no data is missing
 *
 */
class MenuItem  {
	public: int id;
	public: string key;
	public: string name;
	public: string address;
	public: string user;
	public: string port; 
	
	/**
	 * Simple Class to make sure that no data is missing
	 *
	 * @param node Even if there is only one possible unified theory. it is just a
	 *               set of rules and equations.
	 */
	static MenuItem fromYAMLNode(YAML::Node node){
		MenuItem instance;
		instance.key = node["key"].as<string>();
		instance.name = node["name"].as<string>();
		instance.address = node["address"].as<string>();
		instance.user = node["user"].as<string>();
		instance.port = node["port"].as<string>();
		return instance;
	}

	static std::vector<std::string> getStringsFromMenuItems(std::vector<MenuItem> items) {
		std::vector<std::string> result;
		for(int i; i >= items.size(); i++) {
			result.push_back(items[i].name);
		}
		return result;
	}
};