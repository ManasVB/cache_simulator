CC = g++
OPT = -O3
#OPT = -g
WARN = -Wall
# You can select a C++ standard using the STD define below.  To do so, uncomment (remove leading #) and adjust the standard as needed.
STD = -std=c++11
CFLAGS = $(OPT) $(WARN) $(STD) $(INC) $(LIB)

# List all your .cc/.cpp files here (source files, excluding header files)
SIM_SRC = src/
SIM_INC = -I inc/
BUILD_DIR = build

# List corresponding compiled object files here (.o files)
SIM_OBJ = sim.o
 
SOURCES = $(shell find $(SIM_SRC) -name '*.cpp')
OBJECTS = $(SOURCES:%=$(BUILD_DIR)/%.o)

EXECUTABLE = sim

# Define Messages

MSG_MKDIR = Creating Build Directory
MSG_COMPILE = Compiling...


#################################

# default rule

all: mkdir $(EXECUTABLE)
	@echo "my work is done here..."


mkdir:
	@echo $(MSG_MKDIR)
	@mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.cpp
	@echo $(MSG_COMPILE)
	$(CC) $(CFLAGS) $(INC_PATH) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o sim $(CFLAGS) $(SIM_OBJ) -lm
# rule for making sim

# sim: $(SIM_OBJ)
# 	$(CC) -o sim $(CFLAGS) $(SIM_OBJ) -lm
# 	@echo "-----------DONE WITH sim-----------"


# # generic rule for converting any .cc file to any .o file
 
# .cc.o:
# 	$(CC) $(CFLAGS) -c $*.cc

# # generic rule for converting any .cpp file to any .o file

# .cpp.o:
# 	$(CC) $(CFLAGS) -c $*.cpp


# type "make clean" to remove all .o files plus the sim binary

clean:
	rm -f *.o sim


# type "make clobber" to remove all .o files (leaves sim binary)

clobber:
	rm -f *.o


