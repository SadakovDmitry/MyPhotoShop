// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "scrollbar_gor.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {
    const int kScrollBarGorWindowId  = kToolBarWindowId + 11;
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
//                                                                          ScrollBar
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void ScrollBarGor::draw(IRenderWindow* renderWindow) {
        renderWindow->draw(&sprite);
        for (const auto& button : scrollbar) {
            button->draw(renderWindow);
        }

        //std::cout << "draw ScrollBar\n";
    }

    bool ScrollBarGor::update(const IRenderWindow* renderWindow, const Event& event) {
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

    wid_t ScrollBarGor::getId() const { return id; }

    IWindow* ScrollBarGor::getWindowById(wid_t id) {
        if (this->id == id) return this;
        for (auto& button : scrollbar) {
            IWindow* found = button->getWindowById(id);
            if (found) return found;
        }
        return nullptr;
    }

    const IWindow* ScrollBarGor::getWindowById(wid_t id) const {
        if (this->id == id) return this;
        for (const auto& button : scrollbar) {
            const IWindow* found = button->getWindowById(id);
            if (found) return found;
        }
        return nullptr;
    }

    vec2i ScrollBarGor::getPos() const { return pos; }
    vec2u ScrollBarGor::getSize() const { return size; }
    void ScrollBarGor::setParent(const IWindow* parent) { this->parent = parent; }
    void ScrollBarGor::forceActivate() { is_active = true; }
    void ScrollBarGor::forceDeactivate() { is_active = false; }
    bool ScrollBarGor::isActive() const { return is_active == true; }
    bool ScrollBarGor::isWindowContainer() const { return true; }

    void ScrollBarGor::addWindow(std::unique_ptr<IWindow> window) {
        scrollbar.push_back(std::unique_ptr<ABarButton>(dynamic_cast<ABarButton*>(window.release())));

    }

    void ScrollBarGor::removeWindow(wid_t id) {
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

    ChildInfo ScrollBarGor::getNextChildInfo() const {
        ChildInfo info;
        int totalWidth = 0;
        for (const auto& button : scrollbar) {
            totalWidth += button->getSize().x;
        }
        info.pos = vec2i(pos.x + totalWidth % size.x, pos.y + totalWidth / size.x * 50);
        info.size = vec2u(50, 50);
        return info;
    }

    void ScrollBarGor::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const {
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
        vec2u canvas_size   = canvas->getSize();
        // mouse_pos.x -= canvas_pos.x;
        // mouse_pos.y -= canvas_pos.y;
        vec2i cur_pos;

        if (this->state == ABarButton::State::Released) {
            // shift canvas
            //this->pos.y = mouse_pos.y;
            is_dragging = false;
            //std::cout << "stop dragging released\n";
        }
        if (this->state == ABarButton::State::Press) {
            // shift canvas
            ScrollBarGor* scrollbar = static_cast<ScrollBarGor*>(getRootWindowptr()->getWindowById(kScrollBarGorWindowId));
            //this->pos.y = mouse_pos.y;
            int slider_pos_x = pos.x;
            float shift_coef  = static_cast<float>(slider_pos_x - canvas_pos.x) / canvas_size.x;
            int shift_x = shift_coef * (1800 - canvas_size.x);
            is_dragging = true;
            drag_offset.x = mouse_pos.x - pos.x - shift_x;
            drag_offset.y = mouse_pos.y - pos.y;
            //std::cout << "start dragging\n";
            //window_mgr[i] -> start_drag(position);
            //window_mgr[i] -> update_position(position);
        }
        if (is_dragging) {
            // shift canvas
            ScrollBarGor* scrollbar = static_cast<ScrollBarGor*>(getRootWindowptr()->getWindowById(kScrollBarGorWindowId));
            //this->pos.y = mouse_pos.y;
            int slider_pos_x = pos.x;
            float shift_coef  = static_cast<float>(slider_pos_x - canvas_pos.x) / canvas_size.x;
            int shift_x = shift_coef * (1800 - canvas_size.x);
            pos.x = mouse_pos.x - drag_offset.x - shift_x;
            sprite.setPosition(pos.x, pos.y);
            //std::cout << "dragging\n";
            //window_mgr[i] -> start_drag(position);
            //window_mgr[i] -> update_position(position);
        }
    }

    void ScrollBarArrRight::action() {
        ScrollBarGor* scrollbar = static_cast<ScrollBarGor*>(getRootWindowptr()->getWindowById(kScrollBarGorWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (this->state == ABarButton::State::Press) {
            slider->pos.x += 5;
            slider->sprite.setPosition(slider->pos.x, slider->pos.y);
        }
    }

    void ScrollBarArrLeft::action() {
        ScrollBarGor* scrollbar = static_cast<ScrollBarGor*>(getRootWindowptr()->getWindowById(kScrollBarGorWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (this->state == ABarButton::State::Press) {
            slider->pos.x -= 5;
            slider->sprite.setPosition(slider->pos.x, slider->pos.y);
        }
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      Load and Unload
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {

            auto canvas = static_cast<ICanvas*>(getRootWindowptr()->getWindowById(kCanvasWindowId));
            vec2u size = {canvas->getSize().x, 20};
            vec2i pos = {static_cast<int>(canvas->getPos().x),
                         static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            auto scrollbar = std::make_unique<ScrollBarGor>(pos, size);
            getRootWindowptr()->addWindow(std::move(scrollbar));

            auto toolbar = static_cast<IBar*>(getRootWindowptr()->getWindowById(kToolBarWindowId + 11));
            assert(toolbar);

            size = {40, toolbar->getSize().y};
            pos = {static_cast<int>(canvas->getPos().x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            auto slider = std::make_unique<ScrollBarSlider>(pos, size, 1);

            size = {toolbar->getSize().y, toolbar->getSize().y};
            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x - size.x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            auto arr_up = std::make_unique<ScrollBarArrRight>(pos, size, 2);

            pos = {static_cast<int>(canvas->getPos().x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            auto arr_down = std::make_unique<ScrollBarArrLeft>(pos, size, 3);

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

