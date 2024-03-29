src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
# obj = $(src:%.cpp=%.o)
target = main
CC = g++

$(target): $(obj)
	$(CC) $(obj) -o $(target) -g

%.o: %.cpp
	$(CC) -c $< -o $@ -g

.PHONY: clean
clean:
	rm -rf $(obj) $(target)