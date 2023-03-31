#!/bin/bash

sudo cmake CMakeLists.txt
sudo make install
sudo mkdir -p etc/xtracter
sudo cp english.dic /etc/xtracter
