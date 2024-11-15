#ifndef ERASER_HPP
#define ERASER_HPP

//#include "api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/my_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/photoshop.hpp"

#include <cstdint>
#include <memory>
#include <iostream>

namespace psapi {

class ABarButton : public IBarButton {
protected:
    friend class ScrollBar;
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

class ScrollBarVert : public IBar {
private:
    friend class ScrollBar;
    wid_t id;
    bool is_active;
    vec2i pos;
    vec2u size;
    vec2f scale;
    sfm::Sprite sprite;
    sfm::Texture texture;
    const IWindow* parent;
    std::vector<std::unique_ptr<ABarButton>> scrollbar;
    Scrollable* scroll_obj;
public:
    ScrollBarVert(vec2i pos_, vec2u size_, Scrollable* scroll_obj_) : id(kCanvasWindowId - 1), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr), scrollbar(), scroll_obj(scroll_obj_) {
        //texture.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y));
        std::vector<sfm::Color> pix_arr(size.x * size.y, sfm::Color(170, 170, 170, 255));
        texture.loadFromMemory(pix_arr.data(), size.x * size.y, sfm::IntRect(pos.y, pos.y, size.x, size.y));
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(170, 170, 170, 255));
        sprite.setPosition(pos.x, pos.y);
    }
    ~ScrollBarVert() = default;

    virtual void draw(IRenderWindow* renderWindow);

    virtual bool update(const IRenderWindow* renderWindow, const Event& event);

    virtual wid_t getId() const;

    virtual IWindow* getWindowById(wid_t id);

    virtual const IWindow* getWindowById(wid_t id) const;

    virtual vec2i getPos() const;

    virtual vec2u getSize() const;

    virtual void setParent(const IWindow* parent);

    virtual void forceActivate();

    virtual void forceDeactivate();

    virtual bool isActive() const;

    virtual void addWindow(std::unique_ptr<IWindow> window);

    virtual void removeWindow(wid_t id);

    virtual bool isWindowContainer() const;

    virtual ChildInfo getNextChildInfo() const;

    virtual void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const;
};

class ScrollBarSlider : public ABarButton {
private:
    friend class ScrollBarArrUp;
    friend class ScrollBarArrDown;
    bool is_dragging;
    vec2i drag_offset;
    Scrollable* scroll_obj;
public:
    ScrollBarSlider(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_)
        : ABarButton(pos_, size_, id_), is_dragging(false), scroll_obj(scroll_obj_) {
        //texture.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y));
        std::vector<sfm::Color> pix_arr(size.x * size.y, sfm::Color(100, 100, 100, 255));
        texture.loadFromMemory(pix_arr.data(), size.x * size.y, sfm::IntRect(pos.y, pos.y, size.x, size.y));
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(100, 100, 100, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    virtual void action() override ;

    virtual ~ScrollBarSlider() = default;
};

class ScrollBarArrUp : public ABarButton {
    Scrollable* scroll_obj;
public:
    ScrollBarArrUp(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_)
        : ABarButton(pos_, size_, id_), scroll_obj(scroll_obj_) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/arrow_up.jpg")) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(100, 100, 100, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    virtual void action() override ;

    virtual ~ScrollBarArrUp() = default;
};

class ScrollBarArrDown : public ABarButton {
    Scrollable* scroll_obj;
public:
    ScrollBarArrDown(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_)
        : ABarButton(pos_, size_, id_), scroll_obj(scroll_obj_) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/arrow_down.jpg")) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(100, 100, 100, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    virtual void action() override ;

    virtual ~ScrollBarArrDown() = default;
};

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}
#endif
