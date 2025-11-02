#include <iostream>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "ftxui/component/component.hpp"           // for Menu
#include "ftxui/component/component_options.hpp"   // for MenuOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "yaml-cpp/yaml.h"
#include "string_helpers.hpp"
#include "connection_menu.hpp"

#include "modules/errors.hpp"
#include "modules/menu_items.hpp"


int main(int argc, char *argv[]) {
	

	// TODO: Put this stuff in seperate class that returns a YAML::Node or false
	const char *homedir;

	if ((homedir = getenv("HOME")) == NULL) {
		homedir = getpwuid(getuid())->pw_dir;
	}

	// later we add a command line argument here
	std::string home(homedir);
	std::string name = home + "/.scm.yaml";
	std::string title = "SSH Connection Manager";
	std::string searchQuery = "";

	if(!std::filesystem::exists(name)) {
		std::cout << "No configuration file found. Please create a .scm.yaml file in your home directory.\n" << std::endl;
		return EXIT_SUCCESS;
	}
	
	YAML::Node config = YAML::LoadFile(name);

	if(config["connections"].size() == 0) {
		std::cout << "No connections found.\n" << std::endl;
		return EXIT_SUCCESS;
	}
    
	int selected{0};
	bool openConnection{false};

	Errors* errors = new Errors();
	MenuItems items = MenuItems::fromYAMLNode(config["connections"], errors);
	items.filter(searchQuery); // Initial filter
	auto screen = ftxui::ScreenInteractive::Fullscreen();
	std::vector<std::string> menuEntries = items.toStrings();
	
	auto menu = ConnectionMenu(&menuEntries, &selected);

	
	
	auto renderer = Renderer(menu, [&menu, &searchQuery, &errors]() {

		Element bottomMessage = errors->has_errors() 
			? bgcolor(Color::Red, text(" " + errors->pop() + " ")) 
			: text(" Make your choice");

  		return vbox({
			  text("Search: " + searchQuery) | borderLight,
			  text(""),
			  hbox(
				  text(" #   Key Name") | flex_grow,
				  text("Connection ")
			  ),
			  separator(),
			  menu->Render(),
			  filler(),
			  hbox({
				  bottomMessage,
				  text("") | flex_grow,
				  text("Press C-c or F3 to quit")
			  })
		  });
	});

	auto component = CatchEvent(renderer, [&screen, &searchQuery, &items, &menuEntries, &openConnection](Event event) {

		auto exit_now = [&screen]() { screen.ExitLoopClosure()(); };

		if (event == Event::Return) {
			exit_now();
			openConnection = true;
			return true;
		}

		if (event == Event::F3) {
			exit_now();
			return true;
		}

		if(event == Event::Backspace) {
			if (searchQuery.empty()) {
				return false;
			}
			
			searchQuery.pop_back();
			items.filter(searchQuery); // Filter aktualisieren
			menuEntries = items.toStrings(); // 🔥 Menü-Liste aktualisiere
			return true;
		}

		if(event.is_character()) {
			searchQuery += event.character();
			items.filter(searchQuery); // Filter aktualisieren
			menuEntries = items.toStrings(); // 🔥 Menü-Liste aktualisieren
			return true;
		}
		return false;
	});
	
	screen.Loop(component);

	if(openConnection == false) {
		return EXIT_SUCCESS;
	}
	
	std::string command = items.find(selected)->GetCommand();
		

	return EXIT_SUCCESS;
}
