
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "eraser.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {

    void EraserTool::action() {
        ICanvas* canvas = static_cast<ICanvas*>(psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();
        // mouse_pos.x -= canvas_pos.x;
        // mouse_pos.y -= canvas_pos.y;
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
