// #include "api_photoshop.hpp"
// #include "api_bar.hpp"
#include <dlfcn.h>
#include "canvas.hpp"
#include "toolbar.hpp"
// #include "sfm_prot.hpp"
#include "plugin_example.hpp"

typedef bool (*LoadPluginFunc)();
typedef void (*UnloadPluginFunc)();

using namespace psapi;
using namespace sfm;

void loadPluginfromLib(const std::string& plugin) {
    void* pluginLib = dlopen(("/Users/dima/MIPT/SecondSem/MyPaint/plugins/" + plugin).c_str(), RTLD_LAZY);
    if (!pluginLib) {
        std::cerr << "Failed to load plugin dylib in main: " << dlerror() << "\n";
        assert("loadPluginfromLib" && 0);
    }

    LoadPluginFunc loadPlugin = reinterpret_cast<LoadPluginFunc>(dlsym(pluginLib, "loadPlugin"));
    UnloadPluginFunc unloadPlugin = reinterpret_cast<UnloadPluginFunc>(dlsym(pluginLib, "unloadPlugin"));

    if (!loadPlugin || !unloadPlugin) {
        std::cerr << "Failed to locate functions in plugin dylib in main: " << dlerror() << "\n";
        dlclose(pluginLib);
        assert("loadPluginfromLib" && 0);
    }

    if (!loadPlugin()) {
        std::cerr << "Failed to initialize plugin in main.\n";
        dlclose(pluginLib);
        assert("loadPluginfromLib" && 0);
    }

    std::cout << "Plugin loaded successfully!\n";
}

int main() {
    loadBasePlugin();
    loadPluginfromLib("pencil.dylib");
    loadPluginfromLib("eraser.dylib");
    loadPluginfromLib("sprayer.dylib");
    loadPluginfromLib("scrollbar_vert.dylib");
    loadPluginfromLib("scrollbar_gor.dylib");

    std::cout << "Hello!!!\n";
    const float file_height = 1564.0;
    const float file_width = 2332.0;
    RenderWindow RW;
    while (RW.isOpen()) {
        Event event;
        while (RW.pollEvent(event)) {
            if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) { RW.close(); }
            if (event.type == Event::Closed) { RW.close(); }
            getRootWindow()->update(&RW, event);
        }

        getRootWindow()->draw(&RW);
        RW.display();
    }
    return 0;
}
