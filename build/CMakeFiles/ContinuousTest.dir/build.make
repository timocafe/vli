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

# Utility rule file for ContinuousTest.

# Include the progress variables for this target.
include CMakeFiles/ContinuousTest.dir/progress.make

CMakeFiles/ContinuousTest:
	/usr/local/cmake/2.8.12/bin/ctest -D ContinuousTest

ContinuousTest: CMakeFiles/ContinuousTest
ContinuousTest: CMakeFiles/ContinuousTest.dir/build.make
.PHONY : ContinuousTest

# Rule to build all files generated by this target.
CMakeFiles/ContinuousTest.dir/build: ContinuousTest
.PHONY : CMakeFiles/ContinuousTest.dir/build

CMakeFiles/ContinuousTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ContinuousTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ContinuousTest.dir/clean

CMakeFiles/ContinuousTest.dir/depend:
	cd /Users/ewart/Desktop/vli/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ewart/Desktop/vli /Users/ewart/Desktop/vli /Users/ewart/Desktop/vli/build /Users/ewart/Desktop/vli/build /Users/ewart/Desktop/vli/build/CMakeFiles/ContinuousTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ContinuousTest.dir/depend

