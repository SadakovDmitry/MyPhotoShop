
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "pencil.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {
    typedef IWindowContainer* (*RootWindow)();

    IWindowContainer* getRootWindowptr() {
        void* photoshopLib = dlopen("/Users/dima/MIPT/SecondSem/MyPaint/Standard/libimplementation.dylib", RTLD_LAZY);
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


    void PencilTool::action() {
        ICanvas* canvas = static_cast<ICanvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();
        // mouse_pos.x -= canvas_pos.x;
        // mouse_pos.y -= canvas_pos.y;
        // if (canvas->isPressed()) {
        //     temp_layer->setPixel(mouse_pos, {0, 255, 0, 255});
        // }
            //std::cout << "pencil" << mouse_pos.x << ", " << mouse_pos.y << "\n";

            if (points_arr.size() < 4) {
                points_arr.push_back(mouse_pos);
            } else {
                points_arr.erase(points_arr.begin());
                points_arr.push_back(mouse_pos);
    	        for (double t = 0; t <= 1; t += 0.001) {
                    vec2i point = (*this)(t + 1);
                    if (canvas->isPressed()) {
                        temp_layer->setPixel(point, {0, 255, 0, 255});
                    }
    	        }
            }
    }

    double PencilTool::CatmullRom(double p0, double p1, double p2, double p3, double t){
        double t2   = t * t;
        double t3   = t2 * t;
        double t3_2 = t3 * 2;
        double t2_3 = t2 * 3;

        double hermite00 =  t3_2 - t2_3 + 1;
        double hermite10 =  t3   - t2 * 2 + t;
        double hermite01 = -t3_2 + t2_3;
        double hermite11 =  t3   - t2;

        double m0 = (p2 - p0) / 2;
        double m1 = (p3 - p1) / 2;

        return std::max(hermite00 * p1 + hermite10 * m0 * 1.0 +
               hermite01 * p2 + hermite11 * m1 * 1.0, 0.0);
    }

    vec2i PencilTool::operator() (double t){
        std::vector<vec2i>::iterator iter = points_arr.begin();
        int i = t / 1;
        iter += i;
        t -= i;
        vec2i temp_point {0, 0};

        temp_point.x = CatmullRom((*(iter - 1)).x, (*iter).x, (*(iter + 1)).x, (*(iter + 2)).x, t);
        temp_point.y = CatmullRom((*(iter - 1)).y, (*iter).y, (*(iter + 1)).y, (*(iter + 2)).y, t);
        return temp_point;
    }


    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {
            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            ChildInfo info = toolbar->getNextChildInfo();
            auto pencil = std::make_unique<PencilTool>(info.pos, info.size, 1);

            if (toolbar) {
                toolbar->addWindow(std::move(pencil));
                return true;
            }
            assert(toolbar);
            return false;
        }

        __attribute__((visibility("default"))) void unloadPlugin() {
            IBar* toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            if (toolbar) {
                toolbar->removeWindow(1);
            }
        }
    }
}
