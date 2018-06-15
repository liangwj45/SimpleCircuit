CC = g++
CFLAGS = -std=c++11 -I $(include)

bin = bin
build = build
include = include
src = src
target = $(bin)/circuit

objects := $(patsubst $(src)/%.cpp, $(build)/%.obj,\
		$(wildcard $(src)/*.cpp))

$(target) : $(objects)
	@if not exist $(bin) mkdir $(bin)
	$(CC) $(objects) $(CFLAGS) -o $@

$(build)/%.obj : $(src)/%.cpp
	@if not exist $(build) mkdir $(build)
	$(CC) $(CFLAGS) -c -o $@ $<

run :
	$(target)

clear :
	rm $(target)
	rmdir $(build)