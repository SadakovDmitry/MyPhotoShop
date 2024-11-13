
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "eraser.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {
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
        //std::cerr << "Photoshop loaded successfully action!\n";
        //std::cerr << "action: " << getRootWindowlib() << "\n";
        return getRootWindowlib();
    }

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


    void EraserTool::action() {
        ICanvas* canvas = static_cast<ICanvas*>(psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();
        mouse_pos.x -= canvas_pos.x;
        mouse_pos.y -= canvas_pos.y;
        vec2i cur_pos;
        if (canvas->isPressed()) {
            for (int i = -2; i < 3; i++) {
                for (int j = -2; j < 3; j++) {
                    cur_pos.x = mouse_pos.x + i;
                    cur_pos.y = mouse_pos.y + j;
                    temp_layer->setPixel(cur_pos, {0, 0, 0, 0});
                }
            }
        }
    }


    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {
            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            ChildInfo info = toolbar->getNextChildInfo();
            auto eraser = std::make_unique<EraserTool>(info.pos, info.size, 2);

            if (toolbar) {
                toolbar->addWindow(std::move(eraser));
                std::cout << "add Window Succsesfull\n";
                return true;
            }
            std::cout << "not add Window Succsesfull\n";
            assert(toolbar);
            return false;
        }

        __attribute__((visibility("default"))) void unloadPlugin() {
            IBar* toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            if (toolbar) {
                toolbar->removeWindow(2);
            }
        }
    }
}
