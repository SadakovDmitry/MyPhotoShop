#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <stdexcept>

#include "Controller.h"

int main() {
    try {
        Controller controller;

        while (controller.window.isOpen()) {
            controller.update_model();
            controller.draw_model();
        }
    } catch (const std::runtime_error& error){
        std::cerr << "Произошла ошибка: " << error.what() << std::endl;
    }

    return 0;
}
