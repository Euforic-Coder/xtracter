#!/bin/bash

sudo cmake CMakeLists.txt
sudo make install
sudo mkdir -p etc/xtracter
cp english.dic /etc/xtracter
