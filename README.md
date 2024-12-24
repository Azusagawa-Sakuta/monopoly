# Monopoly

Actions build for Linux and MacOS disabled.  

[![Qt Build Status](https://github.com/Azusagawa-Sakuta/monopoly/actions/workflows/main.yml/badge.svg)](https://github.com/Azusagawa-Sakuta/monopoly/actions)   
[![Shell-Cli Build Status](https://github.com/Azusagawa-Sakuta/monopoly/actions/workflows/shell-cli.yml/badge.svg)](https://github.com/Azusagawa-Sakuta/monopoly/actions)   

[Click here to build QT!](https://github.com/Azusagawa-Sakuta/monopoly/actions/workflows/main.yml)  
[Click here to build Shell-Cli!](https://github.com/Azusagawa-Sakuta/monopoly/actions/workflows/shell-cli.yml)  

## Commandline client
Windows commandline client designed to test backend interface. To build:  
```
cd backend/shell-cli/
mkdir build
cd build
g++ ../client.cpp ../../game.cpp ../../utils.cpp ../../constant.cpp
```