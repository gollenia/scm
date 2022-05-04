#include <iostream>
#include <string>

#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "ftxui/component/component.hpp"           // for Menu
#include "ftxui/component/component_options.hpp"   // for MenuOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "yaml-cpp/yaml.h"
#include "menu_item.hpp"
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

	auto screen = ScreenInteractive::Fullscreen();
	std::vector<MenuItem> entries;

	// Prepare the menu icons
	for (int i = 0; i < config["connections"].size(); i++) {
		entries.push_back(MenuItem::fromYAMLNode(config["connections"][i]));
	}

	int selected{0};
	
	auto menu = ConnectionMenu(entries, &selected);

	auto renderer = Renderer(menu, [&] {
  		return vbox({
			  text("Welcome to the SSH Connection Manager! So cool!") | border,
			  menu->Render(),
			  filler(),
			  hbox({
				  text("Please make your choice") | flex_grow,
				  text("Press C-c to quit")
			  })
		  });

	});

	// here, we capture the short keys and quit
	auto component = CatchEvent(renderer, [&](Event event) {
		for (int i = 0; i < config["connections"].size(); i++) {
			if (event == Event::Character(config["connections"][i]["key"].as<string>())) {
				screen.ExitLoopClosure()();
				selected = i;
				return true;
			}
		}
		return false;
	});


	screen.Loop(component);
	
	std::cout << "Selected element = " << config["connections"][selected]["name"] << std::endl;
	
	return EXIT_SUCCESS;
}
