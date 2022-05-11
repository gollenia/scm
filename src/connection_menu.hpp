#include "ftxui/component/component.hpp"           // for Menu

#include "ftxui/component/animation.hpp"  // for ElasticOut, Linear
#include "ftxui/component/component.hpp"  // for Menu, Horizontal, Renderer, Vertical
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for MenuOption, MenuEntryOption, AnimatedColorOption, 


using namespace ftxui;

Component ConnectionMenu(std::vector<std::string>* entries, int* selected) {
	auto option = MenuOption::Vertical();
	option.entries.transform = [](EntryState state) {
		std::vector<std::string> labelArray = StringHelpers::split(state.label, '|');
		std::string numberColumn = (std::stoi(labelArray[0]) < 10 ? labelArray[0] + "  " : labelArray[0] + " ");
		Element e = hbox({
			text(" " + numberColumn + " " + labelArray[1] + "   " + labelArray[2]) | flex,
			text(labelArray[3] + " "),
		});
		if (state.focused)
		e = e | bgcolor(Color::Blue);
		if (state.active)
		e = e | bold;
		return e;
	};
  	return Menu(entries, selected, option);
}