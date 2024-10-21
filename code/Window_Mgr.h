#ifndef WINDOW_MGR_GUARD
#define WINDOW_MGR_GUARD

#include "Scene.h"
#include "Molecules.h"
#include <iostream>

class Window_Mgr;

class Window {
public:
    int width;
    int height;
    int x_0;
    int y_0;
    Color color;
    sf::Sprite sprite;
    sf::Texture texture;
    bool is_dragging = false;
    sf::Vector2i drag_offset;

    // Window(int width_,     int height_) :
    //      width(width_), height(height_) {}
    Window(int width_,     int height_,  int x_0_,  int y_0_, const Color &color_) :
         width(width_), height(height_), x_0(x_0_), y_0(y_0_),       color(color_) {}

    virtual void draw(sf::RenderWindow &window)     = 0;
    virtual void set_sprite(Color &color)           = 0;
    virtual bool action_pressed(sf::Vector2i mouse) = 0;
    virtual void set_windows() = 0;
    virtual bool check_mouse(sf::RenderWindow &window) = 0;

    void start_drag(sf::Vector2i mouse) {
        if (mouse.x > x_0 && mouse.x < x_0 + width && mouse.y > y_0 && mouse.y < y_0 + height) {
            is_dragging = true;
            drag_offset = mouse - sf::Vector2i(x_0, y_0);
            //std::cout << "start dragging\n";
        }
    }

    void stop_drag() {
        is_dragging = false;
    }

    void update_position(sf::Vector2i mouse) {
        if (is_dragging) {
            x_0 = mouse.x - drag_offset.x;
            y_0 = mouse.y - drag_offset.y;
            //std::cout << "update dragging\n";
        }
    }

    virtual ~Window() = default;
};

#include "Buttons.h"

class SW_Window_Mgr {
public:
    //std::vector<Window*> window_mgr;
    std::vector<std::shared_ptr<Window>> sw_window_mgr;

    SW_Window_Mgr() = default;

    void set_windows() {}

    void draw(sf::RenderWindow &window) {
        for (auto& sw_window : sw_window_mgr) {
            sw_window -> draw(window);
        }
    }

    void check_kb(sf::RenderWindow &window) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.close();
        }
    }

    bool check_mouse(sf::RenderWindow &window) {
        sf::Mouse mouse;
        bool delete_flag = false;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            sf::Vector2i position = sf::Mouse::getPosition(window);
            for (auto& window : sw_window_mgr) {
                delete_flag = window -> action_pressed(position);
            }
        }
        return delete_flag;
    }

    void check_events(sf::RenderWindow &window) {
        sf::Event event;
        while (window.pollEvent(event)){
            check_mouse(window);
            check_kb(window);
            if (event.type == sf::Event::Closed) {window.close();}
        } //while (window.pollEvent(event));
    }

};

class Simple_Window : public Window {
private:
    const float file_height = 1564.0;
    const float file_width = 2332.0;
public:
    // int x_0;
    // int y_0;
    // Color color;
    // sf::Sprite sprite;
    // sf::Texture texture;

    SW_Window_Mgr* sw_window_mgr;
    std::string file = src_dir "MacWindow.png";


    // Simple_Window(int width_, int height_,  int x_0_,  int y_0_, const Color &color_) :
    //            Window(width_,     height_), x_0(x_0_), y_0(y_0_),       color(color_) {
    //             set_sprite(color);
    //            }
    Simple_Window(int width_, int height_, int x_0_, int y_0_, const Color &color_/*, SW_Window_Mgr* mgr*/) :
               Window(width_,     height_,     x_0_,     y_0_,              color_)/*,  sw_window_mgr(mgr) */{
                this -> sw_window_mgr = new SW_Window_Mgr();
                set_sprite(color);
                set_windows();
               }

    bool action_pressed(sf::Vector2i mouse) {
        return 0;
    }


    void set_sprite(Color &color) {
        if(!texture.loadFromFile(file)) {
            throw std::runtime_error("ошибка открытия файла > " + file + "!");
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, file_width, file_height));
        sprite.setScale(width / file_width, height / file_height);
        sprite.setColor(sf::Color(color.R, color.G, color.B, color.T));
        sprite.setPosition(x_0, y_0);
    }

    void set_windows() {
        Color color(255, 255, 255);
        color.T = 255;

        sw_window_mgr -> sw_window_mgr.push_back(std::make_shared<Button_Close>(5, 5, x_0 + 22, y_0 + 9, color, src_dir "Button_3.png", sw_window_mgr -> sw_window_mgr));
        //sw_window_mgr -> sw_window_mgr.push_back(std::make_shared<Button_Close>(10, 10, x_0 + 52, y_0 + 59, color, "Button_3.png", sw_window_mgr -> sw_window_mgr));
    }

    void draw(sf::RenderWindow &window) {
        sw_window_mgr -> sw_window_mgr.clear();
        set_windows();
        sprite.setColor(sf::Color(color.R, color.G, color.B, 255));
        sprite.setPosition(x_0, y_0);
        window.draw(sprite);
        sw_window_mgr -> draw(window);
    }

    bool check_mouse(sf::RenderWindow &window) {
        return sw_window_mgr -> check_mouse(window);
        // if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        //     sf::Vector2i position = sf::Mouse::getPosition(window);
        //     for (auto& my_window : sw_window_mgr -> sw_window_mgr) {
        //         // if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        //         //     my_window -> start_drag(position);
        //         //     my_window -> update_position(position);
        //         // } else {
        //             my_window -> action_pressed(position);
        //         // }
        //         my_window -> check_mouse(window);
        //     }
        // }
        // } else {
        //     sf::Vector2i position = sf::Mouse::getPosition(window);
        //     for (auto& my_window : sw_window_mgr -> sw_window_mgr) {
        //         my_window -> stop_drag();
        //     }
        // }
    }

     ~Simple_Window() = default;
};

class Window_Mgr {
public:
    //std::vector<Window*> window_mgr;
    std::vector<std::shared_ptr<Window>> window_mgr;

    Window_Mgr() = default;

    Window_Mgr(std::vector<Molecule*>& molecules) : window_mgr() {
        set_windows(molecules);
    }

    void set_windows(std::vector<Molecule*> &molecules) {
        Color color(255, 255, 255);
        color.T = 255;

        window_mgr.push_back(std::make_shared<Simple_Window>(400, 200, 100, 100, color));
        window_mgr.push_back(std::make_shared<Simple_Window>(400, 200, 400, 400, color));
        window_mgr.push_back(std::make_shared<Molecule_Count_Button>(50, 50, 0, 0, color, "inc", molecules, src_dir "Button_3.png", new Circle_Molecule(), 0));
        window_mgr.push_back(std::make_shared<Molecule_Count_Button>(50, 50, 50, 0, color, "inc", molecules, src_dir "Rect_Button.png", new Square_Molecule(), 0));
        window_mgr.push_back(std::make_shared<Molecule_Count_Button>(50, 50, 0, 50, color, "sub", molecules, src_dir "Button_3.png", new Circle_Molecule(), 1));
        window_mgr.push_back(std::make_shared<Molecule_Count_Button>(50, 50, 50, 50, color, "sub", molecules, src_dir "Rect_Button.png", new Square_Molecule(), 1));
    }

    void check_kb(sf::RenderWindow &window) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.close();
        }
    }

    void check_mouse(sf::RenderWindow &window) {
        sf::Mouse mouse;
        std::vector<size_t> delete_windows_indexes;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            sf::Vector2i position = sf::Mouse::getPosition(window);
            //for (auto& my_window : window_mgr) {
            for (size_t i = 0; i < window_mgr.size(); i++) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                    //my_window -> action_pressed(position);
                    //std::cout << "LOL\n";
                    window_mgr[i] -> start_drag(position);
                    window_mgr[i] -> update_position(position);
                } else {
                    window_mgr[i] -> action_pressed(position);
                }

                if (window_mgr[i] -> check_mouse(window)) {
                    delete_windows_indexes.push_back(i);
                }
            }
        } else {
            sf::Vector2i position = sf::Mouse::getPosition(window);
            for (auto& my_window : window_mgr) {
                my_window -> stop_drag();
            }
        }

        for (auto& delete_i : delete_windows_indexes) {
            window_mgr.erase(window_mgr.begin() + delete_i);
        }
    }

    void check_events(sf::RenderWindow &window) {
        sf::Event event;
        while (window.pollEvent(event)){
            check_mouse(window);
            check_kb(window);
            if (event.type == sf::Event::Closed) {window.close();}
        } //while (window.pollEvent(event));
    }
};



#endif
