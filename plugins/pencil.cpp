
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "pencil.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {

    void ColorPaletteWindow::action() {
        Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        //ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos = canvas->getMousePosition();
        vec2u rel_pos = {static_cast<unsigned int>(mouse_pos.x + canvas->layer_pos.x - pos.x), static_cast<unsigned int>(mouse_pos.y + canvas->layer_pos.y - pos.y)};
        // std::cout << "start acton\n";
        if (this->getState() == ABarButton::State::Press) {
            // std::cout << "acton\n";
            if (this->isActive()) {
                selectedColor = image.getPixel(rel_pos);
                is_active = false;
                // std::cout << "color: " << selectedColor.r << " " << selectedColor.g << " " <<  selectedColor.b << "\n";
            }
        }
    }

    sfm::Color ColorPaletteWindow::getColor() {
        return selectedColor;
    }

    void ColorPaletteWindow::setColor(sfm::Color col) {
        selectedColor = col;
    }


    void ChoiceThicknessWindow::action() {
        Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        //ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos = canvas->getMousePosition();
        vec2i rel_pos = {mouse_pos.x + canvas->layer_pos.x - pos.x, mouse_pos.y + canvas->layer_pos.y - pos.y};
        // std::cout << "rel pos: " << rel_pos.x << " " << rel_pos.y << "\n";
        // std::cout << "rel pos: " << mouse_pos.x << " " << mouse_pos.y << "\n";
        // std::cout << "rel pos: " << pos.x << " " << pos.y << "\n\n";
        // std::cout << "start acton\n";
        if (this->getState() == ABarButton::State::Press) {
            // std::cout << "acton\n";
            if (this->isActive()) {
                thickness = static_cast<size_t>(rel_pos.x);
                // std::cout << thickness << "\n";
                // std::cout << "color: " << selectedColor.r << " " << selectedColor.g << " " <<  selectedColor.b << "\n";
            }
        }
    }

    size_t ChoiceThicknessWindow::getThickness() {
        return thickness;
    }

    void ChoiceThicknessWindow::setThickness(size_t thickness_) {
        thickness = thickness_;
    }





    void PencilTool::action() {
        Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();
        // mouse_pos.x -= canvas_pos.x;
        // mouse_pos.y -= canvas_pos.y;
        // if (canvas->isPressed()) {
        //     temp_layer->setPixel(mouse_pos, {0, 255, 0, 255});
        // }
            //std::cout << "pencil" << mouse_pos.x << ", " << mouse_pos.y << "\n";
            colorPalette->action();
            thicknessWindow->action();
            if (points_arr.size() < 4) {
                points_arr.push_back(mouse_pos);
            } else {
                points_arr.erase(points_arr.begin());
                points_arr.push_back(mouse_pos);
    	        for (double t = 0; t <= 1; t += 0.001) {
                    vec2i point = (*this)(t + 1);
                    if (canvas->isPressed()) {
                        int radius = static_cast<int>(thicknessWindow->getThickness()) / 2;
                        for (int i = radius * -1; i < radius; i++) {
                            for (int j = radius * -1; j < radius; j++) {
                                if (i * i + j * j <= radius * radius / 4) {
                                    vec2i temp_point = {point.x + i, point.y + j};
                                    temp_layer->setPixel(temp_point, colorPalette->getColor());
                                }
                            }
                        }
                        //temp_layer->setPixel(point, colorPalette->getColor());
                    }
    	        }
            }
            if (this->getState() == ABarButton::State::Press) {
                if (!colorPalette->isActive()) {
                    colorPalette->forceActivate();
                    thicknessWindow->forceActivate();
                } else {
                    colorPalette->forceDeactivate();
                    thicknessWindow->forceDeactivate();
                }
            }
    }

    sfm::Color PencilTool::getColor() {
        return color;
    }

    void PencilTool::setColor(sfm::Color col) {
        color = col;
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
            ChildInfo info_palette;
            info_palette.pos = {105, 5};
            info_palette.size = {150, 150};
            auto palette = std::make_unique<ColorPaletteWindow>(info_palette.pos, info_palette.size, 200);
            ChildInfo info_thicknessWindow;
            info_thicknessWindow.pos = {255, 5};
            info_thicknessWindow.size = {100, 50};
            auto thicknessWindow = std::make_unique<ChoiceThicknessWindow>(info_thicknessWindow.pos, info_thicknessWindow.size, 201);

            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            ChildInfo info = toolbar->getNextChildInfo();
            auto pencil = std::make_unique<PencilTool>(info.pos, info.size, 1, palette.get(), thicknessWindow.get());

            if (toolbar) {
                toolbar->addWindow(std::move(pencil));
                getRootWindow()->addWindow(std::move(palette));
                getRootWindow()->addWindow(std::move(thicknessWindow));
                // toolbar->addWindow(std::move(palette));
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
