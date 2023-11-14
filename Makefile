# Ref: https://stackoverflow.com/questions/30573481/how-to-write-a-makefile-with-separate-source-and-header-directories

################################################################################
#### Variables and settings
################################################################################
EXEC := snake

SRC_DIR := src
INCLUDE_DIR := inc
BUILD_DIR = build
BIN_DIR = bin

CLANG_FORMAT_CMD := clang-format
ifdef WSL_DISTRO_NAME	# WSL default settings
	CLANG_FORMAT_CMD := clang-format.exe
endif


CPPCHECK_CMD := cppcheck
CPPCHECKFLAGS := --enable=style,warning,missingInclude --suppressions-list=./.suppress.cppcheck


# Sources (searches recursively inside the source directory)
SRCS := $(sort $(shell find $(SRC_DIR) \( -name '*.cpp' ! -name '*UI.cpp' ! -name '*_utest.cpp' \)))

# Includes
INCLUDES := -I$(INCLUDE_DIR)

# C preprocessor settings
CPPFLAGS = -MMD -MP

# C++ compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -g
WARNINGS = -Wall -Wpedantic -Wextra

# Linker flags
LDFLAGS =

# Libraries to link
LDLIBS =

#### Select GUI engine ###
ifeq ($(gui), 1)
	SRCS += $(SRC_DIR)/GUI.cpp
	CPPFLAGS += -DBUILD_GUI=1
	LDLIBS += -lSDL2 -lSDL2_image -lSDL2_ttf
else
	SRCS += $(SRC_DIR)/TUI.cpp
	LDLIBS += -lncurses
endif

# Objects and dependencies
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

################################################################################
#### Targets
################################################################################
.PHONY: all
all: $(BIN_DIR)/$(EXEC)

# Build executable
$(BIN_DIR)/$(EXEC): $(OBJS)
	@echo "Building executable: $@"
	@mkdir -p $(@D)
	@$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Compile C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $(WARNINGS) -c $< -o $@

# Include automatically generated dependencies
-include $(DEPS)


# Build and run
.PHONY: run
run: all
	@echo "Starting program: $(BIN_DIR)/$(EXEC)"
	@./$(BIN_DIR)/$(EXEC)


# Clean build and bin directories for all platforms
.PHONY: clean
clean:
	@echo "Cleaning $(BUILD_DIR) and $(BIN_DIR) directories"
	@$(RM) -r $(BUILD_DIR)
	@$(RM) -r $(BIN_DIR)

# Run clang-format on source code
.PHONY: format
format:
	@echo "Running clang-format"
	@$(CLANG_FORMAT_CMD) -i $$(find $(SRC_DIR) $(INCLUDE_DIR) -name '*.cpp' -o -name '*.h' -o -name '*.hpp' -o -name '*.inl')

.PHONY: cppcheck
cppcheck:
	@echo "Running cppcheck"
	@mkdir -p $(BUILD_DIR)
	@$(CPPCHECK_CMD) $(CPPCHECKFLAGS) -I$(INCLUDE_DIR) $(SRC_DIR)/*.cpp --cppcheck-build-dir=$(BUILD_DIR)
