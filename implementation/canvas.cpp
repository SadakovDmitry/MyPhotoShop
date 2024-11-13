#include "canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_sfm.hpp"

namespace psapi {

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         Layer
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    Layer::Layer(vec2u size_) : size(size_) {
        pix_arr = std::vector<sfm::Color>(size_.x * size_.y, sfm::Color(255, 255, 255, 255));
    }
    Layer::Layer(vec2i pos_, vec2u size_) : size(size_), pos(pos_){
        pix_arr = std::vector<sfm::Color>(size_.x * size_.y, sfm::Color(255, 255, 255, 255));
    }
    Layer::Layer() = default;

    sfm::Color Layer::getPixel(sfm::vec2i pix_pos) const {
        if (pix_pos.x < size.x && pix_pos.y < size.y &&
            pix_pos.x > 0      && pix_pos.y > 0        ) {
            return pix_arr[pos.x + pix_pos.y * size.x];
        }
        return sfm::Color(0, 0, 0, 0);
    }
    void       Layer::setPixel(sfm::vec2i pos, sfm::Color pixel) {
        if (pos.x < size.x && pos.y < size.y &&
            pos.x > 0      && pos.y > 0        ) {
            pix_arr[pos.x + pos.y * size.x] = pixel;
        }
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         Canvas
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    Canvas::Canvas(vec2i pos_, vec2u size_, vec2u layer_size_)
            : temp_layer(std::make_unique<Layer>(layer_size_)), layers(), pos(pos_), layer_size(layer_size_), layer_pos(pos_), size(size_), texture(), sprite(), mouse_pos(vec2i(0, 0)), is_pressed(false), id(kCanvasWindowId), activeTool(nullptr) {
        texture.create(static_cast<unsigned int>(layer_size.x), static_cast<unsigned int>(layer_size.y));
        std::vector<sfm::Color> pix_arr(layer_size.x * layer_size.y, sfm::Color(255, 255, 255, 255));
        texture.loadFromMemory(pix_arr.data(), layer_size.x * layer_size.y, sfm::IntRect(0, 0, size.x, size.y));
        // if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/Eraser.png")) {
        //     std::cerr << "Error opening file\n";
        // }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
        layers.push_back(std::make_unique<Layer>(layer_size));
    }

    /**
     * @brief Renders the window.
     * @param renderWindow The render target.
     */
    void Canvas::draw(IRenderWindow* renderWindow) {
        if(activeTool) {
            activeTool->action();
        }

        for (auto& layer : layers) {
            assert(layer);
            texture.update(layer->pix_arr.data());
            sprite.setTexture(&texture);
            renderWindow->draw(&sprite);
        }

        texture.update(temp_layer->pix_arr.data());
        sprite.setTexture(&texture);
        renderWindow->draw(&sprite);
    }

    vec2i Canvas::CalcMouseShift() {
        int slider_pos_y = getRootWindow()->getWindowById(kCanvasWindowId - 1)->getWindowById(1)->getPos().y;
        int slider_pos_x = getRootWindow()->getWindowById(kToolBarWindowId + 11)->getWindowById(1)->getPos().x;
        float shift_coef_y  = static_cast<float>(slider_pos_y - pos.y) / size.y;
        float shift_coef_x  = static_cast<float>(slider_pos_x - pos.x) / size.x;
        int shift_y = shift_coef_y * (layer_size.y - size.y);
        int shift_x = (shift_coef_x * (layer_size.x - size.x)) * (layer_size.x / size.x);
        sprite.setTextureRect(sfm::IntRect(shift_x, shift_y, size.x, size.y));
        return vec2i(shift_x, shift_y);
    }

    bool Canvas::update(const IRenderWindow* renderWindow, const Event& event) {
        vec2i shift = CalcMouseShift();
        if (event.type == Event::MouseMoved) {
            mouse_pos = sfm::vec2i(event.mouseMove.x  + shift.x, event.mouseMove.y  + shift.y);

            if (mouse_pos.x >= layer_pos.x && mouse_pos.x <= layer_pos.x + layer_size.x &&
                mouse_pos.y >= layer_pos.y && mouse_pos.y <= layer_pos.y + layer_size.y) {
                return true;
            }
            return true;
        }
        if (event.type == Event::MouseButtonPressed) {
            mouse_pos = sfm::vec2i(event.mouseButton.x  + shift.x, event.mouseButton.y  + shift.y);

            if (mouse_pos.x >= layer_pos.x && mouse_pos.x <= layer_pos.x + layer_size.x &&
                mouse_pos.y >= layer_pos.y && mouse_pos.y <= layer_pos.y + layer_size.y) {
                is_pressed = true;
                return true;
            }
            is_pressed = false;
            return true;
        }
        if (event.type == Event::MouseButtonReleased) {
            mouse_pos = sfm::vec2i(event.mouseButton.x  + shift.x, event.mouseButton.y  + shift.y);

            if (mouse_pos.x >= layer_pos.x && mouse_pos.x <= layer_pos.x + layer_size.x &&
                mouse_pos.y >= layer_pos.y && mouse_pos.y <= layer_pos.y + layer_size.y) {
                is_pressed = false;
                return true;
            }
            is_pressed = false;
            return true;
        }

        return false;
    }

    /**
     * @brief Gets the unique ID of the window.
     * @return The window ID, or kInvalidWindowId if not set.
     */
    wid_t Canvas::getId() const {
        return id;
    }

    /**
     * @brief Retrieves a window by its ID from this window or its children.
     * @param id The ID of the window to search for.
     * @return A pointer to the window if found, or nullptr if not found.
     */
    IWindow* Canvas::getWindowById(wid_t searchId) {
        if (id == searchId) {
            return this;
        }
        return nullptr;
    }

    /**
     * @brief Retrieves a window by its ID from this window or its children (const version).
     * @param id The ID of the window to search for.
     * @return A const pointer to the window if found, or nullptr if not found.
     */
    const IWindow* Canvas::getWindowById(wid_t searchId) const {
        if (id == searchId) {
            return this;
        }
        return nullptr;
    }

    /**
     * @brief Gets the position of the window.
     * @return The absolute window position as a vec2i.
     */
    vec2i Canvas::getPos() const {
        return vec2i(pos.x, pos.y);
    }

    /**
     * @brief Gets the size of the window.
     * @return The window size as a vec2i.
     */
    vec2u Canvas::getSize() const {
        return vec2u(size.x, size.y);
    }

    /**
     * @brief Sets the parent of this window.
     * @param parent A pointer to the parent window.
     */
    void Canvas::setParent(const IWindow* parent) {
        this -> parent = parent;
    }

    /**
     * @brief Forces the window to activate.
     */
    void Canvas::forceActivate() {
        is_active = true;
    }

    /**
     * @brief Forces the window to deactivate.
     */
    void Canvas::forceDeactivate() {
        is_active = false;
    }

    bool Canvas::isActive() const { return is_active == true; }

    /**
     * @brief Checks if the window is a window container.
     * @return True if the window is a window container, false otherwise.
     */
    bool Canvas::isWindowContainer() const {
        return false;
    }

          ILayer* Canvas::getLayer(size_t index) {
            return layers.at(index).get();
    }
    const ILayer* Canvas::getLayer(size_t index) const {
            return layers.at(index).get();
    }

          ILayer* Canvas::getTempLayer() {
            return temp_layer.get();
    }
    const ILayer* Canvas::getTempLayer() const {
        return temp_layer.get();
    }

    void Canvas::cleanTempLayer() {
        if (layers.size()) {
            vec2u size = temp_layer.get()->size;
            temp_layer = std::make_unique<Layer>(size);
            std::cout << "bed realised clean\n";
        }
    }

    size_t Canvas::getNumLayers() const {
        return layers.size();
    }

    size_t Canvas::getActiveLayerIndex() const {
        for (int i = 0; i < layers.size(); i++) {
            if (layers.at(i).get()->is_active) {
                return i;
            }
        }
        return 0;
    }
    void   Canvas::setActiveLayerIndex(size_t index) {
        if(index < layers.size()) {
            for (int i = 0; i < layers.size(); i++) {
                if (layers.at(i).get()->is_active) {
                    layers.at(i).get()->is_active = false;
                }
            }
            layers.at(index).get()->is_active = true;
        }
    }

    bool Canvas::insertLayer     (size_t index, std::unique_ptr<ILayer> layer) {
        if (index <= layers.size()) {
            std::unique_ptr<Layer> new_layer = std::make_unique<Layer>(size);
            for (int x = 0; x < size.x; x++) {
                for (int y = 0; y < size.y; y++) {
                    new_layer->setPixel(vec2i(x, y), layer->getPixel(vec2i(x, y)));
                }
            }
            layers.insert(layers.begin() + index, std::move(new_layer));
            return true;
        }
        return false;
    }
    bool Canvas::removeLayer     (size_t index) {
        if (index < layers.size()) {
            layers.erase(layers.begin() + index);
            return true;
        }
        return false;
    }
    bool Canvas::insertEmptyLayer(size_t index) {
        if (index <= layers.size()) {
            std::unique_ptr<Layer> new_layer = std::make_unique<Layer>(size);
            for (int x = 0; x < size.x; x++) {
                for (int y = 0; y < size.y; y++) {
                    new_layer->setPixel(vec2i(x, y), sfm::Color(0, 0, 0, 0));
                }
            }
            layers.insert(layers.begin() + index, std::move(new_layer));
            return true;
        }
        return false;
    }

    void Canvas::setPos  (sfm::vec2i pos) {
        this->pos = pos;
    }
    void Canvas::setSize (sfm::vec2u size) {
        this->size = size;
    }
    void Canvas::setScale(sfm::vec2f scale) {
        this->scale = scale;
    }

    sfm::vec2i Canvas::getMousePosition() const {
        return mouse_pos;
    }
    bool Canvas::isPressed() const {
        return is_pressed;
    }

    bool Canvas::scroll(const vec2i& vec) {
        std::cout << "scroll\n";
        vec2f sprite_pos = sprite.getPosition();
        sprite.setTextureRect(sfm::IntRect(sprite_pos.x + vec.x, sprite_pos.y + vec.y, size.x, size.y));
        return true;
    }

    void Canvas::setShift(vec2i shift_) {
        scroll_shift.x = shift_.x;
        scroll_shift.y = shift_.y;
    }
}
