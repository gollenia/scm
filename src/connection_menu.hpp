#include "ftxui/component/component.hpp"           // for Menu

#include "ftxui/component/animation.hpp"  // for ElasticOut, Linear
#include "ftxui/component/component.hpp"  // for Menu, Horizontal, Renderer, Vertical
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for MenuOption, MenuEntryOption, AnimatedColorOption, 


using namespace ftxui;

Component ConnectionMenu(std::vector<std::string>* entries, int* selected, const MenuItems* items) {
	auto option = MenuOption::Vertical();
	option.entries.transform = [items](EntryState state) {
		std::vector<std::string> labelArray = StringHelpers::split(state.label, '|');
		int index = std::stoi(labelArray[0]) - 1;
		std::string numberColumn = (index < 9 ?  "  " + labelArray[0] : " " + labelArray[0] );
		Element title = text(" " + numberColumn + " " + labelArray[1]);
		if (index >= 0 && static_cast<std::size_t>(index) < items->entries.size() && items->entries[index].color_) {
			auto rgb = *items->entries[index].color_;
			title = title | color(Color::RGB((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff));
		}
		Element e = hbox({
			title | flex,
			text(labelArray[2] + " "),
		});
		if (state.focused)
		e = e | bgcolor(Color::Blue);
		if (state.active)
		e = e | bold;
		return e;
	};
  	return Menu(entries, selected, option);
}
