#ifndef BUTTONS_GUARD
#define BUTTONS_GUARD

const int CH_COL_SPEED    = 1;
const int ADD_BUT_WIDTH   = 50;
const int ADD_BUT_HEIGHT  = 50;
const bool SUB = 1;
const bool INC = 0;

#include "Window_Mgr.h"
#include "Scene.h"
#include <iostream>


class Button : public Window{
public:
    sf::Sprite sprite_pushed;
    sf::Texture texture_pushed;
    // sf::Text str;
    // sf::Font font;

    Button(int width_, int height_, int x_0_, int y_0_, const Color &color_) :
        Window(width_,     height_,     x_0_,     y_0_,              color_) {}


    // virtual void draw(sf::RenderWindow &window)   = 0;
    // virtual void set_sprite(Color &color)         = 0;
    virtual void set_sprite_pushed(Color &color)     = 0;
    virtual void set_text(std::string str)           = 0;
    virtual void action_guidance(sf::Vector2i mouse) = 0;
    //virtual void action_pressed(sf::Vector2i mouse)  = 0;
};

class Molecule_Count_Button : public Button {
private:
    const float file_height = 50.0;
    const float file_width  = 50.0;
public:
    sf::Text str;
    sf::Font font;
    std::string file;
    std::string text;
    std::vector<Molecule*> &molecules;
    Molecule* molecule;
    bool inc_or_sub;

    Molecule_Count_Button(int width_, int height_, int x_0_, int y_0_, const Color &color_, std::string text_,
                    std::vector<Molecule*> &molecules_, std::string file_, Molecule* molecule_,        bool inc_or_sub_) :
                       Button(width_,     height_,     x_0_,     y_0_,              color_),       text(text_),
                                  molecules(molecules_),       file(file_), molecule(molecule_), inc_or_sub(inc_or_sub_){
        set_sprite(color);
        set_sprite_pushed(color);
        set_text(text);
    }

    void draw(sf::RenderWindow &window) {
        sprite.setColor(sf::Color(color.R, color.G, color.B, color.T));
        sprite_pushed.setColor(sf::Color(color.R, color.G, color.B, 255 - color.T));
        sprite.setPosition(x_0, y_0);
        str.setPosition(x_0 + (width - text.length()) / 3, y_0 + 12);
        window.draw(sprite);
        window.draw(sprite_pushed);
        window.draw(str);
    }

    bool action_pressed(sf::Vector2i mouse) {
        if(mouse.x > x_0 && mouse.x < x_0 + width && mouse.y > y_0 && mouse.y < y_0 + height){
            if(inc_or_sub) {
                if(molecules.size() > 0){ molecules.pop_back(); }
            } else {
                Molecule* new_molecule = molecule -> add_molecule();
                molecules.push_back(new_molecule);
            }
        }
        return 0;
    }

    void action_guidance(sf::Vector2i mouse) {
        if(mouse.x > x_0 && mouse.x < x_0 + width && mouse.y > y_0 && mouse.y < y_0 + height){
            //nothing
        }
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

    void set_sprite_pushed(Color &color) {
        if(!texture_pushed.loadFromFile(src_dir "Button_3.png")) {
            throw std::runtime_error("ошибка открытия файла > " + file + "!");
        }
        sprite_pushed.setTexture(texture_pushed);
        sprite_pushed.setTextureRect(sf::IntRect(0, 0, width, height));
        sprite_pushed.setColor(sf::Color(color.R, color.G, color.B, 255 - color.T));
        sprite_pushed.setPosition(x_0, y_0);
    }

    void set_text(std::string text) {
        if(!font.loadFromFile(src_dir "Zamyatin.ttf")) {
            throw std::runtime_error("ошибка открытия файла > " + file + "!");
        }
        str.setFont(font);
        str.setFillColor(sf::Color::Black);
        str.setCharacterSize(20);
        str.setPosition(x_0 + (width - text.length()) / 3, y_0 + 12);
        str.setString(text);
    }

    void set_windows() {};

    bool check_mouse(sf::RenderWindow &window) {
        return false;
    }

    ~Molecule_Count_Button() {}
};

class Button_Close : public Button {
private:
    const float file_height = 50.0;
    const float file_width  = 50.0;
public:
    std::vector<std::shared_ptr<Window>>& window_mgr;
    std::string file;

    Button_Close(int width_, int height_, int x_0_, int y_0_, const Color &color_, std::string file_, std::vector<std::shared_ptr<Window>>& window_mgr_) :
        Button(      width_,     height_,     x_0_,     y_0_,              color_),       file(file_),                           window_mgr(window_mgr_){
        set_sprite(color);
        set_sprite_pushed(color);
    }

    void draw(sf::RenderWindow &window) {
        sprite.setColor(sf::Color(color.R, color.G, color.B, color.T));
        sprite_pushed.setColor(sf::Color(color.R, color.G, color.B, 255 - color.T));
        sprite.setPosition(x_0, y_0);
        window.draw(sprite);
        window.draw(sprite_pushed);
    }

    bool action_pressed(sf::Vector2i mouse) {
        if(mouse.x > x_0 && mouse.x < x_0 + width && mouse.y > y_0 && mouse.y < y_0 + height){
            window_mgr.clear();
            std::cout << "CLEAN\n";
            return true;
        }
        return false;
    }

    void action_guidance(sf::Vector2i mouse) {
        if(mouse.x > x_0 && mouse.x < x_0 + width && mouse.y > y_0 && mouse.y < y_0 + height){
            //nothing
        }
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

    void set_sprite_pushed(Color &color) {
        texture_pushed.loadFromFile(src_dir "Button_3.png");
        sprite_pushed.setTexture(texture_pushed);
        sprite_pushed.setTextureRect(sf::IntRect(0, 0, width, height));
        sprite_pushed.setColor(sf::Color(color.R, color.G, color.B, 255 - color.T));
        sprite_pushed.setPosition(x_0, y_0);
    }

    void set_text(std::string str) {}

    void set_windows() {};

    bool check_mouse(sf::RenderWindow &window) {
        return false;
    }

    ~Button_Close() {}
};

class Buttons_Manager {
public:
    std::vector<Button*> buttons;

    Buttons_Manager(std::vector<Molecule*>& molecules) : buttons() {
        fill_button_vector(molecules);
    }

    // void show_buttons(sf::RenderWindow &window){
    //     for (Button* button : buttons){
    //         assert(button);
    //         button -> sprite.setColor(sf::Color(button -> color.R, button -> color.G, button -> color.B, button -> color.T));
    //         button -> sprite_pushed.setColor(sf::Color(button -> color.R, button -> color.G, button -> color.B, 255 - button -> color.T));
    //         window.draw(button -> sprite);
    //         window.draw(button -> sprite_pushed);
    //         window.draw(button -> str);
    //     }
    // }

    void fill_button_vector(std::vector<Molecule*> &molecules){
        Color color(255, 255, 255);
        color.T = 255;

        Molecule_Count_Button* inc_R = new Molecule_Count_Button(50, 50, 0, 0, color, "inc", molecules, src_dir "Button_3.png", new Circle_Molecule(), 0);
        buttons.push_back(inc_R);
        Molecule_Count_Button* inc_P = new Molecule_Count_Button(50, 50, 50, 0, color, "inc", molecules, src_dir "Rect_Button.png", new Square_Molecule(), 0);
        buttons.push_back(inc_P);
        Molecule_Count_Button* sub_R = new Molecule_Count_Button(50, 50, 0, 50, color, "sub", molecules, src_dir "Button_3.png", new Circle_Molecule(), 1);
        buttons.push_back(sub_R);
        Molecule_Count_Button* sub_P = new Molecule_Count_Button(50, 50, 50, 50, color, "sub", molecules, src_dir "Rect_Button.png", new Square_Molecule(), 1);
        buttons.push_back(sub_P);
    }

    void check_kb(sf::RenderWindow &window) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.close();
        }
    }

    void check_mouse(sf::RenderWindow &window) {
        sf::Mouse mouse;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            sf::Vector2i position = sf::Mouse::getPosition(window);
            for(Button* button : buttons){
                assert(button);
                button -> action_pressed(position);
            }
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

    ~Buttons_Manager() {}
};

#endif
