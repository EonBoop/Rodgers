# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/eon/fucking_around/c++/ai/Rodgers

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eon/fucking_around/c++/ai/Rodgers/buid

# Include any dependencies generated for this target.
include CMakeFiles/rodgers.exe.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/rodgers.exe.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/rodgers.exe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rodgers.exe.dir/flags.make

CMakeFiles/rodgers.exe.dir/main.cpp.o: CMakeFiles/rodgers.exe.dir/flags.make
CMakeFiles/rodgers.exe.dir/main.cpp.o: ../main.cpp
CMakeFiles/rodgers.exe.dir/main.cpp.o: CMakeFiles/rodgers.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eon/fucking_around/c++/ai/Rodgers/buid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rodgers.exe.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rodgers.exe.dir/main.cpp.o -MF CMakeFiles/rodgers.exe.dir/main.cpp.o.d -o CMakeFiles/rodgers.exe.dir/main.cpp.o -c /home/eon/fucking_around/c++/ai/Rodgers/main.cpp

CMakeFiles/rodgers.exe.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rodgers.exe.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eon/fucking_around/c++/ai/Rodgers/main.cpp > CMakeFiles/rodgers.exe.dir/main.cpp.i

CMakeFiles/rodgers.exe.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rodgers.exe.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eon/fucking_around/c++/ai/Rodgers/main.cpp -o CMakeFiles/rodgers.exe.dir/main.cpp.s

# Object files for target rodgers.exe
rodgers_exe_OBJECTS = \
"CMakeFiles/rodgers.exe.dir/main.cpp.o"

# External object files for target rodgers.exe
rodgers_exe_EXTERNAL_OBJECTS =

rodgers.exe: CMakeFiles/rodgers.exe.dir/main.cpp.o
rodgers.exe: CMakeFiles/rodgers.exe.dir/build.make
rodgers.exe: CMakeFiles/rodgers.exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eon/fucking_around/c++/ai/Rodgers/buid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable rodgers.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rodgers.exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rodgers.exe.dir/build: rodgers.exe
.PHONY : CMakeFiles/rodgers.exe.dir/build

CMakeFiles/rodgers.exe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rodgers.exe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rodgers.exe.dir/clean

CMakeFiles/rodgers.exe.dir/depend:
	cd /home/eon/fucking_around/c++/ai/Rodgers/buid && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eon/fucking_around/c++/ai/Rodgers /home/eon/fucking_around/c++/ai/Rodgers /home/eon/fucking_around/c++/ai/Rodgers/buid /home/eon/fucking_around/c++/ai/Rodgers/buid /home/eon/fucking_around/c++/ai/Rodgers/buid/CMakeFiles/rodgers.exe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rodgers.exe.dir/depend

