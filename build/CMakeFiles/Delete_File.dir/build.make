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
CMAKE_SOURCE_DIR = /usr/local/nginx/CGI_Program

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /usr/local/nginx/CGI_Program/build

# Include any dependencies generated for this target.
include CMakeFiles/Delete_File.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Delete_File.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Delete_File.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Delete_File.dir/flags.make

CMakeFiles/Delete_File.dir/src/delete_file.cpp.o: CMakeFiles/Delete_File.dir/flags.make
CMakeFiles/Delete_File.dir/src/delete_file.cpp.o: ../src/delete_file.cpp
CMakeFiles/Delete_File.dir/src/delete_file.cpp.o: CMakeFiles/Delete_File.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/usr/local/nginx/CGI_Program/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Delete_File.dir/src/delete_file.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Delete_File.dir/src/delete_file.cpp.o -MF CMakeFiles/Delete_File.dir/src/delete_file.cpp.o.d -o CMakeFiles/Delete_File.dir/src/delete_file.cpp.o -c /usr/local/nginx/CGI_Program/src/delete_file.cpp

CMakeFiles/Delete_File.dir/src/delete_file.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Delete_File.dir/src/delete_file.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/local/nginx/CGI_Program/src/delete_file.cpp > CMakeFiles/Delete_File.dir/src/delete_file.cpp.i

CMakeFiles/Delete_File.dir/src/delete_file.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Delete_File.dir/src/delete_file.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/local/nginx/CGI_Program/src/delete_file.cpp -o CMakeFiles/Delete_File.dir/src/delete_file.cpp.s

# Object files for target Delete_File
Delete_File_OBJECTS = \
"CMakeFiles/Delete_File.dir/src/delete_file.cpp.o"

# External object files for target Delete_File
Delete_File_EXTERNAL_OBJECTS =

../CGI_bin/Delete_File: CMakeFiles/Delete_File.dir/src/delete_file.cpp.o
../CGI_bin/Delete_File: CMakeFiles/Delete_File.dir/build.make
../CGI_bin/Delete_File: /usr/lib/x86_64-linux-gnu/libspdlog.so.1.9.2
../CGI_bin/Delete_File: /usr/lib/x86_64-linux-gnu/libfmt.so.8.1.1
../CGI_bin/Delete_File: CMakeFiles/Delete_File.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/usr/local/nginx/CGI_Program/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../CGI_bin/Delete_File"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Delete_File.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Delete_File.dir/build: ../CGI_bin/Delete_File
.PHONY : CMakeFiles/Delete_File.dir/build

CMakeFiles/Delete_File.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Delete_File.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Delete_File.dir/clean

CMakeFiles/Delete_File.dir/depend:
	cd /usr/local/nginx/CGI_Program/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /usr/local/nginx/CGI_Program /usr/local/nginx/CGI_Program /usr/local/nginx/CGI_Program/build /usr/local/nginx/CGI_Program/build /usr/local/nginx/CGI_Program/build/CMakeFiles/Delete_File.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Delete_File.dir/depend
