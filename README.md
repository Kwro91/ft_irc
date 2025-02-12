# ft_irc

![42](https://img.shields.io/static/v1?label=&labelColor=000000e&logo=42&message=project&color=000000&style=flate)


## Summary
- [About the project](#about-the-project)
- [Installation & Usage](#installation--usage)
- [Team](#team)
- [Links](#links)


## About the project

42 project, creating an Internet Relay Chat (IRC) server functionnal with Irssi client.

## Installation & Usage

### Run
You will need Irssi Client to use this IRC.
You can get it [there](https://irssi.org/download/).

To run the project, you will need to **make**. Then run the project like this :
```bash
./ircserv port password
```

The port must be between 6666 & 6668 included.

Then you can use another terminal window to :
```bash
irssi -c [ip] -p [port] -w [password]
```
If you try it in local on the same machine you can use **127.0.0.1** as IP to connect.

### Commands

#### Main commands
- **/join [name] [password]** to join/create a channel. *This command can be use by everyone.*

- **/leave** to leave the channel. *This command can be use by everyone.*

- **/kick [nickname]** to kick an user from a channel if you're an operator on this channel.

- **/invite [nickname]** to invite someone on the channel. *This command can be use by everyone.*

- **/topic** print the topic of the channel. *This command can be use by everyone.*

- **/topic [text]** to set a topic in the channel.

- **/mode [parameter]** to execute different commands based ont the parameter in the channel.

#### List of parameters for **mode** command:

- **-o [nickname]** make someone an operator or remove someone from operator.
- **-l [number]** set the limit of users per channel. (Max is 20).
- **-i** to make the channel in invite only mode. You can also turn it off by using it again.
- **-k [password]** define a password to enter in the channel and so make it Private. You can also turn it off by using it again.
- **-t** to make */topic [text]* usable by everyone in the channel. You can also turn it off by using it again.

## Team

project made with:

- **afontain**  : [Aurelien Fontaine](https://github.com/AurelienFontaine)

- **npetitpi** : [Gigot Blaster](https://github.com/GigotBlaster)


## Links

This repository is a copy. The original Repo is on [this page](https://github.com/GigotBlaster/IRC-git-/). It may be private.