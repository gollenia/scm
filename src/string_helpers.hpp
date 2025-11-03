#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

class StringHelpers {

	public: static std::string join(std::vector<std::string> array, char delimiter) {
		std::string result{""};
		
		for(std::uint8_t i = 0; i < array.size(); i++) {
			result += std::string(1, delimiter) + array[i];
			
		}
		return result;
	}

	public: static std::vector<std::string> split (const std::string &s, char delimiter) {
		std::vector<std::string> result;
		std::stringstream stream(s);
		std::string item;

		while (getline(stream, item, delimiter)) {
			result.push_back (item);
		}

		return result;
	}

	public: static std::string to_lower(const std::string& str) {
		std::string lowerStr = str;
		std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
			[](unsigned char c){ return std::tolower(c); });
		return lowerStr;
	}
};