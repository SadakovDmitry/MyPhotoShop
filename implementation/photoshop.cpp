
#include "photoshop.hpp"

using namespace psapi;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                       IWindowContainer
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
extern "C" {

__attribute__((visibility("default"))) IWindowContainer* psapi::getRootWindow() {
    static RootWindow rootWindow;
    return &rootWindow;
}

}

bool IWindowContainer::isWindowContainer() const {
    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          IWindowVector
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

void IWindowVector::addWindow(std::unique_ptr<IWindow> window) {
    windows_.push_back(std::move(window));
}
void IWindowVector::removeWindow(wid_t id) {
    windows_.erase(std::remove_if(windows_.begin(), windows_.end(),
        [id](const std::unique_ptr<IWindow>& win) { return win->getId() == id; }),
        windows_.end());
}

IWindow* IWindowVector::getWindowById(wid_t id) {
    for (const auto& window : windows_) {
        if (window->getId() == id) {
            return window.get();
        }
    }
    return nullptr;
}

const IWindow* IWindowVector::getWindowById(wid_t id) const {
    for (const auto& window : windows_) {
        if (window->getId() == id) {
            return window.get();
        }
    }
    return nullptr;
}

bool IWindowVector::isWindowContainer() const {
    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          RootWindow
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

RootWindow::RootWindow() = default;

void RootWindow::draw(IRenderWindow* renderWindow) {
    for (const auto& window : windows_) {
        window->draw(renderWindow);
    }
}

bool RootWindow::update(const IRenderWindow* renderWindow, const sfm::Event& event) {
    for (const auto& window : windows_) {
        if (window->update(renderWindow, event)) {
            //return true;
        }
    }
    return false;
}

int64_t RootWindow::getId() const {
    return kRootWindowId;
}

psapi::vec2i RootWindow::getPos() const {
    return {0, 0};
}

psapi::vec2u RootWindow::getSize() const {
    return {800, 600};
}

void RootWindow::setParent(const IWindow* parent) {
    std::cout << "no no no parent in root window\n";
    abort();
}

void RootWindow::forceActivate() {
    std::cout << "no no no root window is always active";
    // is_active_ = true;
}

void RootWindow::forceDeactivate() {
    // is_active_ = false;
    std::cout << "no no no root window is always active";
}

bool RootWindow::isActive() const { return is_active_ == true; }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          AWindow
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

wid_t AWindow::getId() const {return id;}

vec2i AWindow::getPos() const { return pos; }
vec2u AWindow::getSize() const { return size; }
void AWindow::setParent(const IWindow* parent) { this->parent = parent; }
void AWindow::forceActivate() { is_active = true; }
void AWindow::forceDeactivate() { is_active = false; }
bool AWindow::isActive() const { return is_active == true; }
bool AWindow::isWindowContainer() const { return true; }

