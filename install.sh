#!/bin/bash

sudo cmake CMakeLists.txt
sudo make install
sudo mkdir -p etc/xtracter
sudo cp english.dic /etc/xtracter
sudo cp dutch.dic /etc/xtracter
sudo cp spanish.dic /etc/xtracter
sudo cp french.dic /etc/xtracter
sudo cp german.dic /etc/xtracter
