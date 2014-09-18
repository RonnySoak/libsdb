# libsdb

# Makefile for libsdb

# non compilable files, some targets depend on
DEPS := Makefile
# objects to compile
OBJS := 
# tests
TESTS := 
# files to clean, that are not in OBJS or TESTS 
TO_CLEAN := libsdb.a

# All of the sources participating in the build are defined here
-include tests/subdir.mk
-include src/subdir.mk

MPI_COMPILE := `mpicxx --showme:compile`
MPI_LINK := `mpicxx --showme:link`

COMMON := -g
#COMMON := -pg -g

LIBS := -lpthread
TEST_LIBS := -lcheck -lm -lrt
LINKFLAGS := $(COMMON)

# Intel options
#CXX := icpc
#CXXFLAGS := -Wall -Wno-missing-declarations -fast -xSSE2 $(COMMON)

# GNU options
CXX := gcc
# -Wno-write-strings removes the `deprecated conversion from\
 string constant to char*` warnings
CXXFLAGS := -Wall -O3 -std=c99 -march=native $(COMMON)

PROG := libsdb libsdb_check

.SUFFIXES := .o .c

%.o : %.c $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all : $(PROG)

#mpilibsdb.o : src/libsdb.cc $(OBJS) $(DEPS)
#	$(CXX) $(CXXFLAGS) -DMPIlibsdb $(MPI_COMPILE) -c -o $@ src/libsdb.cc

libsdb : $(OBJS) $(DEPS)
	@echo 'Building target: $@'
#	$(CXX) $(LINKFLAGS) -o $@ $(OBJS) $(LIBS)
	ar -cvq libsdb.a $(DEPS) $(OBJS)
	@echo 'Finished building target: $@'

#mpilibsdb : mpilibsdb.o $(OBJS) $(DEPS)
#	@echo 'Building target: $@'
#	$(CXX) $(LINKFLAGS) -o $@ mpilibsdb.o $(OBJS) $(LIBS) $(MPI_LINK)
#	@echo 'Finished building target: $@'

libsdb_check : $(TESTS) $(OBJS) $(DEPS)
	@echo 'Building target: $@'
	$(CXX) $(LINKFLAGS) -o $@ $(OBJS) $(TESTS) $(TEST_LIBS) $(LIBS)
	@echo 'Finished building target: $@'

# clean created files
clean:
	-rm -f $(OBJS) $(TESTS) $(TO_CLEAN) $(PROG)

# run tests
check:
	./libsdb_check