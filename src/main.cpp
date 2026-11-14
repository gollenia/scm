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

#include "modules/errors.hpp"
#include "modules/menu_items.hpp"
#include "connection_menu.hpp"
#include "connection_editor.hpp"


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

	YAML::Node config(YAML::NodeType::Map);
	try {
		if (std::filesystem::exists(name)) config = YAML::LoadFile(name);
		if (config.IsNull()) config = YAML::Node(YAML::NodeType::Map);
		if (!config.IsMap() || (config["connections"] && !config["connections"].IsSequence())) {
			std::cerr << "Invalid configuration: expected a map with a connections list.\n";
			return EXIT_FAILURE;
		}
	} catch (const std::exception& e) {
		std::cerr << "Could not load configuration: " << e.what() << '\n';
		return EXIT_FAILURE;
	}
    
	int selected{0};
	bool openConnection{false};

	Errors* errors = new Errors();
	MenuItems items = MenuItems::fromYAMLNode(config["connections"], errors);
	items.filter(searchQuery); // Initial filter
	auto screen = ftxui::ScreenInteractive::Fullscreen();
	std::vector<std::string> menuEntries = items.toStrings();
	if (menuEntries.empty()) menuEntries.push_back("1|No connections| ");
	
	auto menu = ConnectionMenu(&menuEntries, &selected, &items);
	ConnectionEditor editor(config, name, [&] {
		items = MenuItems::fromYAMLNode(config["connections"], errors);
		items.filter(searchQuery);
		menuEntries = items.toStrings();
		if (menuEntries.empty()) menuEntries.push_back("1|No connections| ");
		selected = 0;
	});

	
	
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
			  text("F4: delete | F5: new | F6: edit | F3: quit")
			  })
		  });
	});

	auto component = CatchEvent(renderer, [&screen, &searchQuery, &items, &menuEntries, &openConnection, &selected, &editor](Event event) {

		auto exit_now = [&screen]() { screen.ExitLoopClosure()(); };
		if (event == Event::F4) {
			if (auto* item = items.find(selected)) editor.confirm_delete(*item);
			return true;
		}
		if (event == Event::F5) { editor.open(nullptr); return true; }
		if (event == Event::F6) {
			if (auto* item = items.find(selected)) editor.open(item);
			return true;
		}

		if (event == Event::Return) {
			if (!items.find(selected)) return true;
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
			if (menuEntries.empty()) menuEntries.push_back("1|No connections| ");
			selected = 0;
			return true;
		}

		if(event.is_character()) {
			searchQuery += event.character();
			items.filter(searchQuery); // Filter aktualisieren
			menuEntries = items.toStrings(); // 🔥 Menü-Liste aktualisieren
			if (menuEntries.empty()) menuEntries.push_back("1|No connections| ");
			selected = 0;
			return true;
		}
		return false;
	});
	
	screen.Loop(Modal(component, editor.component(), editor.visible()));

	if(openConnection == false) {
		return EXIT_SUCCESS;
	}
	
	auto* item = items.find(selected);
	if (!item) return EXIT_SUCCESS;
	std::vector<std::string> arguments = {"ssh"};
	if (!item->username_.empty()) {
		arguments.push_back("-l");
		arguments.push_back(item->username_);
	}
	if (item->port_ != "22") {
		arguments.push_back("-p");
		arguments.push_back(item->port_);
	}
	arguments.push_back("--");
	arguments.push_back(item->server_);
	std::vector<char*> argv_ssh;
	for (auto& argument : arguments) argv_ssh.push_back(argument.data());
	argv_ssh.push_back(nullptr);
	execvp("ssh", argv_ssh.data());
	std::perror("Could not start ssh");
	return EXIT_FAILURE;
}
