CXX = g++
CXXFLAGS = -std=c++11 -Wall
CURSES=-lncurses
TINFO=-ltinfo

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

EXECUTABLE = wordle_client
LIBRARY_NAMES = libncurses5-dev libncursesw5-dev

SOURCES = $(wildcard $(SRC_DIR)/*.cc)
OBJECTS = $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SOURCES))

.PHONY: compile clean run check_library

all: check_library compile

check_library:
	@for lib in $(LIBRARY_NAMES); do \
		echo "Checking for $$lib..."; \
		dpkg -l | grep -q $$lib || { \
			echo "$$lib is not installed. Please install it manually:"; \
			echo "sudo apt-get install $$lib"; \
			exit 1; \
		}; \
	done

compile: $(BIN_DIR)/$(EXECUTABLE)

$(BIN_DIR)/$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(CURSES) $(TINFO)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

run: $(BIN_DIR)/$(EXECUTABLE)
	./$(BIN_DIR)/$(EXECUTABLE)
