#include <iostream>
#include <string>

#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "ftxui/component/component.hpp"           // for Menu
#include "ftxui/component/component_options.hpp"   // for MenuOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "yaml-cpp/yaml.h"
#include "string_helpers.hpp"
#include "connection_menu.hpp"


int main(int argc, char *argv[]) {
	using namespace ftxui;
	using namespace std;

	// later we add a command line argument here
	string name = "./test/sample_config.yaml";
	YAML::Node config = YAML::LoadFile(name);

	if(config["connections"].size() == 0) {
		std::cout << "No connections found.\n" << std::endl;
		return EXIT_SUCCESS;
	}
    

	int selected{0};

	std::vector<std::string> menuItems;

	for(std::uint8_t i = 0; i < config["connections"].size(); i++) {
		if(!config["connections"][i]["shortcut"].IsDefined()) continue;
		if(!config["connections"][i]["title"].IsDefined()) continue;
		if(!config["connections"][i]["username"].IsDefined()) continue;
		if(!config["connections"][i]["server"].IsDefined()) continue;
		
		menuItems.push_back(
			std::to_string(i+1) + "|"
			+ config["connections"][i]["shortcut"].as<string>() + "|" 
			+ config["connections"][i]["title"].as<string>() + "|"
			+ config["connections"][i]["username"].as<string>() + "@"
			+ config["connections"][i]["server"].as<string>() + ":"
			+ (config["connections"][i]["port"].IsDefined() ? config["connections"][i]["port"].as<string>() : "22")
		);
	}
	auto screen = ScreenInteractive::Fullscreen();
	auto menu = ConnectionMenu(&menuItems, &selected);

	auto renderer = Renderer(menu, [&] {
  		return vbox({
			  text("SSH Connection Manager") | borderLight,
			  text(""),
			  hbox(
				  text("#   Key Name") | flex_grow,
				  text("Connection")
			  ),
			  separator(),
			  menu->Render(),
			  filler(),
			  hbox({
				  text("Please make your choice") | flex_grow,
				  text("Press C-c or F3 to quit")
			  })
		  });

	});

	// here, we capture the short keys and quit
	auto component = CatchEvent(renderer, [&](Event event) {
		for (int i = 0; i < config["connections"].size(); i++) {
			if (event == Event::Character(config["connections"][i]["shortcut"].as<string>())) {
				screen.ExitLoopClosure()();
				selected = i;
				return true;
			}
			
		}
		if (event == Event::Return || event == Event::F3) {	
				screen.ExitLoopClosure()();
				return true;
		}
		// doesn't work, yet
		if (isdigit(event.character().at(0))) {
			selected = std::stoi(event.character()) + 1;
			screen.ExitLoopClosure()();
			return true;
		}
		return false;
	});


	screen.Loop(component);
	
	std::cout << "Selected element = " << config["connections"][selected]["title"] << std::endl;
	
	return EXIT_SUCCESS;
}
