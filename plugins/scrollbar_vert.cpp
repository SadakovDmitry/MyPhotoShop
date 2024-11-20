// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "scrollbar_vert.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {

    const int kScrollBarWindowId  = kCanvasWindowId - 1;
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
            drag_offset.y = mouse_pos.y + canvas->layer_pos.y - pos.y;
            scroll_obj->setShift({canvas_pos.x - canvas->layer_pos.x, canvas_pos.y - canvas->layer_pos.y});
        }
        else if (is_dragging) {

            int old_slider_pos_y = pos.y;
            int new_slider_pos_y = mouse_pos.y + canvas->layer_pos.y - drag_offset.y;

            new_slider_pos_y = std::min(static_cast<int>(canvas_pos.y + canvas_size.y - size.y),
                    std::max(new_slider_pos_y, canvas_pos.y));
            int slider_shift_y = (new_slider_pos_y - old_slider_pos_y);
            int layer_shift_y = slider_shift_y * (canvas->layer_size.y / canvas_size.y);
            pos.y = new_slider_pos_y;
            sprite.setPosition(pos.x, pos.y);
            scroll_obj->scroll({0, layer_shift_y});
        }
    }

    void ScrollBarArrUp::action() {
        ScrollBarVert* scrollbar = static_cast<ScrollBarVert*>(getRootWindow()->getWindowById(kScrollBarWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (this->state == ABarButton::State::Press) {
            slider->pos.y -= 5;
            scroll_obj->scroll({0, -5});
            slider->sprite.setPosition(slider->pos.x, slider->pos.y);
        }
    }

    void ScrollBarArrDown::action() {
        ScrollBarVert* scrollbar = static_cast<ScrollBarVert*>(getRootWindow()->getWindowById(kScrollBarWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (this->state == ABarButton::State::Press) {
            slider->pos.y += 5;
            scroll_obj->scroll({0, 5});
            slider->sprite.setPosition(slider->pos.x, slider->pos.y);
        }
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      Load and Unload
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {

            auto canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(psapi::kCanvasWindowId));
            vec2u size = {20, canvas->getSize().y};
            vec2i pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x),
                         static_cast<int>(canvas->getPos().y)};
            Scrollable* scroll_obj = static_cast<Scrollable*>(canvas);
            scroll_obj->setShift({canvas->getPos().x - canvas->layer_pos.x, canvas->getPos().y - canvas->layer_pos.y});
            auto scrollbar = std::make_unique<ScrollBarVert>(pos, size, scroll_obj);
            getRootWindow()->addWindow(std::move(scrollbar));
            vec2u layer_size = static_cast<Canvas*>(canvas)->layer_size;

            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kScrollBarWindowId));
            assert(toolbar);

            float slider_scale = canvas->getSize().y / (layer_size.y * 1.0);
            size = {toolbar->getSize().x, static_cast<uint32_t>(slider_scale * canvas->getSize().y)};
            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x),
                   static_cast<int>(canvas->getPos().y)};
            auto slider = std::make_unique<ScrollBarSlider>(pos, size, 1, scroll_obj);

            size = {toolbar->getSize().x, toolbar->getSize().x};
            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x),
                   static_cast<int>(canvas->getPos().y)};
            auto arr_up = std::make_unique<ScrollBarArrUp>(pos, size, 2, scroll_obj);

            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y - size.y)};
            auto arr_down = std::make_unique<ScrollBarArrDown>(pos, size, 3, scroll_obj);

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

