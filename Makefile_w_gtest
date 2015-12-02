# Where to find user code.
USER_DIR = ../samples

CC=g++ #compiler 
# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include -Wfatal-errors -g -Wall -Wextra -pthread -O2 -std=c++1y $(shell root-config --cflags)

# Flags passed to the C++ compiler.
CXXFLAGS +=  $(shell root-config --libs) -lRooFit -lMinuit -lRooFitCore

# Executables produced by this Makefile
RUN = Run

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = Test

# Useful variables
MAIN = objects/main.o
CELLID = objects/CreateCellID.o
INCLUDES = -I ./test/include/ -I ./include

OBJECTS = $(CELLID)

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : $(RUN) $(TESTS)

clean :
	@rm -f ./$(RUN) ./$(TESTS) objects/*

# Builds gtest.a and gtest_main.a.
# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
objects/gtest-all.o : $(GTEST_SRCS_)
	@$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

objects/gtest_main.o : $(GTEST_SRCS_)
	@$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

objects/gtest.a : objects/gtest-all.o
	@$(AR) $(ARFLAGS) $@ $^

objects/gtest_main.a : objects/gtest-all.o objects/gtest_main.o
	@$(AR) $(ARFLAGS) $@ $^

# Builds a sample test.  A test should link with either gtest.a or
# gtest_main.a, depending on whether it defines its own main()
# function.

objects/test.o : ./test/src/test.cpp $(GTEST_HEADERS)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ./test/src/test.cpp $(INCLUDES) -o $@

Test : objects/test.o objects/gtest.a $(OBJECTS)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ $(INCLUDES)

#Test : objects/sample1.o objects/gtest_main.a ../src/ConfigReaderAnalysis.cc ../src/ConfigReaderHisto.cc ../src/ConfigReader.cc  ../src/EventLooper.cc
#	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ $(INCLUDES)


# Building the graviton code
Run: $(MAIN) $(CELLID)
	$(CC) $(CXXFLAGS) $(INCLUDES) $^ -o $@

objects/CreateCellID.o: src/CreateCellID.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

objects/main.o: src/main.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

