// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "scrollbar_gor.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {
    const int kScrollBarHorWindowId  = kToolBarWindowId + 11;
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
//                                                                          ScrollBar
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void ScrollBarHor::draw(IRenderWindow* renderWindow) {
        renderWindow->draw(&sprite);
        for (const auto& button : scrollbar) {
            button->draw(renderWindow);
        }

        //std::cout << "draw ScrollBar\n";
    }

    bool ScrollBarHor::update(const IRenderWindow* renderWindow, const Event& event) {
        for (auto& button : scrollbar) {
            if (button->update(renderWindow, event)) {
                //if (button->getState() == ABarButton::State::Press) {
                    button->action();
                //     ICanvas* canvas = static_cast<ICanvas*>(getRootWindow()->getWindowById(psapi::kCanvasWindowId));
                //     canvas->setActiveTool(nullptr);
                //}
                return true;
            }
        }
        return false;
    }

    wid_t ScrollBarHor::getId() const { return id; }

    IWindow* ScrollBarHor::getWindowById(wid_t id) {
        if (this->id == id) return this;
        for (auto& button : scrollbar) {
            IWindow* found = button->getWindowById(id);
            if (found) return found;
        }
        return nullptr;
    }

    const IWindow* ScrollBarHor::getWindowById(wid_t id) const {
        if (this->id == id) return this;
        for (const auto& button : scrollbar) {
            const IWindow* found = button->getWindowById(id);
            if (found) return found;
        }
        return nullptr;
    }

    vec2i ScrollBarHor::getPos() const { return pos; }
    vec2u ScrollBarHor::getSize() const { return size; }
    void ScrollBarHor::setParent(const IWindow* parent) { this->parent = parent; }
    void ScrollBarHor::forceActivate() { is_active = true; }
    void ScrollBarHor::forceDeactivate() { is_active = false; }
    bool ScrollBarHor::isActive() const { return is_active == true; }
    bool ScrollBarHor::isWindowContainer() const { return true; }

    void ScrollBarHor::addWindow(std::unique_ptr<IWindow> window) {
        scrollbar.push_back(std::unique_ptr<ABarButton>(dynamic_cast<ABarButton*>(window.release())));

    }

    void ScrollBarHor::removeWindow(wid_t id) {
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

    ChildInfo ScrollBarHor::getNextChildInfo() const {
        ChildInfo info;
        int totalWidth = 0;
        for (const auto& button : scrollbar) {
            totalWidth += button->getSize().x;
        }
        info.pos = vec2i(pos.x + totalWidth % size.x, pos.y + totalWidth / size.x * 50);
        info.size = vec2u(50, 50);
        return info;
    }

    void ScrollBarHor::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const {
        // renderWindow->draw(reinterpret_cast<const sfm::Drawable*>(&(static_cast<const BarButton*>(button)->sprite)));
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      ScrollBarTools
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------


    void ScrollBarSlider::action() {
        Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();
        vec2u canvas_size  = canvas->getSize();

        vec2i cur_pos;

        if (this->state == ABarButton::State::Released) {
            is_dragging = false;
        }
        if (this->state == ABarButton::State::Press) {
            is_dragging = true;
            drag_offset.x = mouse_pos.x + canvas->layer_pos.x - pos.x;
            scroll_obj->setShift({canvas_pos.x - canvas->layer_pos.x, canvas_pos.y - canvas->layer_pos.y});
        }
        else if (is_dragging) {

            int old_slider_pos_x = pos.x;
            int new_slider_pos_x = mouse_pos.x + canvas->layer_pos.x - drag_offset.x;

            new_slider_pos_x = std::min(static_cast<int>(canvas_pos.x + canvas_size.x - size.x),
                    std::max(new_slider_pos_x, canvas_pos.x));
            int slider_shift_x = (new_slider_pos_x - old_slider_pos_x);
            int layer_shift_x = slider_shift_x * (canvas->layer_size.x / canvas_size.x);
            pos.x = new_slider_pos_x;
            sprite.setPosition(pos.x, pos.y);
            scroll_obj->scroll({layer_shift_x, 0});
        }
    }

    void ScrollBarArrRight::action() {
        ScrollBarHor* scrollbar = static_cast<ScrollBarHor*>(getRootWindow()->getWindowById(kScrollBarHorWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (this->state == ABarButton::State::Press) {
            slider->pos.x += 5;
            scroll_obj->scroll({5, 0});
            slider->sprite.setPosition(slider->pos.x, slider->pos.y);
        }
    }

    void ScrollBarArrLeft::action() {
        ScrollBarHor* scrollbar = static_cast<ScrollBarHor*>(getRootWindow()->getWindowById(kScrollBarHorWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (this->state == ABarButton::State::Press) {
            slider->pos.x -= 5;
            scroll_obj->scroll({-5, 0});
            slider->sprite.setPosition(slider->pos.x, slider->pos.y);
        }
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      Load and Unload
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {

            auto canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
            vec2u size = {canvas->getSize().x, 20};
            vec2i pos = {static_cast<int>(canvas->getPos().x),
                         static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            Scrollable* scroll_obj = static_cast<Scrollable*>(canvas);
            scroll_obj->setShift({canvas->getPos().x - canvas->layer_pos.x, canvas->getPos().y - canvas->layer_pos.y});
            auto scrollbar = std::make_unique<ScrollBarHor>(pos, size, scroll_obj);
            getRootWindow()->addWindow(std::move(scrollbar));
            vec2u layer_size = static_cast<Canvas*>(canvas)->layer_size;

            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kScrollBarHorWindowId));
            assert(toolbar);

            float slider_scale = canvas->getSize().x / (layer_size.x * 1.0);
            size = {static_cast<uint32_t>(slider_scale * canvas->getSize().x), toolbar->getSize().y};
            pos = {static_cast<int>(canvas->getPos().x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            auto slider = std::make_unique<ScrollBarSlider>(pos, size, 1, scroll_obj);

            size = {toolbar->getSize().y, toolbar->getSize().y};
            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x - size.x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            auto arr_up = std::make_unique<ScrollBarArrRight>(pos, size, 2, scroll_obj);

            pos = {static_cast<int>(canvas->getPos().x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            auto arr_down = std::make_unique<ScrollBarArrLeft>(pos, size, 3, scroll_obj);

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

