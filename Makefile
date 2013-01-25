CXX = g++
CXXFLAGS = -Wall -pthread -I./src/jsoncpp
OBJ = src/pinned.o src/jsoncpp/jsoncpp.o

all: pinned

pinned: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

debug: CXXFLAGS += -g3 -DDEBUG
debug: pinned

%.o: %.c
	$(CXX) $(CXXFLAGS) $<

clean:
	rm -rf src/*.o
	rm -rf src/jsoncpp/*.o
	rm -rf pinned
