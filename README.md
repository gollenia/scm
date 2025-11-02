# SSH Connection manager

This is a small and (yet) dirty SSH connection manager. You can use it to quickly connect to different servers with different usernames and ports. There already exists [nccm](https://github.com/flyingrhinonz/nccm), but it was not exactly what I was looking for.

![Screenshot](https://gollenia.at/wp-content/uploads/2022/05/scm.jpg)

## Features
### Finished
- Every connection can have a shortcut assigned for quick access.
- Connections can be accessed by their number (only 1-9)

### Planned
- Interactive connection editor (F2)
- Help text (F1)
- commandline options (shortcut, config file path, ...)
- colors, icons (nerdfont)

## Build

To build ssc, you need to install `yaml-cpp` first. On Ubuntu, type

```shellsession
$ sudo apt-get install libyaml-cpp-dev
```

Then, clone the repository

```shellsession
$ cd
$ git clone https://github.com/gollenia/scm
```

Please be aware that the Connection manager uses [FTXUI](https://github.com/ArthurSonzogni/FTXUI), a Terminal UI comparable to ncurses. So when running cmake, make sure you have a working internet connection:

```shellsession
$ cd scm
$ cmake .
$ make
```

## Configure

Add a `~/.scm.yaml` file and add connections like so:

```
connections:
  - title: "DigitalOcean"
    server: "mydroplet.example.net"
    username: root
	port: 4022
  - title: "Home Server"
    server: "192.168.1.5"
    username: john
```

> 👉 Add `"\es": "scm\n"` to your `.inputrc` and you can call scm with <kbd>ALT</kbd>+<kbd>s</kbd>. So you can connect to your home server, for example with <kbd>ALT</kbd>+<kbd>s</kbd> <kbd>k</kbd>

## FAQ

  - Can I store Passwords? 
  No. I assume you use SSH keys, which is much safer

## Changelog

### 0.0.3
- removed shortcuts
- added search function

### 0.0.2
- Ctrl-C now works as expected
- Better error handeling with corrupt YAML files

### 0.0.1 
- initial release
