#include "Window.h"

#pragma region PUBLIC Window::Window()

Window::Window() {
    window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Volume Mixer", sf::Style::None);
    window->setPosition(sf::Vector2i(SCREEN_WIDTH, SCREEN_HEIGHT - HEIGHT - TASKBAR_HEIGHT));
    window->setFramerateLimit(30);
    
    /*
    * WS_EX_TOOLWINDOW  - Window with no border and no taskbar icon
    * WS_EX_TOPMOST     - Window with the topmost layer
    */
    hr = SetWindowLong(window->getSystemHandle(), GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
    error(hr, "(Window.cpp::Window) SetWindowLong");
    
    MARGINS margins;
    margins.cxLeftWidth = -1;
    hr = DwmExtendFrameIntoClientArea(window->getSystemHandle(), &margins);
    error(hr, "(Window.cpp::Window) DwmExtendFrameIntoClientArea");

    //Set background
    background.setFillColor(sf::Color(0, 0, 0, 150));
    background.setSize(sf::Vector2f(WIDTH, HEIGHT));
    
    //Set view
    view = window->getView();

    log("Window.cpp", "Window initialized");
}

#pragma endregion

#pragma region PUBLIC Window::update(): void

void Window::update() {
    while (window->pollEvent(event))
        if (event.type == sf::Event::Closed) window->close();

    //Not stretch window when size of window changes
    view.reset(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));
    window->setView(view);

    window->clear(sf::Color::Transparent);
    window->draw(background);
    for (int i = 0; i < sliders.size(); i++)
        sliders[i]->render(window);

    window->display();
}

#pragma endregion
#pragma region PUBLIC Window::loadDevice(Deivce*): void

void Window::loadDevice(Device* device) {
    log("Window.cpp", "Loading device");

    //Clear sliders
    for (int i = 0; i < sliders.size(); i++) delete sliders[i];
    sliders.clear();

    sf::RectangleShape r;
    r.setSize(sf::Vector2f(WIDTH, BAR_HEIGHT));
    r.setPosition(sf::Vector2f(0, 0));

    //Device volume
    Slider* master = new Slider(ws2s(device->name), device->getVolume() * 100, r);
    sliders.push_back(master);
    
    //Proces volume
    processCount = device->getProcessCount();
    for (int i = 0; i < processCount; i++) {
        r.setPosition(sf::Vector2f(0, (i + 1) * BAR_HEIGHT));
        Slider* process = new Slider(ws2s(device->getProcess(i)->name), device->getProcess(i)->getVolume() * 100, r);
        sliders.push_back(process);
    }

    //Set window
    window->setSize(sf::Vector2u(WIDTH, (processCount + 1) * BAR_HEIGHT));
    window->setPosition(sf::Vector2i(SCREEN_WIDTH, SCREEN_HEIGHT - TASKBAR_HEIGHT - window->getSize().y));

    background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));

    //Add master volume
    processCount++;

    log("Window.cpp", "Deivce loaded");
}

#pragma endregion
#pragma region PUBLIC Window::updateVolume(Deivce*): void

void Window::updateVolume(Device* device) {
    for (int i = 0; i < processCount; i++) {
        if (i == 0) sliders[i]->setLevel(round(device->getVolume() * 100));
        else { sliders[i]->setLevel(round(device->getProcess(i - 1)->getVolume() * 100)); }
    }
}

#pragma endregion
#pragma region PUBLIC Window::select(int): void

void Window::select(int sel) {
    //Enabling selecting slider and disabling others
    for (int i = 0; i < sliders.size(); i++) {
        if (i == sel) sliders[i]->setFocus(1);
        else sliders[i]->setFocus(0);
    }
}

#pragma endregion
#pragma region PUBLIC Window::isVisible(): int

int Window::isVisible() {
    return visible;
}

#pragma endregion
#pragma region PUBLIC Window::isOpen(): int

int Window::isOpen() {
    return window->isOpen();
}

#pragma endregion
#pragma region PUBLIC Window::show(): void

void Window::show() {
    log("Window.cpp", "Show");
    
    visible = 1;
    ShowWindow(window->getSystemHandle(), 1); //Show window

    hr = SendMessage(window->getSystemHandle(), WM_KILLFOCUS, NULL, NULL); //Don't focus on window
    SetWindowPos(window->getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //Set window topmost
    error(hr, "(Window.cpp::show) SendMessage");
}

#pragma endregion
#pragma region PUBLIC Window::hide(): void

void Window::hide() {
    log("Window.cpp", "Hide");

    visible = 0;
    ShowWindow(window->getSystemHandle(), 0); //Hide window
}

#pragma endregion