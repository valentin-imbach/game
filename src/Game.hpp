
#pragma once
#include "Console.hpp"
#include "DebugScreen.hpp"
#include "GuiElement.hpp"
#include "SoundManager.hpp"
#include "Menu.hpp"

ENUM(GameState,
LOADING,
MENU,
CREATE,
SETTINGS,
RUNNING,
PAUSED)

struct GameSettings {
	bool showHitbox = false;
	bool showCollider = false;
	bool showGuiBox = false;
};

class Game {
public:
	Game();

	// TrackMix trackMix;

	void update(uint dt);
	void draw();
	void pollEvents(uint dt);

	void cleanup();

	GameState::value gameState;
	Console console;
	DebugScreen debugScreen;
	std::unique_ptr<World> world;

	int framesPerSecond;
	int updatesPerSecond;

	static GameSettings settings;

private:
	bool handleEvent(InputEvent event, uint dt);
	void buildMenus();

	MainMenu mainMenu;
	CreateMenu createMenu;
	PauseMenu pauseMenu;

	uint ticks;

	void createWorld(std::string name, uint seed, WorldParameters params);
	void removeWorld(std::string name);
	void loadWorld(std::string name);
	void saveWorld();

friend class MainMenu;
friend class PauseMenu;
friend class CreateMenu;
};