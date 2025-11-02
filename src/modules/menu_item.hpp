#include <iostream>
#include <iomanip>
#include <string>
#include "yaml-cpp/yaml.h"

class MenuItem  {
	public: int index_;
	public: std::string title_;
	public: std::string server_;
	public: std::string username_;
	public: std::string port_; 

	public: std::string ToString() const {
		std::string result = std::to_string(index_ + 1) + "|" + title_ + "|" + username_ + "@" + server_;
		if(port_ != "22") {
			result += ":" + port_;
		}
		
		return result;
	}

	std::string GetCommand(std::string app = "ssh") const {
		std::string result = app + " ";

		if (!username_.empty()) {
			result += username_ + "@";
		}

		result += server_;
		if (port_ != "22") {
			result += " -p " + port_;
		}
		return result;
	}
};