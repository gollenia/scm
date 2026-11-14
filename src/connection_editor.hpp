#pragma once

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <functional>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "yaml-cpp/yaml.h"

class ConnectionEditor {
 public:
  ConnectionEditor(YAML::Node& config, const std::string& path, std::function<void()> on_save)
      : config_(config), path_(path), on_save_(std::move(on_save)) {
    using namespace ftxui;
    auto title = Input(&title_, "optional");
    auto server = Input(&server_, "required");
    auto username = Input(&username_, "optional");
    auto port = Input(&port_, "22");
    auto color = Input(&color_, "#RRGGBB (optional)");
    auto fields = Container::Vertical({title, server, username, port, color});
    fields_ = fields;
    first_field_ = title;
    component_ = CatchEvent(Renderer(fields, [this, title, server, username, port, color] {
      if (deleting_) {
        return vbox({
          text("Delete connection?") | bold,
          separator(),
          text(delete_label_),
          text(error_) | ftxui::color(Color::Red),
          separator(),
          text("Enter: delete  |  Esc: cancel")
        }) | border;
      }
      return vbox({
        text(editing_ ? "Edit connection" : "New connection") | bold,
        separator(),
        hbox(text("Title:    "), title->Render()),
        hbox(text("Server:   "), server->Render()),
        hbox(text("Username: "), username->Render()),
        hbox(text("Port:     "), port->Render()),
        hbox(text("Color:    "), color->Render()),
        text(error_) | ftxui::color(Color::Red),
        separator(),
        text("Tab / Shift+Tab: next / previous  |  F2: save  |  Esc: cancel")
      }) | border;
    }), [this](Event event) {
      if (event == Event::Escape) { visible_ = false; return true; }
      if (deleting_) {
        if (event == Event::Return) remove();
        return true;
      }
      if (event == Event::F2) { save(); return true; }
      return false;
    });
  }

  void open(const MenuItem* item) {
    deleting_ = false;
    editing_ = item != nullptr;
    index_ = item ? item->yaml_index_ : 0;
    title_ = item ? item->title_ : "";
    server_ = item ? item->server_ : "";
    username_ = item ? item->username_ : "";
    port_ = item && item->port_ != "22" ? item->port_ : "";
    color_.clear();
    if (item && config_["connections"][index_]["color"].IsScalar())
      color_ = config_["connections"][index_]["color"].Scalar();
    error_.clear();
    fields_->SetActiveChild(first_field_);
    visible_ = true;
  }

  void confirm_delete(const MenuItem& item) {
    deleting_ = true;
    index_ = item.yaml_index_;
    delete_label_ = item.title_ + " (" + item.server_ + ")";
    error_.clear();
    visible_ = true;
  }

  ftxui::Component component() const { return component_; }
  bool* visible() { return &visible_; }

 private:
  void remove() {
    try {
      YAML::Node updated = YAML::Clone(config_);
      updated["connections"].remove(index_);
      if (!persist(updated)) return;
      visible_ = false;
      on_save_();
    } catch (const std::exception& e) {
      error_ = std::string("Could not delete: ") + e.what();
    }
  }

  bool persist(const YAML::Node& updated) {
    YAML::Emitter output;
    output << updated;
    if (!output.good()) { error_ = "Could not serialize configuration."; return false; }
    if (!write_file(std::string(output.c_str()) + "\n")) return false;
    config_ = updated;
    return true;
  }

  void save() {
    if (server_.empty() || server_.find_first_not_of(" \t\r\n") == std::string::npos) {
      error_ = "Server is required."; return;
    }
    if (title_.find('|') != std::string::npos || server_.find('|') != std::string::npos ||
        username_.find('|') != std::string::npos) {
      error_ = "The character | is not supported in these fields."; return;
    }
    if (!port_.empty()) {
      if (port_.size() > 5 || !std::all_of(port_.begin(), port_.end(), [](unsigned char c) { return std::isdigit(c); }) ||
          std::stoi(port_) < 1 || std::stoi(port_) > 65535) {
        error_ = "Port must be between 1 and 65535."; return;
      }
    }
    if (!color_.empty() && (color_.size() != 7 || color_[0] != '#' ||
        !std::all_of(color_.begin() + 1, color_.end(), [](unsigned char c) { return std::isxdigit(c); }))) {
      error_ = "Color must be a hex value in the form #RRGGBB."; return;
    }
    try {
      YAML::Node updated = YAML::Clone(config_);
      if (!updated["connections"]) updated["connections"] = YAML::Node(YAML::NodeType::Sequence);
      if (!editing_) updated["connections"].push_back(YAML::Node(YAML::NodeType::Map));
      YAML::Node entry = updated["connections"][editing_ ? index_ : updated["connections"].size() - 1];
      entry["server"] = server_;
      set_optional(entry, "title", title_);
      set_optional(entry, "username", username_);
      set_optional(entry, "port", port_);
      set_optional(entry, "color", color_);
      if (!persist(updated)) return;
      visible_ = false;
      on_save_();
    } catch (const std::exception& e) {
      error_ = std::string("Could not save: ") + e.what();
    }
  }

  static void set_optional(YAML::Node& entry, const char* key, const std::string& value) {
    if (value.empty()) entry.remove(key);
    else entry[key] = value;
  }

  bool write_file(const std::string& data) {
    std::string temp = path_ + ".tmp.XXXXXX";
    std::vector<char> name(temp.begin(), temp.end());
    name.push_back('\0');
    int fd = mkstemp(name.data());
    if (fd < 0) { error_ = "Could not create temporary configuration file."; return false; }
    struct stat existing;
    if (stat(path_.c_str(), &existing) == 0 && fchmod(fd, existing.st_mode & 0777) != 0) {
      close(fd); unlink(name.data()); error_ = "Could not set configuration permissions."; return false;
    }
    FILE* file = fdopen(fd, "w");
    if (!file) { close(fd); unlink(name.data()); error_ = "Could not open temporary file."; return false; }
    bool ok = fwrite(data.data(), 1, data.size(), file) == data.size();
    if (fclose(file) != 0) ok = false;
    if (!ok || rename(name.data(), path_.c_str()) != 0) {
      unlink(name.data()); error_ = "Could not write configuration file."; return false;
    }
    return true;
  }

  YAML::Node& config_;
  std::string path_;
  std::function<void()> on_save_;
  ftxui::Component component_;
  ftxui::Component fields_, first_field_;
  bool visible_ = false;
  bool editing_ = false;
  bool deleting_ = false;
  std::size_t index_ = 0;
  std::string title_, server_, username_, port_, color_, error_, delete_label_;
};
