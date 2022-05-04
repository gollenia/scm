#include "ftxui/component/component.hpp"           // for Menu

#include "ftxui/component/animation.hpp"  // for ElasticOut, Linear
#include "ftxui/component/component.hpp"  // for Menu, Horizontal, Renderer, Vertical
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for MenuOption, MenuEntryOption, AnimatedColorOption, 


using namespace ftxui;

Component ConnectionMenu(std::vector<MenuItem> items, int* selected) {
	std::vector<std::string> entries = MenuItem::getStringsFromMenuItems(items);
	auto option = MenuOption::Vertical();
	option.entries.transform = [](EntryState state) {
		state.label = (state.active ? "  " : "  ") + state.label;
		Element e = hbox({
		text(state.label) | flex,
		text("jji") | flex,
		text("fddgkl"),
		});
		if (state.focused)
		e = e | bgcolor(Color::Blue);
		if (state.active)
		e = e | bold;
		return e;
	};
  	return Menu(&entries, selected, option);
}