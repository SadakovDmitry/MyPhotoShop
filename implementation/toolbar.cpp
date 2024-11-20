#include "my_sfm.hpp"
#include "toolbar.hpp"
#include <assert.h>
#include <dlfcn.h>
#include "canvas.hpp"
namespace psapi {

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         ABarButton
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void ABarButton::draw(IRenderWindow* renderWindow) {   // different sprites for each state
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

    bool ABarButton::update(const IRenderWindow* renderWindow, const Event& event) {
        vec2i mousePos = sfm::vec2i(event.mouseButton.x, event.mouseButton.y);

        if (event.type == Event::MouseMoved) {
            if (mousePos.x >= getPos().x && mousePos.x <= getPos().x + getSize().x &&
                mousePos.y >= getPos().y && mousePos.y <= getPos().y + getSize().y) {
                setState(ABarButton::State::Hover);
            } else {
                setState(ABarButton::State::Normal);
            }
            return true;
        }

        if (event.type == Event::MouseButtonPressed) {
            if (mousePos.x >= getPos().x && mousePos.x <= getPos().x + getSize().x &&
                mousePos.y >= getPos().y && mousePos.y <= getPos().y + getSize().y) {
                setState(ABarButton::State::Press);
                return true;
            }
        }

        if (event.type == Event::MouseButtonReleased) {
            if (mousePos.x >= getPos().x && mousePos.x <= getPos().x + getSize().x &&
                mousePos.y >= getPos().y && mousePos.y <= getPos().y + getSize().y) {
                setState(ABarButton::State::Released);
                return true;
            }
        }

        return false;
    }

    wid_t ABarButton::getId() const {
        return id;
    }

    IWindow* ABarButton::getWindowById(wid_t id) {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    const IWindow* ABarButton::getWindowById(wid_t id) const {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    vec2i ABarButton::getPos() const {
        return pos;
    }

    vec2u ABarButton::getSize() const {
        return size;
    }

    void ABarButton::setParent(const IWindow* parent) {
       this->parent = parent;
    }

    void ABarButton::forceActivate() {is_active = true;}

    void ABarButton::forceDeactivate() {is_active = false;}

    bool ABarButton::isActive() const { return is_active == true; }

    bool ABarButton::isWindowContainer() const {
        return false;
    }

    void ABarButton::setState(ABarButton::State state) {
        this->state = state;
    }

    ABarButton::State ABarButton::getState() const {
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
            if (button->getState() == ABarButton::State::Press) {
                Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(psapi::kCanvasWindowId));
                canvas->setActiveTool(static_cast<ABarButton*>(button.get()));
                if(canvas->getActiveTool()) {
                    canvas->getActiveTool()->action();
                }
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
    // renderWindow->draw(reinterpret_cast<const sfm::Drawable*>(&(static_cast<const ABarButton*>(button)->sprite)));
}

}
