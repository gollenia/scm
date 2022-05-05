# SSH Connection manager

This is a small and (yet) not functional SSH connection manager. You can use it to quickly connect to different servers with different usernames and ports. Every connection can have a letter assigned for quick access. 

## Build

To build ssc, you need to install `yaml-cpp` first. On Ubuntu, type

```shellsession
$ sudo apt-get install libyaml-cpp-dev
```

Then, clone the repository

```shellsession
$ cd
$ git clone 
```

## Configure

Add a `~/.ssc.yaml` file and add connections like so:

```
connections:
  - name: "DigitalOcean"
    address: "mydroplet.example.net"
    user: root
	port: 4022
    shortcut: d
  - name: "Home Server"
    address: "192.168.1.5"
    port: 4022
    user: john
    shortcut: k
```

## FAQ

  - Can I store Passwords? 
  No. I assume you use SSH keys, which is much safer