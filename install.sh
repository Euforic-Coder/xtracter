#!/bin/bash

if command -v cmake
then
	cmake CMakeLists.txt
fi

if command -v make
then
	sudo make install
fi

sudo mkdir -p /etc/xtracter
sudo cp english.dic /etc/xtracter
