
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "sprayer.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {

    void SprayerTool::action() {
        srand(time(0));
        ICanvas* canvas = static_cast<ICanvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();
        // mouse_pos.x -= canvas_pos.x;
        // mouse_pos.y -= canvas_pos.y;
        vec2i cur_pos;

        if (canvas->isPressed()) {
            for (int i = 0; i < 30; i++) {
                cur_pos.x = mouse_pos.x + (rand() % 20);
                cur_pos.y = mouse_pos.y + (rand() % 20);
                temp_layer->setPixel(cur_pos, {0, 0, 0, 255});
            }
        }
    }


    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {

            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            ChildInfo info = toolbar->getNextChildInfo();
            auto tool = std::make_unique<SprayerTool>(info.pos, info.size, 3);

            if (toolbar) {
                toolbar->addWindow(std::move(tool));
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
                toolbar->removeWindow(1);
            }
        }
    }
}
