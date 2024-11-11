#ifndef ERASER_HPP
#define ERASER_HPP

//#include "api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/sfm_prot.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_photoshop.hpp"
//#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/photoshop.hpp"

#include <cstdint>
#include <memory>
#include <iostream>

namespace psapi {

// const wid_t kToolBarWindowId    = 101;
// const wid_t kOptionsBarWindowId = 102;

class ABarButton : public IBarButton {
protected:
    friend class ToolBar;
    wid_t id;
    bool is_active = true;
    sfm::Sprite sprite;
    sfm::Texture texture;
    vec2u size;
    vec2i pos;
    vec2f scale;
    State state;
    const IWindow* parent;
public:
    // ABarButton(vec2i pos_, vec2u size_, wid_t id_) : id(id_), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr) {
    //     // if(!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/Pencil.png")) {
    //     //     //throw std::runtime_error("ошибка открытия файла > " + file + "!");
    //     // }
    //     // sprite.setTexture(&texture);
    //     // sprite.setTextureRect(sfm::IntRect(0, 0, 50, 50));
    //     // sprite.setScale(1, 1);
    //     // sprite.setColor(sfm::Color(255, 255, 255, 255));
    //     // sprite.setPosition(5, 5);
    // }
    ABarButton(vec2i pos_, vec2u size_, wid_t id_) : id(id_), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr) {}
    ABarButton() = default;

    virtual ~ABarButton() = default;

    virtual void draw(IRenderWindow* renderWindow) override;

    virtual bool update(const IRenderWindow* renderWindow, const Event& event) override;

    virtual wid_t getId() const override;

    virtual IWindow* getWindowById(wid_t id) override;

    virtual const IWindow* getWindowById(wid_t id) const override;

    virtual vec2i getPos() const override;

    virtual vec2u getSize() const override;

    virtual void setParent(const IWindow* parent) override;

    virtual void forceActivate() override;

    virtual void forceDeactivate() override;

    virtual bool isActive() const override;

    virtual bool isWindowContainer() const override;

    virtual void setState(State state) override;
    virtual State getState() const override;

    virtual void action() = 0;
};

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
