#pragma once

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <Dwmapi.h>
#include <vector>

#include "Util.h"
#include "Device.h"
#include "Slider.h"
#include "Debugger.h"

#define TASKBAR_HEIGHT 40
#define WIDTH 400
#define HEIGHT 300
#define BAR_HEIGHT 80
#define SCREEN_WIDTH sf::VideoMode::getDesktopMode().width
#define SCREEN_HEIGHT sf::VideoMode::getDesktopMode().height

class Window {

private:
	sf::RenderWindow* window;
	sf::View view;
	sf::RectangleShape background;
	sf::Event event;

	std::vector<Slider*> sliders;

	int processCount;
	int visible = 0;

	HRESULT hr;

public:
	Window();
	void update();
	void loadDevice(Device*);
	void updateVolume(Device*);
	int isOpen();
	int isVisible();
	void show();
	void hide();
	void select(int);

};