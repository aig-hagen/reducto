# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
# and to chaselambda (https://makefiletutorial.com/)

.DELETE_ON_ERROR:

#--------------------------------------------------------------------------#
#	Paths to SAT solvers
#--------------------------------------------------------------------------#

# Directories for the source of the SAT Solvers
GLUCOSE_DIR	= sat/glucose-4.2.1
CMSAT_DIR	= sat/cryptominisat-5.11.21

#--------------------------------------------------------------------------#
#	Build Variables
#--------------------------------------------------------------------------#

# Default value for the chosen SAT Solver
SOLVER		?= glucose
TARGET=$(shell basename "`pwd`")_$(SOLVER)

# Path to the SAT solver library (e.g., points to where libcadical.a is)
SATLIBDIR	?=	./sat/$(SOLVER)/build/ 
SATLIBNAME ?= $(SOLVER)

ifeq ($(SOLVER), cryptominisat)
	SATLIBNAME  = cryptominisat5
else ifeq ($(SOLVER), glucose)
	SATLIBDIR	= ./$(GLUCOSE_DIR)/simp
endif

# Directory to store object files, libraries, executables, and dependencies:
BUILD_DIR := ./build/$(SOLVER)

# Paths to the .cpp files
SRC				=	./src/
SRC_LOGIC		=	./src/logic/
SRC_UTIL		=	./src/util/

#Paths to the .h files
INCLUDE			=	./include/
INCLUDE_LOGIC	=	./include/logic/
INCLUDE_UTIL	=	./include/util/

#--------------------------------------------------------------------------#
#	Flags
#--------------------------------------------------------------------------#


CC	=	gcc
CFLAGS	?=	-Wall -std=c11
CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -L$(SATLIBDIR)
LDLIBS = -l$(SATLIBNAME)

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC) -name '*.cpp' -or -name '*.c' -or -name '*.s')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./include will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(INCLUDE) -type d)

ifeq ($(SOLVER), cryptominisat)
	INC_DIRS	+= ./$(CMSAT_DIR)/src
else ifeq ($(SOLVER), glucose)
	INC_DIRS	+= ./$(GLUCOSE_DIR)
endif

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

ifeq ($(SOLVER), cryptominisat)
	CPPFLAGS    += -D SAT_CMSAT
else ifeq ($(SOLVER), glucose)
	CPPFLAGS	+= -D SAT_GLUCOSE
endif

#--------------------------------------------------------------------------#
#	Targets
#--------------------------------------------------------------------------#
.PHONY:	all
all:
	@$(MAKE) cryptominisat
	@$(MAKE) glucose
	@$(MAKE) solo SOLVER=cryptominisat
	@$(MAKE) solo SOLVER=glucose

.PHONY:	cryptominisat
cryptominisat:
	@echo "Compiling CryptoMiniSat..."
	cd $(CMSAT_DIR) && \
	mkdir -p build && cd build && \
	cmake .. && \
	make && \
	sudo make install && \
	sudo ldconfig

.PHONY:	glucose
glucose:
	@echo "Compiling Glucose..."
	cd $(GLUCOSE_DIR)\simp\ && \
	make

.PHONY:	solo
solo: CXXFLAGS += -DNDEBUG -O3 
solo: CCFLAGS += -DNDEBUG -O3 
solo: $(BUILD_DIR)/$(TARGET)

.PHONY: clean
clean: 
	rm -r $(BUILD_DIR)

.PHONY: debug
debug: CXXFLAGS += -DDEBUG -g -O0
debug: CCFLAGS += -DDEBUG -g -O0
debug: $(BUILD_DIR)/$(TARGET)

#--------------------------------------------------------------------------#
#	Build Rules
#--------------------------------------------------------------------------#

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)