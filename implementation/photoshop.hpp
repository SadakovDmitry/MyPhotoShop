#ifndef PHOTOSHOP_HPP
#define PHOTOSHOP_HPP

#include <iostream>
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_photoshop.hpp"

using namespace psapi;

class RootWindow : public psapi::IWindowVector {
public:
    RootWindow();

    virtual void draw  (      psapi::IRenderWindow* renderWindow);
    virtual bool update(const psapi::IRenderWindow* renderWindow,  const psapi::sfm::Event& event);
    virtual int64_t getId() const;

    virtual psapi::vec2i getPos()  const;
    virtual psapi::vec2u getSize() const;
    virtual void setParent(const IWindow* parent);
    virtual void forceActivate();
    virtual void forceDeactivate();
    virtual bool isActive() const;

private:
    bool is_active_ = true;
};

class AWindow : public IWindow {
private:
    wid_t id;
    bool is_active = true;
    vec2u size;
    vec2i pos;
    vec2f scale;
    const IWindow* parent;
public:
    virtual ~AWindow() = default;                                                       // out of standart

    virtual void draw(IRenderWindow* renderWindow) = 0;

    virtual bool update(const IRenderWindow* renderWindow, const Event& event) = 0;

    virtual wid_t getId() const;

    virtual IWindow* getWindowById(wid_t id) = 0;

    virtual const IWindow* getWindowById(wid_t id) const = 0;

    virtual vec2i getPos() const;

    virtual vec2u getSize() const;

    virtual void setParent(const IWindow* parent);

    virtual void forceActivate();

    virtual void forceDeactivate();

    virtual bool isActive() const;

    virtual bool isWindowContainer() const;
};

#endif
