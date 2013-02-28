CXX = clang++
CXXFLAGS = -Wall -Isrc/jsoncpp
LDFLAGS = -pthread $(shell curl-config --libs)
OBJ = src/pinned.o src/request.o src/config.o src/jsoncpp/jsoncpp.o src/libinet/uri.o src/libinet/sockets.o src/libinet/http.o
PREFIX = /usr/local

all: pinned

pinned: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

debug: CXXFLAGS += -g3 -DDEBUG
debug: pinned

%.o: %.c
	$(CXX) $(CXXFLAGS) $<

clean:
	rm -rf src/*.o
	rm -rf src/jsoncpp/*.o
	rm -rf pinned

install: pinned
	chmod a+x ./pinned
	install ./pinned $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/pinned
	rm -f ~/.pinboard_token
