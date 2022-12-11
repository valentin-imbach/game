
#include "GuiElement.hpp"
#include <vector>
#include "Window.hpp"
#include "utils/direction.hpp"

GuiElement::GuiElement(pair position, Direction alignment) : position(position), alignment(alignment) {}

void GuiElement::reposition(GuiElement* parent) {
	screenSize = GUI_SCALE * size;
	if (parent) {
		screenPosition = parent->position + position;
		if (alignment != Direction::NONE) {
			pair step = taxiSteps[int(alignment) - 1];
			screenPosition.x += parent->screenSize.x * step.x / 2;
			screenPosition.y += parent->screenSize.y * step.y / 2;
		}
	} else {
		screenPosition = Window::instance->size / 2;
		if (alignment != Direction::NONE) {
			pair step = taxiSteps[int(alignment) - 1];
			screenPosition.x += Window::instance->size.x * step.x / 2;
			screenPosition.y += Window::instance->size.y * step.y / 2;
		}
	}
}

Widget::Widget(pair position, Sprite sprite) : GuiElement(position), sprite(sprite) {
	children = std::vector<GuiElement>();
}

void Widget::addGuiElement(GuiElement guiElement) {
	children.push_back(guiElement);
}

void Widget::update() {
	for (GuiElement& guiElement : children) {
		guiElement.reposition(this);
		guiElement.update();
	}
}

void Widget::draw() {
	sprite.draw(screenPosition, GUI_SCALE, true);
	for (GuiElement& guiElement : children) {
		guiElement.draw();
	}
}