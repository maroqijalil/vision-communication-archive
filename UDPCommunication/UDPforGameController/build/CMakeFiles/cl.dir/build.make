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
CMAKE_SOURCE_DIR = /home/maroqi/Downloads/jalil-job/UDPforGameController

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maroqi/Downloads/jalil-job/UDPforGameController/build

# Include any dependencies generated for this target.
include CMakeFiles/cl.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cl.dir/flags.make

CMakeFiles/cl.dir/Client.cpp.o: CMakeFiles/cl.dir/flags.make
CMakeFiles/cl.dir/Client.cpp.o: ../Client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maroqi/Downloads/jalil-job/UDPforGameController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cl.dir/Client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cl.dir/Client.cpp.o -c /home/maroqi/Downloads/jalil-job/UDPforGameController/Client.cpp

CMakeFiles/cl.dir/Client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cl.dir/Client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maroqi/Downloads/jalil-job/UDPforGameController/Client.cpp > CMakeFiles/cl.dir/Client.cpp.i

CMakeFiles/cl.dir/Client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cl.dir/Client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maroqi/Downloads/jalil-job/UDPforGameController/Client.cpp -o CMakeFiles/cl.dir/Client.cpp.s

CMakeFiles/cl.dir/Client.cpp.o.requires:

.PHONY : CMakeFiles/cl.dir/Client.cpp.o.requires

CMakeFiles/cl.dir/Client.cpp.o.provides: CMakeFiles/cl.dir/Client.cpp.o.requires
	$(MAKE) -f CMakeFiles/cl.dir/build.make CMakeFiles/cl.dir/Client.cpp.o.provides.build
.PHONY : CMakeFiles/cl.dir/Client.cpp.o.provides

CMakeFiles/cl.dir/Client.cpp.o.provides.build: CMakeFiles/cl.dir/Client.cpp.o


# Object files for target cl
cl_OBJECTS = \
"CMakeFiles/cl.dir/Client.cpp.o"

# External object files for target cl
cl_EXTERNAL_OBJECTS =

cl: CMakeFiles/cl.dir/Client.cpp.o
cl: CMakeFiles/cl.dir/build.make
cl: /usr/lib/x86_64-linux-gnu/libboost_thread.so
cl: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
cl: /usr/lib/x86_64-linux-gnu/libboost_system.so
cl: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
cl: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
cl: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
cl: /usr/lib/x86_64-linux-gnu/libpthread.so
cl: CMakeFiles/cl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maroqi/Downloads/jalil-job/UDPforGameController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cl"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cl.dir/build: cl

.PHONY : CMakeFiles/cl.dir/build

CMakeFiles/cl.dir/requires: CMakeFiles/cl.dir/Client.cpp.o.requires

.PHONY : CMakeFiles/cl.dir/requires

CMakeFiles/cl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cl.dir/clean

CMakeFiles/cl.dir/depend:
	cd /home/maroqi/Downloads/jalil-job/UDPforGameController/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maroqi/Downloads/jalil-job/UDPforGameController /home/maroqi/Downloads/jalil-job/UDPforGameController /home/maroqi/Downloads/jalil-job/UDPforGameController/build /home/maroqi/Downloads/jalil-job/UDPforGameController/build /home/maroqi/Downloads/jalil-job/UDPforGameController/build/CMakeFiles/cl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cl.dir/depend

