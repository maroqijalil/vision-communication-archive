# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maroqi/Documents/UDPboostAsio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maroqi/Documents/UDPboostAsio/build

# Include any dependencies generated for this target.
include CMakeFiles/sr.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sr.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sr.dir/flags.make

CMakeFiles/sr.dir/Server.cpp.o: CMakeFiles/sr.dir/flags.make
CMakeFiles/sr.dir/Server.cpp.o: ../Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maroqi/Documents/UDPboostAsio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sr.dir/Server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sr.dir/Server.cpp.o -c /home/maroqi/Documents/UDPboostAsio/Server.cpp

CMakeFiles/sr.dir/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sr.dir/Server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maroqi/Documents/UDPboostAsio/Server.cpp > CMakeFiles/sr.dir/Server.cpp.i

CMakeFiles/sr.dir/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sr.dir/Server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maroqi/Documents/UDPboostAsio/Server.cpp -o CMakeFiles/sr.dir/Server.cpp.s

CMakeFiles/sr.dir/Server.cpp.o.requires:

.PHONY : CMakeFiles/sr.dir/Server.cpp.o.requires

CMakeFiles/sr.dir/Server.cpp.o.provides: CMakeFiles/sr.dir/Server.cpp.o.requires
	$(MAKE) -f CMakeFiles/sr.dir/build.make CMakeFiles/sr.dir/Server.cpp.o.provides.build
.PHONY : CMakeFiles/sr.dir/Server.cpp.o.provides

CMakeFiles/sr.dir/Server.cpp.o.provides.build: CMakeFiles/sr.dir/Server.cpp.o


# Object files for target sr
sr_OBJECTS = \
"CMakeFiles/sr.dir/Server.cpp.o"

# External object files for target sr
sr_EXTERNAL_OBJECTS =

sr: CMakeFiles/sr.dir/Server.cpp.o
sr: CMakeFiles/sr.dir/build.make
sr: /usr/lib/x86_64-linux-gnu/libboost_thread.so
sr: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
sr: /usr/lib/x86_64-linux-gnu/libboost_system.so
sr: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
sr: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
sr: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
sr: /usr/lib/x86_64-linux-gnu/libpthread.so
sr: CMakeFiles/sr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maroqi/Documents/UDPboostAsio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sr"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sr.dir/build: sr

.PHONY : CMakeFiles/sr.dir/build

CMakeFiles/sr.dir/requires: CMakeFiles/sr.dir/Server.cpp.o.requires

.PHONY : CMakeFiles/sr.dir/requires

CMakeFiles/sr.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sr.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sr.dir/clean

CMakeFiles/sr.dir/depend:
	cd /home/maroqi/Documents/UDPboostAsio/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maroqi/Documents/UDPboostAsio /home/maroqi/Documents/UDPboostAsio /home/maroqi/Documents/UDPboostAsio/build /home/maroqi/Documents/UDPboostAsio/build /home/maroqi/Documents/UDPboostAsio/build/CMakeFiles/sr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sr.dir/depend

