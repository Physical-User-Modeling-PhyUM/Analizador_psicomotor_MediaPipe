#!/bin/bash
g++ testpose.cpp -o testpose -std=c++17
g++ testposemanager.cpp -o testposemanager -std=c++17
g++ testposefunctional.cpp -o testposefunctional -std=c++17

./testpose
./testposemanager
./testposefunctional
