#ifndef ERASER_HPP
#define ERASER_HPP

//#include "api_photoshop.hpp"
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

// const wid_t kToolBarWindowId    = 101;
// const wid_t kOptionsBarWindowId = 102;

class EraserTool : public ABarButton {
public:
    EraserTool(vec2i pos_, vec2u size_, wid_t id_)
        : ABarButton(pos_, size_, id_)
    {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/Eraser.png")) {
            std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    virtual void action() override;

    virtual ~EraserTool() = default;
};

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}

#endif
