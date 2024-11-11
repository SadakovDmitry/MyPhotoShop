// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "scrollbar_vert.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {
    const int kScrollBarWindowId  = kToolBarWindowId + 10;
    typedef IWindowContainer* (*RootWindow)();

    IWindowContainer* getRootWindowptr() {
        void* photoshopLib = dlopen("/Users/dima/MIPT/SecondSem/MyPaint/Standard/libphotoshop.dylib", RTLD_LAZY);
        if (!photoshopLib) {
            std::cerr << "Failed to load photoshop dylib in pencil: " << dlerror() << "\n";
        }
        RootWindow getRootWindowlib = reinterpret_cast<RootWindow>(dlsym(photoshopLib, "getRootWindow"));
        if (!getRootWindowlib) {
            std::cerr << "Failed to locate functions in photoshop dylib in pencil: " << dlerror() << "\n";
            dlclose(photoshopLib);
        }
        if (!getRootWindowlib()) {
            std::cerr << "Failed to initialize photoshop in pencil.\n";
            dlclose(photoshopLib);
        }

        return getRootWindowlib();
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          ABarButton
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

    void ABarButton::forceActivate() {is_active = false;}

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
//                                                                          ScrollBarVert
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void ScrollBarVert::draw(IRenderWindow* renderWindow) {
        renderWindow->draw(&sprite);
        for (const auto& button : scrollbar) {
            button->draw(renderWindow);
        }

        //std::cout << "draw ScrollBarVert\n";
    }

    bool ScrollBarVert::update(const IRenderWindow* renderWindow, const Event& event) {
        for (auto& button : scrollbar) {
            if (button->update(renderWindow, event)) {
                //if (button->getState() == ABarButton::State::Press) {
                    button->action();
                //     ICanvas* canvas = static_cast<ICanvas*>(getRootWindow()->getWindowById(psapi::kCanvasWindowId));
                //     canvas->setActiveTool(static_cast<ABarButton*>(button.get()));
                //}
                return true;
            }
        }
        return false;
    }

    wid_t ScrollBarVert::getId() const { return id; }

    IWindow* ScrollBarVert::getWindowById(wid_t id) {
        if (this->id == id) return this;
        for (auto& button : scrollbar) {
            IWindow* found = button->getWindowById(id);
            if (found) return found;
        }
        return nullptr;
    }

    const IWindow* ScrollBarVert::getWindowById(wid_t id) const {
        if (this->id == id) return this;
        for (const auto& button : scrollbar) {
            const IWindow* found = button->getWindowById(id);
            if (found) return found;
        }
        return nullptr;
    }

    vec2i ScrollBarVert::getPos() const { return pos; }
    vec2u ScrollBarVert::getSize() const { return size; }
    void ScrollBarVert::setParent(const IWindow* parent) { this->parent = parent; }
    void ScrollBarVert::forceActivate() { is_active = true; }
    void ScrollBarVert::forceDeactivate() { is_active = false; }
    bool ScrollBarVert::isActive() const { return is_active == true; }
    bool ScrollBarVert::isWindowContainer() const { return true; }

    void ScrollBarVert::addWindow(std::unique_ptr<IWindow> window) {
        scrollbar.push_back(std::unique_ptr<ABarButton>(dynamic_cast<ABarButton*>(window.release())));

    }

    void ScrollBarVert::removeWindow(wid_t id) {
        scrollbar.erase(
            std::remove_if(
                scrollbar.begin(),
                scrollbar.end(),
                [id](const std::unique_ptr<ABarButton>& button) {
                    return button->getId() == id;
                }
            ),
            scrollbar.end()
        );
    }

    ChildInfo ScrollBarVert::getNextChildInfo() const {
        ChildInfo info;
        int totalWidth = 0;
        for (const auto& button : scrollbar) {
            totalWidth += button->getSize().x;
        }
        info.pos = vec2i(pos.x + totalWidth % size.x, pos.y + totalWidth / size.x * 50);
        info.size = vec2u(50, 50);
        return info;
    }

    void ScrollBarVert::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const {
        // renderWindow->draw(reinterpret_cast<const sfm::Drawable*>(&(static_cast<const BarButton*>(button)->sprite)));
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      ScrollBarTools
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------


    void ScrollBarSlider::action() {
        srand(time(0));
        ICanvas* canvas = static_cast<ICanvas*>(getRootWindowptr()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();
        mouse_pos.x -= canvas_pos.x;
        mouse_pos.y -= canvas_pos.y;
        vec2i cur_pos;

        if (canvas->isPressed()) {
            // shift canvas
            //this->pos.y = mouse_pos.y;
        }
    }

    void ScrollBarArrUp::action() {
        ScrollBarVert* scrollbar = static_cast<ScrollBarVert*>(getRootWindowptr()->getWindowById(kScrollBarWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (this->state == ABarButton::State::Press) {
            slider->pos.y -= 5;
            slider->sprite.setPosition(slider->pos.x, slider->pos.y);
        }
    }

    void ScrollBarArrDown::action() {
        ScrollBarVert* scrollbar = static_cast<ScrollBarVert*>(getRootWindowptr()->getWindowById(kScrollBarWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (this->state == ABarButton::State::Press) {
            slider->pos.y += 5;
            slider->sprite.setPosition(slider->pos.x, slider->pos.y);
        }
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      Load and Unload
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {

            auto canvas = static_cast<ICanvas*>(getRootWindowptr()->getWindowById(psapi::kCanvasWindowId));
            vec2u size = {20, canvas->getSize().y};
            vec2i pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x - size.x),
                         static_cast<int>(canvas->getPos().y)};
            auto scrollbar = std::make_unique<ScrollBarVert>(pos, size);
            getRootWindowptr()->addWindow(std::move(scrollbar));

            auto toolbar = static_cast<IBar*>(getRootWindowptr()->getWindowById(kToolBarWindowId + 10));
            assert(toolbar);

            size = {toolbar->getSize().x, 40};
            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x - size.x),
                  (static_cast<int>(canvas->getPos().y + canvas->getSize().y - size.y) / 2)};
            auto slider = std::make_unique<ScrollBarSlider>(pos, size, 1);

            size = {toolbar->getSize().x, toolbar->getSize().x};
            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x - size.x),
                   static_cast<int>(canvas->getPos().y)};
            auto arr_up = std::make_unique<ScrollBarArrUp>(pos, size, 2);

            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x - size.x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y - size.y)};
            auto arr_down = std::make_unique<ScrollBarArrDown>(pos, size, 3);

            toolbar->addWindow(std::move(slider));
            toolbar->addWindow(std::move(arr_up));
            toolbar->addWindow(std::move(arr_down));

            assert(canvas);
            return true;
        }

        __attribute__((visibility("default"))) void unloadPlugin() {
            IBar* toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            if (toolbar) {
                toolbar->removeWindow(4);
            }
        }
    }
}

