#include <iostream>
#include <string>
#include "yaml-cpp/yaml.h"

/**
 * Simple Class to make sure that no data is missing
 *
 */
class MenuItem  {
	public: int index{-1};
	public: std::string shortcut;
	public: std::string title;
	public: std::string server;
	public: std::string username;
	public: std::string port; 
	
	/**
	 * Simple Class to make sure that no data is missing
	 *
	 * @param node Even if there is only one possible unified theory. it is just a
	 *               set of rules and equations.
	 */
	

	public: std::string toString() {
		std::string result = std::to_string(index + 1) + "|" + shortcut + "|" + title + "|" + username + "@" + server;
		if(port != "22") {
			result += ":" + port;
		}
		
		return result;
	}

	public: std::string getCommand(std::string app = "ssh") {
		std::string result = app + " " + username + "@" + server;
		if(port != "22") {
			result += " -p " + port;
		}
		return result;
	}
};