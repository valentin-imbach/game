
#pragma once
#include "GuiElement.hpp"
class Game;

class Menu {
public:
	virtual void build(Game* game) = 0;
	virtual void update();
	virtual void draw(uint ticks);
	virtual bool handleEvent(InputEvent event);
protected:
	std::unique_ptr<Widget> widget;
};

class MainMenu : public Menu {
public:
	MainMenu();
	void build(Game* game) override;
	void draw(uint ticks) override;

private:
	SDL_Texture* parallax[4];
};

class PauseMenu : public Menu {
public:
	void build(Game* game) override;
};

class CreateMenu : public Menu {
public:
	void build(Game* game) override;
private:
	TextField* nameField = nullptr;
	TextField* seedField = nullptr;
	
	int s1 = 50;
	int s2 = 50;
	int s3 = 50;
	int s4 = 50;
};