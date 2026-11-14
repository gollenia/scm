# SSH Connection manager

SCM is a small terminal-based SSH connection manager for quickly connecting to servers with different usernames and ports. It uses the system's `ssh` command and stores connections in a YAML file.

![Screenshot](https://gollenia.at/wp-content/uploads/2022/05/scm.jpg)

## Features

- Case-insensitive search by title, server, username, or port (starting at two characters)
- Create, edit, and delete connections in the terminal
- Optional color per connection using a hexadecimal RGB value

### Planned

- Help text (F1)
- Command-line options (config file path, ...)
- Icons (Nerd Font)

## Build

To build scm, you need CMake, Git, and a C++20-compatible compiler. On Ubuntu, install them with:

```shellsession
$ sudo apt-get install build-essential cmake git
```

Then, clone the repository

```shellsession
$ cd
$ git clone https://github.com/gollenia/scm
```

CMake downloads [FTXUI](https://github.com/ArthurSonzogni/FTXUI) and [yaml-cpp](https://github.com/jbeder/yaml-cpp) when configuring the project for the first time, so you need an internet connection:

```shellsession
$ cd scm
$ cmake -S . -B build
$ cmake --build build
```

Run the executable with `./build/scm`. An SSH client must be available as `ssh` on your `PATH` to open connections.

## Usage

Type in the connection list to search; filtering starts at two characters. Use Backspace to remove search characters.

| Key | Action |
| --- | --- |
| Arrow keys | Select a connection |
| Enter | Connect to the selected server |
| F3 or Ctrl+C | Quit |
| F4 | Delete the selected connection after confirmation |
| F5 | Create a connection |
| F6 | Edit the selected connection |

In the editor, use Tab and Shift+Tab to move between fields, F2 to save, and Escape to cancel. In the deletion prompt, press Enter to confirm or Escape to cancel.

## Configure

Connections are stored in `~/.scm.yaml`. You can create and edit them in SCM, or create the file yourself:

```yaml
connections:
  - title: "DigitalOcean"
    server: "mydroplet.example.net"
    username: root
    port: 4022
    color: "#FF9900"
  - title: "Home Server"
    server: "192.168.1.5"
    username: john
```

Only `server` is required. `title` defaults to the server name, `username` defaults to the SSH client's choice, and `port` defaults to 22. In the editor, ports must be between 1 and 65535, and `|` is not supported in the title, server, or username.

The optional `color` sets the connection title's text color. Use a quoted six-digit hex value (`"#RRGGBB"`); quote it because `#` starts a YAML comment. Invalid colors are ignored and reported in the interface. Color rendering depends on terminal support.

Saving or deleting rewrites `~/.scm.yaml`: YAML comments and formatting are not preserved, but other configuration fields are retained.

> 👉 If `scm` is on your `PATH`, add `"\es": "scm\n"` to your `.inputrc` to start it with <kbd>ALT</kbd>+<kbd>s</kbd>.

## FAQ

**Can I store passwords?** No. SCM delegates authentication to the SSH client; use SSH keys or the client's normal authentication prompts.

## Changelog

### Unreleased

- Add connection creation, editing, and deletion with confirmation.
- Support optional connection colors.

### 0.0.4
- stupid bugfix: not starting ssh
- small bugfix: ignore uppercase letters in search

### 0.0.3
- removed shortcuts
- added search function

### 0.0.2
- Ctrl-C now works as expected
- Better error handeling with corrupt YAML files

### 0.0.1 
- initial release
