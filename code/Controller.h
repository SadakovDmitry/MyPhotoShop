#ifndef CONTROLLER_GUARD
#define CONTROLLER_GUARD

#include "View.h"

class Controller {
public:
    sf::RenderWindow window;
    std::vector<Molecule*> molecules;
    Window_Mgr window_mgr;
    Buttons_Manager buttons;
    View view_mgr;
    Phys_Mgr phys_mgr;
    sf::Clock clock;

    Controller() : window(sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Moleculs")),
                                            molecules(),
                                            window_mgr(Window_Mgr(molecules)),
                                            buttons(Buttons_Manager(molecules)),
                                            view_mgr(View(molecules, buttons, &window_mgr)),
                                            phys_mgr(Phys_Mgr(molecules)),
                                            clock() {
                                            srand(static_cast<unsigned>(time(0)));
                                            }


    void draw_model() {
        view_mgr.draw_all(window);
    }

    void update_model() {
        window_mgr.check_events(window);
        float delta_time = clock.restart().asSeconds();
        phys_mgr.check_collisions(molecules, delta_time);
    }
};

#endif
