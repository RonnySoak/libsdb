# libsdb

# Makefile for libsdb

LIBSSA_DIR = ../libssa

# non compilable files, some targets depend on
DEPS := Makefile
# objects to compile
OBJS := 
# tests
TESTS := 
# files to clean, that are not in OBJS or TESTS 
TO_CLEAN := libsdb.a

COVERAGE_DIR = coverage_data

# All of the sources participating in the build are defined here
-include tests/subdir.mk
-include src/subdir.mk

LIBS := -lpthread
TEST_LIBS := -lcheck -lm -lrt

# GNU options
#DEBUG_FLAGS := -g --coverage

CXX := gcc
CXXFLAGS := -Wall -O3 -std=c99 -march=native $(DEBUG_FLAGS)

PROG := libsdb libsdb_check libsdb_example

.SUFFIXES := .o .c

%.o : %.c $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all : init $(PROG)

init:
	@echo 'Copying file libssa_extern_db.h'
	cp $(LIBSSA_DIR)/src/libssa_extern_db.h src/

libsdb : init $(OBJS) $(USR_OBJS) $(DEPS)
	@echo 'Building target: $@'
	ar -cvq libsdb.a $(DEPS) $(OBJS)
	@echo 'Finished building target: $@'

libsdb_check : init libsdb $(TESTS)
	@echo 'Building target: $@'
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(TESTS) $(TEST_LIBS) $(LIBS)
	@echo 'Finished building target: $@'

libsdb_example : init libsdb ./src/libsdb_example.o
	@echo 'Building target: $@'
	$(CXX) $(CXXFLAGS) -o $@ ./src/libsdb_example.c $(LIBS) -L. -lsdb
	@echo 'Finished building target: $@'

# clean created files
clean:
	rm -f $(OBJS) $(TESTS) $(TO_CLEAN) $(PROG) gmon.out
	rm -rf $(COVERAGE_DIR)
	find . -type f -name '*.gcda' -print | xargs /bin/rm -f
	find . -type f -name '*.gcno' -print | xargs /bin/rm -f
	find . -type f -name '*.gcov' -print | xargs /bin/rm -f

# run tests
check:
	./libsdb_check

# to use this, DEBUG_FLAGS have to be added to CXXFLAGS while compiling the sources
coverage : $(OBJS) 
	@echo Running tests
	./libsdb_check
	@echo Computing test coverage
	mkdir -p $(COVERAGE_DIR)
	lcov --directory . --capture --output-file $(COVERAGE_DIR)/libsdb.info
	genhtml --output-directory $(COVERAGE_DIR)/cov_htmp $(COVERAGE_DIR)/libsdb.info
	@echo Finished computing test coverage

# run example
example:
	./libsdb_example
