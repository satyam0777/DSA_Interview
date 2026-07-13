# Makefile - build & run any example with one command.
#
# USAGE EXAMPLES:
#   make run FILE=01_basics/01_class_and_objects.cpp
#   make run FILE=07_real_world_project/food_delivery_system.cpp
#   make debug FILE=07_real_world_project/food_delivery_system.cpp   # builds with -g for gdb
#   make all                                                          # compiles every .cpp file (syntax check)
#   make clean                                                        # remove build artifacts

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DEBUGFLAGS := -std=c++17 -g -O0
BUILD_DIR := build

# All .cpp files in the project, found automatically.
SOURCES := $(shell find . -name "*.cpp")

.PHONY: run debug all clean

run:
ifndef FILE
	$(error Usage: make run FILE=path/to/file.cpp)
endif
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(FILE) -o $(BUILD_DIR)/$(notdir $(basename $(FILE)))
	@echo "----- Running $(FILE) -----"
	@./$(BUILD_DIR)/$(notdir $(basename $(FILE)))

debug:
ifndef FILE
	$(error Usage: make debug FILE=path/to/file.cpp)
endif
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(DEBUGFLAGS) $(FILE) -o $(BUILD_DIR)/$(notdir $(basename $(FILE)))_debug
	@echo "Built with debug symbols. Now run:"
	@echo "  gdb ./$(BUILD_DIR)/$(notdir $(basename $(FILE)))_debug"

# Compiles EVERY .cpp file in the repo, just to catch errors quickly (no run).
all:
	@mkdir -p $(BUILD_DIR)
	@for f in $(SOURCES); do \
		echo "Compiling $$f ..."; \
		$(CXX) $(CXXFLAGS) "$$f" -o $(BUILD_DIR)/$$(basename "$${f%.cpp}") || exit 1; \
	done
	@echo "All files compiled successfully."

clean:
	rm -rf $(BUILD_DIR)
