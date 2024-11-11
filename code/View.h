#ifndef VIEW_GUARD
#define VIEW_GUARD

#include "Scene.h"
#include "Window_Mgr.h"
#include "Buttons.h"

class View {
public:
    Buttons_Manager button_mgr;
    Window_Mgr* window_mgr;

    View(class Buttons_Manager button_mgr_, Window_Mgr* window_mgr_) :
                               button_mgr(button_mgr_), window_mgr(window_mgr_) {}

    void draw_all(sf::RenderWindow& window) {
        window.clear(sf::Color::Black);
        show_windows(window);
        window.display();
    }

    void show_buttons(sf::RenderWindow &window) {
        for (Button* button : button_mgr.buttons){
            assert(button);
            button -> sprite.setColor(sf::Color(button -> color.R, button -> color.G, button -> color.B, button -> color.T));
            button -> sprite_pushed.setColor(sf::Color(button -> color.R, button -> color.G, button -> color.B, 255 - button -> color.T));
            window.draw(button -> sprite);
            window.draw(button -> sprite_pushed);
        }
    }

    void show_windows(sf::RenderWindow &window) {
        for (const auto& my_window : window_mgr -> window_mgr) {
            assert(my_window);
            my_window -> draw(window);
        }
    }

    ~View() = default;
};

#endif
