#CXX ?= g++
CXX ?= clang

doc-user: doc-user.cpp doc-library.hh
	$(CXX) --std=c++17 -lstdc++ -g -o doc-user doc-user.cpp

.PHONEY: test
test: doc-user
	doc-user
