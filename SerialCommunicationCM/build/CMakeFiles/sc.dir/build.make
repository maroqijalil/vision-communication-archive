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
CMAKE_SOURCE_DIR = /home/hiro/framework/project/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hiro/framework/project/test/build

# Include any dependencies generated for this target.
include CMakeFiles/sc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sc.dir/flags.make

CMakeFiles/sc.dir/scCM(Boost).cpp.o: CMakeFiles/sc.dir/flags.make
CMakeFiles/sc.dir/scCM(Boost).cpp.o: ../scCM(Boost).cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hiro/framework/project/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sc.dir/scCM(Boost).cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o "CMakeFiles/sc.dir/scCM(Boost).cpp.o" -c "/home/hiro/framework/project/test/scCM(Boost).cpp"

CMakeFiles/sc.dir/scCM(Boost).cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sc.dir/scCM(Boost).cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/hiro/framework/project/test/scCM(Boost).cpp" > "CMakeFiles/sc.dir/scCM(Boost).cpp.i"

CMakeFiles/sc.dir/scCM(Boost).cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sc.dir/scCM(Boost).cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/hiro/framework/project/test/scCM(Boost).cpp" -o "CMakeFiles/sc.dir/scCM(Boost).cpp.s"

CMakeFiles/sc.dir/scCM(Boost).cpp.o.requires:

.PHONY : CMakeFiles/sc.dir/scCM(Boost).cpp.o.requires

CMakeFiles/sc.dir/scCM(Boost).cpp.o.provides: CMakeFiles/sc.dir/scCM(Boost).cpp.o.requires
	$(MAKE) -f CMakeFiles/sc.dir/build.make "CMakeFiles/sc.dir/scCM(Boost).cpp.o.provides.build"
.PHONY : CMakeFiles/sc.dir/scCM(Boost).cpp.o.provides

CMakeFiles/sc.dir/scCM(Boost).cpp.o.provides.build: CMakeFiles/sc.dir/scCM(Boost).cpp.o


# Object files for target sc
sc_OBJECTS = \
"CMakeFiles/sc.dir/scCM(Boost).cpp.o"

# External object files for target sc
sc_EXTERNAL_OBJECTS =

sc: CMakeFiles/sc.dir/scCM(Boost).cpp.o
sc: CMakeFiles/sc.dir/build.make
sc: /usr/lib/x86_64-linux-gnu/libboost_thread.so
sc: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
sc: /usr/lib/x86_64-linux-gnu/libboost_system.so
sc: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
sc: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
sc: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
sc: /usr/lib/x86_64-linux-gnu/libpthread.so
sc: CMakeFiles/sc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hiro/framework/project/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sc.dir/build: sc

.PHONY : CMakeFiles/sc.dir/build

CMakeFiles/sc.dir/requires: CMakeFiles/sc.dir/scCM(Boost).cpp.o.requires

.PHONY : CMakeFiles/sc.dir/requires

CMakeFiles/sc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sc.dir/clean

CMakeFiles/sc.dir/depend:
	cd /home/hiro/framework/project/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hiro/framework/project/test /home/hiro/framework/project/test /home/hiro/framework/project/test/build /home/hiro/framework/project/test/build /home/hiro/framework/project/test/build/CMakeFiles/sc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sc.dir/depend

