# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /home/chaos/Downloads/clion-2018.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/chaos/Downloads/clion-2018.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chaos/projects/programming-lib/samhandler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chaos/projects/programming-lib/samhandler/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/samhandler.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/samhandler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/samhandler.dir/flags.make

CMakeFiles/samhandler.dir/main.cpp.o: CMakeFiles/samhandler.dir/flags.make
CMakeFiles/samhandler.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chaos/projects/programming-lib/samhandler/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/samhandler.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/samhandler.dir/main.cpp.o -c /home/chaos/projects/programming-lib/samhandler/main.cpp

CMakeFiles/samhandler.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/samhandler.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chaos/projects/programming-lib/samhandler/main.cpp > CMakeFiles/samhandler.dir/main.cpp.i

CMakeFiles/samhandler.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/samhandler.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chaos/projects/programming-lib/samhandler/main.cpp -o CMakeFiles/samhandler.dir/main.cpp.s

# Object files for target samhandler
samhandler_OBJECTS = \
"CMakeFiles/samhandler.dir/main.cpp.o"

# External object files for target samhandler
samhandler_EXTERNAL_OBJECTS =

samhandler: CMakeFiles/samhandler.dir/main.cpp.o
samhandler: CMakeFiles/samhandler.dir/build.make
samhandler: core/libsamtoolcore.a
samhandler: CMakeFiles/samhandler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chaos/projects/programming-lib/samhandler/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable samhandler"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/samhandler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/samhandler.dir/build: samhandler

.PHONY : CMakeFiles/samhandler.dir/build

CMakeFiles/samhandler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/samhandler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/samhandler.dir/clean

CMakeFiles/samhandler.dir/depend:
	cd /home/chaos/projects/programming-lib/samhandler/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chaos/projects/programming-lib/samhandler /home/chaos/projects/programming-lib/samhandler /home/chaos/projects/programming-lib/samhandler/cmake-build-debug /home/chaos/projects/programming-lib/samhandler/cmake-build-debug /home/chaos/projects/programming-lib/samhandler/cmake-build-debug/CMakeFiles/samhandler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/samhandler.dir/depend
