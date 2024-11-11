#ifndef CONTROLLER_GUARD
#define CONTROLLER_GUARD

#include "View.h"

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

class Controller {
public:
    sf::RenderWindow window;
    Window_Mgr window_mgr;
    Buttons_Manager buttons;
    View view_mgr;
    sf::Clock clock;

    Controller() : window(sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "PhotoShop")),
                                            window_mgr(Window_Mgr()),
                                            buttons(Buttons_Manager()),
                                            view_mgr(View(buttons, &window_mgr)),
                                            clock() {
                                            srand(static_cast<unsigned>(time(0)));
                                            }


    void draw_model() {
        view_mgr.draw_all(window);
    }

    void update_model() {
        window_mgr.check_events(window);
    }
};

#endif
