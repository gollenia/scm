#include <iostream>
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
	YAML::Node config = YAML::LoadFile(name);

	if(config["connections"].size() == 0) {
		std::cout << "No connections found.\n" << std::endl;
		return EXIT_SUCCESS;
	}
    
	int selected{0};
	bool openConnection{false};

	Errors* errors = new Errors;
	MenuItems items = MenuItems::fromYAMLNode(config["connections"], errors);

	auto screen = ftxui::ScreenInteractive::Fullscreen();
	std::vector<std::string> menuEntries = items.toStrings();
	
	auto menu = ConnectionMenu(&menuEntries, &selected);

	Element bottomMessage = errors->existing() ? bgcolor(Color::Red, text(" " + errors->pop() + " ")) : text(" Make your choice");
	

	auto renderer = Renderer(menu, [&] {
  		return vbox({
			  text("SSH Connection Manager 0.0.2") | borderLight,
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

	

	// here, we capture the short keys and quit
	auto component = CatchEvent(renderer, [&](Event event) {
		if (event == Event::Return) {	
				screen.ExitLoopClosure()();
				openConnection = true;
				return true;
		}
		// doesn't work, yet
		if (isdigit(event.character().at(0))) {
			selected = std::stoi(event.character()) + 1;
			openConnection = true;
			screen.ExitLoopClosure()();
			return true;
		}

		if (event == Event::F3) {	
				screen.ExitLoopClosure()();
				return true;
		}
		MenuItem* item = items.findByShortcut(event.character());
		if(item) {
			screen.ExitLoopClosure()();
			selected = item->index;
			openConnection = true;
			return true;
		};
		return false;
	});
	
	// This really shout be in a sep. class
	screen.Loop(component);

	if(openConnection == false) {
		return EXIT_SUCCESS;
	}
	
	std::string command = items.find(selected)->getCommand();
	int commandLength = command.size();
 
    char *cCommand = new char[commandLength + 1];
	std::copy(command.begin(), command.end(), cCommand);
    cCommand[commandLength] = '\0';
	system(cCommand);
	
	return EXIT_SUCCESS;
}
