#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class StringHelpers {

	public: static std::string join(std::vector<std::string> array, char delimiter) {
		std::string result{""};
		
		for(std::uint8_t i; i < array.size(); i++) {
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
};