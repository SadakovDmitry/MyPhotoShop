#include "my_sfm.hpp"
#include "toolbar.hpp"
#include "canvas.hpp"
#include <assert.h>
#include <dlfcn.h>

namespace psapi {

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         BarButton
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void BarButton::draw(IRenderWindow* renderWindow) {   // different sprites for each state
        //std::cout << "draw button\n";
        renderWindow->draw(&sprite);
        switch (getState()) {
            case State::Hover:
                // std::cout << "Hover button\n";
                renderWindow->draw(&sprite);
                break;
            case State::Press:
                // std::cout << "Press button\n";
                renderWindow->draw(&sprite);
                break;
            case State::Released:
                // std::cout << "Released button\n";
                renderWindow->draw(&sprite);
                break;
            case State::Normal:
                // std::cout << "Normal button\n";
            default:
                // std::cout << "default button\n";
                renderWindow->draw(&sprite);
                break;
        }
    }

    bool BarButton::update(const IRenderWindow* renderWindow, const Event& event) {
        vec2i mousePos = sfm::vec2i(event.mouseButton.x, event.mouseButton.y);

        if (event.type == Event::MouseMoved) {
            if (mousePos.x >= getPos().x && mousePos.x <= getPos().x + getSize().x &&
                mousePos.y >= getPos().y && mousePos.y <= getPos().y + getSize().y) {
                setState(BarButton::State::Hover);
            } else {
                setState(BarButton::State::Normal);
            }
            return true;
        }

        if (event.type == Event::MouseButtonPressed) {
            if (mousePos.x >= getPos().x && mousePos.x <= getPos().x + getSize().x &&
                mousePos.y >= getPos().y && mousePos.y <= getPos().y + getSize().y) {
                setState(BarButton::State::Press);
                return true;
            }
        }

        if (event.type == Event::MouseButtonReleased) {
            if (mousePos.x >= getPos().x && mousePos.x <= getPos().x + getSize().x &&
                mousePos.y >= getPos().y && mousePos.y <= getPos().y + getSize().y) {
                setState(BarButton::State::Released);
                return true;
            }
        }

        return false;
    }

    wid_t BarButton::getId() const {
        return id;
    }

    IWindow* BarButton::getWindowById(wid_t id) {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    const IWindow* BarButton::getWindowById(wid_t id) const {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    vec2i BarButton::getPos() const {
        return pos;
    }

    vec2u BarButton::getSize() const {
        return size;
    }

    void BarButton::setParent(const IWindow* parent) {
       this->parent = parent;
    }

    void BarButton::forceActivate() {is_active = false;}

    void BarButton::forceDeactivate() {is_active = false;}

    bool BarButton::isActive() const { return is_active == true; }

    bool BarButton::isWindowContainer() const {
        return false;
    }

    void BarButton::setState(BarButton::State state) {
        this->state = state;
    }

    BarButton::State BarButton::getState() const {
        return state;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          ToolBar
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ToolBar::ToolBar(vec2i pos_, vec2i size_) : pos(pos_), size(size_) {}

void ToolBar::draw(IRenderWindow* renderWindow) {

    renderWindow->draw(&sprite);
    for (const auto& button : toolbar) {
        button->draw(renderWindow);
    }

    //std::cout << "draw toolbar\n";
}

bool ToolBar::update(const IRenderWindow* renderWindow, const Event& event) {
    for (auto& button : toolbar) {
        if (button->update(renderWindow, event)) {
            if (button->getState() == BarButton::State::Press) {
                Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(psapi::kCanvasWindowId));
                canvas->setActiveTool(static_cast<BarButton*>(button.get()));
            }
            return true;
        }
    }
    return false;
}

wid_t ToolBar::getId() const { return id; }

IWindow* ToolBar::getWindowById(wid_t id) {
    if (this->id == id) return this;
    for (auto& button : toolbar) {
        IWindow* found = button->getWindowById(id);
        if (found) return found;
    }
    return nullptr;
}

const IWindow* ToolBar::getWindowById(wid_t id) const {
    if (this->id == id) return this;
    for (const auto& button : toolbar) {
        const IWindow* found = button->getWindowById(id);
        if (found) return found;
    }
    return nullptr;
}

vec2i ToolBar::getPos() const { return pos; }
vec2u ToolBar::getSize() const { return size; }
void ToolBar::setParent(const IWindow* parent) { this->parent = parent; }
void ToolBar::forceActivate() { is_active = true; }
void ToolBar::forceDeactivate() { is_active = false; }
bool ToolBar::isActive() const { return is_active == true; }
bool ToolBar::isWindowContainer() const { return true; }

void ToolBar::addWindow(std::unique_ptr<IWindow> window) {
    toolbar.push_back(std::unique_ptr<psapi::IBarButton>(dynamic_cast<psapi::IBarButton*>(window.release())));

}

void ToolBar::removeWindow(wid_t id) {
    toolbar.erase(
        std::remove_if(
            toolbar.begin(),
            toolbar.end(),
            [id](const std::unique_ptr<IBarButton>& button) {
                return button->getId() == id;
            }
        ),
        toolbar.end()
    );
}

ChildInfo ToolBar::getNextChildInfo() const {
    ChildInfo info;
    int totalWidth = 0;
    for (const auto& button : toolbar) {
        totalWidth += button->getSize().x;
    }
    info.pos = vec2i(pos.x + totalWidth % size.x, pos.y + totalWidth / size.x * 50);
    info.size = vec2u(50, 50);
    return info;
}

void ToolBar::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const {
    // renderWindow->draw(reinterpret_cast<const sfm::Drawable*>(&(static_cast<const BarButton*>(button)->sprite)));
}

}
