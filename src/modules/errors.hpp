#include <string>
#include <vector>

class Errors {
	public: 
		std::vector<std::string> messages;
	
		int add(std::string message) {
			messages.push_back(message);
			return messages.size();
		}

		std::string pop() {
			return messages[messages.size()-1];
		}

		bool existing() {
			return messages.size();
		}
};