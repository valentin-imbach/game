
# Game

This is a work-in-progress prototype of a top-town rpg/adventure game written in c++ using SDL2 for cross-platform compatibility.

## Build using cmake

For unix users, run the following from the repository directory

```bash
mkdir build
cd build
cmake ..
cmake --build .
./Game
```

## Dependencies

* [SDL2](https://www.libsdl.org/download-2.0.php)
* [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
* [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
* [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)

## Controls

There are no control instructions in the game yet, here is a quick rundown:

* Move with the **W A S D** keys
* Interact with objects using the **RMB**
* Use tools with the **LMB**
* Use the **ESC** key to enter the pause/settings screen
* Open the inventory with the **E** key
* Hover over items && hold the **SHIFT** key to display item stats
* Switch selected item using the number keys or the scroll wheel
* Throw selected item with the **Q** key
* Toggle the debug window using the **P** key
* Toggle the console using the **TAB** key


## Contributing

If you have any comments, suggestions or other input, feel free to contact me at **valentin.imbach@gmail.com**.
