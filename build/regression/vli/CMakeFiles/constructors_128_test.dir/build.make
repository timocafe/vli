# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/cmake/2.8.12/bin/cmake

# The command to remove a file.
RM = /usr/local/cmake/2.8.12/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/local/cmake/2.8.12/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ewart/Desktop/vli

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ewart/Desktop/vli/build

# Include any dependencies generated for this target.
include regression/vli/CMakeFiles/constructors_128_test.dir/depend.make

# Include the progress variables for this target.
include regression/vli/CMakeFiles/constructors_128_test.dir/progress.make

# Include the compile flags for this target's objects.
include regression/vli/CMakeFiles/constructors_128_test.dir/flags.make

regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o: regression/vli/CMakeFiles/constructors_128_test.dir/flags.make
regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o: ../regression/vli/constructors.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/ewart/Desktop/vli/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o"
	cd /Users/ewart/Desktop/vli/build/regression/vli && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/constructors_128_test.dir/constructors.cpp.o -c /Users/ewart/Desktop/vli/regression/vli/constructors.cpp

regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/constructors_128_test.dir/constructors.cpp.i"
	cd /Users/ewart/Desktop/vli/build/regression/vli && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/ewart/Desktop/vli/regression/vli/constructors.cpp > CMakeFiles/constructors_128_test.dir/constructors.cpp.i

regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/constructors_128_test.dir/constructors.cpp.s"
	cd /Users/ewart/Desktop/vli/build/regression/vli && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/ewart/Desktop/vli/regression/vli/constructors.cpp -o CMakeFiles/constructors_128_test.dir/constructors.cpp.s

regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o.requires:
.PHONY : regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o.requires

regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o.provides: regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o.requires
	$(MAKE) -f regression/vli/CMakeFiles/constructors_128_test.dir/build.make regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o.provides.build
.PHONY : regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o.provides

regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o.provides.build: regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o

# Object files for target constructors_128_test
constructors_128_test_OBJECTS = \
"CMakeFiles/constructors_128_test.dir/constructors.cpp.o"

# External object files for target constructors_128_test
constructors_128_test_EXTERNAL_OBJECTS =

regression/vli/constructors_128_test: regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o
regression/vli/constructors_128_test: regression/vli/CMakeFiles/constructors_128_test.dir/build.make
regression/vli/constructors_128_test: /usr/local/boost/1.57/lib/libboost_unit_test_framework.dylib
regression/vli/constructors_128_test: /usr/local/boost/1.57/lib/libboost_random.dylib
regression/vli/constructors_128_test: /usr/local/lib/libgmpxx.dylib
regression/vli/constructors_128_test: /usr/local/lib/libgmp.dylib
regression/vli/constructors_128_test: src/vli/libvli.a
regression/vli/constructors_128_test: regression/vli/CMakeFiles/constructors_128_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable constructors_128_test"
	cd /Users/ewart/Desktop/vli/build/regression/vli && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/constructors_128_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
regression/vli/CMakeFiles/constructors_128_test.dir/build: regression/vli/constructors_128_test
.PHONY : regression/vli/CMakeFiles/constructors_128_test.dir/build

regression/vli/CMakeFiles/constructors_128_test.dir/requires: regression/vli/CMakeFiles/constructors_128_test.dir/constructors.cpp.o.requires
.PHONY : regression/vli/CMakeFiles/constructors_128_test.dir/requires

regression/vli/CMakeFiles/constructors_128_test.dir/clean:
	cd /Users/ewart/Desktop/vli/build/regression/vli && $(CMAKE_COMMAND) -P CMakeFiles/constructors_128_test.dir/cmake_clean.cmake
.PHONY : regression/vli/CMakeFiles/constructors_128_test.dir/clean

regression/vli/CMakeFiles/constructors_128_test.dir/depend:
	cd /Users/ewart/Desktop/vli/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ewart/Desktop/vli /Users/ewart/Desktop/vli/regression/vli /Users/ewart/Desktop/vli/build /Users/ewart/Desktop/vli/build/regression/vli /Users/ewart/Desktop/vli/build/regression/vli/CMakeFiles/constructors_128_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : regression/vli/CMakeFiles/constructors_128_test.dir/depend

