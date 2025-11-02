#include <string>
#include <vector>

class Errors {
	private: 
		std::vector<std::string> messages;

	public:
	
		void push(const std::string& message) {
			messages.push_back(message);
		}

		std::string pop() {
			if (messages.empty()) return "No errors.";
			std::string msg = messages.back();
			messages.pop_back();
			return msg;
		}

		int count() {
			return messages.size();
		}

		bool has_errors() {
			return !messages.empty();
		}
};