CXX = clang++
CXXFLAGS = -Wall -pthread
OBJ = src/pinned.o

all: pinned

pinned: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

debug: CXXFLAGS += -g3 -DDEBUG
debug: pinned

%.o: %.c
	$(CXX) $(CXXFLAGS) $<

clean:
	rm -rf src/*.o
	rm -rf pinned
