# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/valentin/Desktop/Game/game

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/valentin/Desktop/Game/game/build

# Include any dependencies generated for this target.
include CMakeFiles/Game.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Game.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Game.dir/flags.make

CMakeFiles/Game.dir/src/Window.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/src/Window.cpp.o: ../src/Window.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valentin/Desktop/Game/game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Game.dir/src/Window.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/src/Window.cpp.o -c /Users/valentin/Desktop/Game/game/src/Window.cpp

CMakeFiles/Game.dir/src/Window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/src/Window.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valentin/Desktop/Game/game/src/Window.cpp > CMakeFiles/Game.dir/src/Window.cpp.i

CMakeFiles/Game.dir/src/Window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/src/Window.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valentin/Desktop/Game/game/src/Window.cpp -o CMakeFiles/Game.dir/src/Window.cpp.s

CMakeFiles/Game.dir/src/Game.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/src/Game.cpp.o: ../src/Game.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valentin/Desktop/Game/game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Game.dir/src/Game.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/src/Game.cpp.o -c /Users/valentin/Desktop/Game/game/src/Game.cpp

CMakeFiles/Game.dir/src/Game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/src/Game.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valentin/Desktop/Game/game/src/Game.cpp > CMakeFiles/Game.dir/src/Game.cpp.i

CMakeFiles/Game.dir/src/Game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/src/Game.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valentin/Desktop/Game/game/src/Game.cpp -o CMakeFiles/Game.dir/src/Game.cpp.s

CMakeFiles/Game.dir/src/TextureManager.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/src/TextureManager.cpp.o: ../src/TextureManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valentin/Desktop/Game/game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Game.dir/src/TextureManager.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/src/TextureManager.cpp.o -c /Users/valentin/Desktop/Game/game/src/TextureManager.cpp

CMakeFiles/Game.dir/src/TextureManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/src/TextureManager.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valentin/Desktop/Game/game/src/TextureManager.cpp > CMakeFiles/Game.dir/src/TextureManager.cpp.i

CMakeFiles/Game.dir/src/TextureManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/src/TextureManager.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valentin/Desktop/Game/game/src/TextureManager.cpp -o CMakeFiles/Game.dir/src/TextureManager.cpp.s

CMakeFiles/Game.dir/src/main.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valentin/Desktop/Game/game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Game.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/src/main.cpp.o -c /Users/valentin/Desktop/Game/game/src/main.cpp

CMakeFiles/Game.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valentin/Desktop/Game/game/src/main.cpp > CMakeFiles/Game.dir/src/main.cpp.i

CMakeFiles/Game.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valentin/Desktop/Game/game/src/main.cpp -o CMakeFiles/Game.dir/src/main.cpp.s

# Object files for target Game
Game_OBJECTS = \
"CMakeFiles/Game.dir/src/Window.cpp.o" \
"CMakeFiles/Game.dir/src/Game.cpp.o" \
"CMakeFiles/Game.dir/src/TextureManager.cpp.o" \
"CMakeFiles/Game.dir/src/main.cpp.o"

# External object files for target Game
Game_EXTERNAL_OBJECTS =

Game: CMakeFiles/Game.dir/src/Window.cpp.o
Game: CMakeFiles/Game.dir/src/Game.cpp.o
Game: CMakeFiles/Game.dir/src/TextureManager.cpp.o
Game: CMakeFiles/Game.dir/src/main.cpp.o
Game: CMakeFiles/Game.dir/build.make
Game: CMakeFiles/Game.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/valentin/Desktop/Game/game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable Game"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Game.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Game.dir/build: Game

.PHONY : CMakeFiles/Game.dir/build

CMakeFiles/Game.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Game.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Game.dir/clean

CMakeFiles/Game.dir/depend:
	cd /Users/valentin/Desktop/Game/game/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/valentin/Desktop/Game/game /Users/valentin/Desktop/Game/game /Users/valentin/Desktop/Game/game/build /Users/valentin/Desktop/Game/game/build /Users/valentin/Desktop/Game/game/build/CMakeFiles/Game.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Game.dir/depend

