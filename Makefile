CXXFLAGS:=-std=c++14 -g -Wall -Wno-parentheses -Wextra -Winline -fsanitize=address,undefined
OFILES:=$(subst .cpp,,$(wildcard *.cpp))
.PHONY: all
all: $(OFILES)
