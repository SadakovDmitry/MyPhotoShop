#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

//#include "api_photoshop.hpp"
// #include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/toolbar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/my_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/canvas.hpp"

#include <cstdint>
#include <memory>
#include <iostream>

namespace psapi {
class ColorPaletteWindow;
class ChoiceThicknessWindow;
// const wid_t kToolBarWindowId    = 101;
// const wid_t kOptionsBarWindowId = 102;

class PencilTool : public ABarButton {
private:
    ColorPaletteWindow* colorPalette;
    ChoiceThicknessWindow* thicknessWindow;
    std::vector<vec2i> points_arr;
    sfm::Color color;
public:
    PencilTool(vec2i pos_, vec2u size_, wid_t id_, ColorPaletteWindow* colorPalette_, ChoiceThicknessWindow* thicknessWindow_)
        : ABarButton(pos_, size_, id_),  points_arr(), color({0, 0, 0, 255}), colorPalette(colorPalette_), thicknessWindow(thicknessWindow_) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/Pencil.png")) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
        assert(colorPalette);
        //colorPalette = std::make_unique<ColorPaletteWindow>(vec2i{105, 5}, vec2u{150, 150}, 200);
    }

    virtual void action() override;
    sfm::Color getColor();
    void setColor(sfm::Color);

    double CatmullRom(double p0, double p1, double p2, double p3, double t);
    vec2i operator() (double t);

    virtual ~PencilTool() = default;
};

class ChoiceThicknessWindow : public ABarButton {
private:
    size_t thickness;
public:

    ChoiceThicknessWindow(vec2i pos_, vec2u size_, wid_t id_)
        : ABarButton(pos_, size_, id_), thickness(3) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/palette.jpg")) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    virtual void action() override;
    size_t getThickness();
    void setThickness(size_t thickness_);

    virtual ~ChoiceThicknessWindow() = default;
};

class ColorPaletteWindow : public ABarButton {
private:
    sfm::Color selectedColor;
    sfm::Image image;
public:

    ColorPaletteWindow(vec2i pos_, vec2u size_, wid_t id_)
        : ABarButton(pos_, size_, id_), selectedColor({0, 0, 0, 255}) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/palette.jpg")) {
             std::cerr << "Error opening file\n";
        }
        if (!image.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/palette.jpg")) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    virtual void action() override;
    sfm::Color getColor();
    void setColor(sfm::Color);

    virtual ~ColorPaletteWindow() = default;
};


extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}

#endif
