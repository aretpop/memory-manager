# Detect OS
ifeq ($(OS),Windows_NT)
    RM = del /F /Q
    RMDIR = rmdir /S /Q
    MKDIR = mkdir
    EXE = .exe
    PATHSEP = \\
else
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p
    EXE =
    PATHSEP = /
endif

CXX = g++
CXXFLAGS = -Wall -std=c++11 -I.

# Create object directories
ifeq ($(OS),Windows_NT)
    $(shell if not exist obj\common $(MKDIR) obj\common)
    $(shell if not exist obj\multi $(MKDIR) obj\multi)
    $(shell if not exist obj\single $(MKDIR) obj\single)
else
    $(shell $(MKDIR) obj/common obj/multi obj/single)
endif

# Common source files
COMMON_SRCS = common/tlb.cpp common/memory_manager.cpp
MULTI_SRCS = $(COMMON_SRCS) multi/iomulti.cpp multi/taskmulti.cpp
SINGLE_SRCS = $(COMMON_SRCS) single/io.cpp single/task.cpp

# Object files with paths
MULTI_OBJS = $(MULTI_SRCS:%.cpp=obj/%.o)
SINGLE_OBJS = $(SINGLE_SRCS:%.cpp=obj/%.o)
TEST_OBJ = obj/test.o

# Executables
MULTI_EXE = multilevel_pagetable$(EXE)
SINGLE_EXE = single_pagetable$(EXE)
TEST_EXE = test$(EXE)

# Default target (multilevel page table)
default: $(MULTI_EXE)

# Multilevel page table
$(MULTI_EXE): $(MULTI_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Single level page table
single: $(SINGLE_EXE)

$(SINGLE_EXE): $(SINGLE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Test trace generator
$(TEST_EXE): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Pattern rule for object files
obj/%.o: %.cpp
ifeq ($(OS),Windows_NT)
	@if not exist $(dir $@) $(MKDIR) $(dir $@)
else
	@$(MKDIR) $(dir $@)
endif
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generate new trace file
trace: $(TEST_EXE)
	@echo "Generating new trace file..."
ifeq ($(OS),Windows_NT)
	@$(TEST_EXE)
else
	@./$(TEST_EXE)
endif

# Clean up
clean:
ifeq ($(OS),Windows_NT)
	@if exist obj rmdir /S /Q obj
	@if exist $(MULTI_EXE) del /F /Q $(MULTI_EXE)
	@if exist $(SINGLE_EXE) del /F /Q $(SINGLE_EXE)
	@if exist $(TEST_EXE) del /F /Q $(TEST_EXE)
else
	$(RMDIR) obj
	$(RM) $(MULTI_EXE) $(SINGLE_EXE) $(TEST_EXE)
endif

# Help target
help:
	@echo "Available targets:"
	@echo "  default          - Build multilevel page table implementation (default)"
	@echo "  single           - Build single-level page table implementation"
	@echo "  trace            - Generate a new trace file (default: 1M lines, 10 tasks)"
	@echo "  clean            - Remove all compiled files"
	@echo ""
	@echo "Usage examples:"
	@echo "  make             - Build multilevel page table"
	@echo "  make single      - Build single-level page table"
	@echo "  make trace       - Generate trace file with default parameters"
	@echo "  ./test 1000 5    - Generate trace file with 1000 lines and 5 tasks"

.PHONY: default single clean trace help
