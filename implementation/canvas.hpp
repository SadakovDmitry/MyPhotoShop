#ifndef CANVAS_HPP
#define CANVAS_HPP

// #include "api_photoshop.hpp"
// #include "api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_canvas.hpp"
#include "toolbar.hpp"
#include "my_sfm.hpp"

#include <cstdint>
#include <memory>
#include <iostream>

namespace psapi {

class Scrollable {
protected:
    bool is_dragging;
    vec2i scroll_shift;
public:
    Scrollable() = default;
    Scrollable(vec2i scroll_shift_) : scroll_shift(scroll_shift_) {}

    virtual bool scroll(const vec2i& vec) = 0;
    virtual void setShift(vec2i shift_) = 0;
};

class Layer : public ILayer {
private:
    friend class Canvas;
    std::vector<sfm::Color> pix_arr;
    vec2i pos;
    vec2u size;
    bool is_active;

public:
    Layer(vec2u size_);
    Layer(vec2i pos_, vec2u size_);
    Layer();
    virtual sfm::Color getPixel(sfm::vec2i pos) const;
    virtual void       setPixel(sfm::vec2i pos, sfm::Color pixel);
};

class Canvas : public ICanvas, public Scrollable{
private:
    std::vector<std::unique_ptr<Layer>> layers;
    std::unique_ptr<Layer> temp_layer;
    int id;
    vec2i pos;
    vec2u size;
    vec2f scale;
    psapi::sfm::Texture texture;
    psapi::sfm::Sprite sprite;
    vec2i mouse_pos;
    bool is_pressed;
    bool is_active  = true;
    const IWindow* parent;
    //Tool* active_tool;
    //std::unique_ptr<BarButton> activeTool;
    BarButton* activeTool;

    vec2i CalcMouseShift();
public:
    vec2u layer_size;
    vec2i layer_pos;

    Canvas(vec2i pos_, vec2u size_, vec2u layer_size_);

    virtual ~Canvas() = default;
    /**
     * @brief Renders the window.
     * @param renderWindow The render target.
     */
    virtual void draw(IRenderWindow* renderWindow);

    /**
     * @brief Updates the window based on events.
     * @param renderWindow The render target.
     * @param event The event to process.
     * @return True if the window was updated, false otherwise.
     */
    virtual bool update(const IRenderWindow* renderWindow, const Event& event);

    /**
     * @brief Gets the unique ID of the window.
     * @return The window ID, or kInvalidWindowId if not set.
     */
    virtual wid_t getId() const;

    /**
     * @brief Retrieves a window by its ID from this window or its children.
     * @param id The ID of the window to search for.
     * @return A pointer to the window if found, or nullptr if not found.
     */
    virtual IWindow* getWindowById(wid_t id);

    /**
     * @brief Retrieves a window by its ID from this window or its children (const version).
     * @param id The ID of the window to search for.
     * @return A const pointer to the window if found, or nullptr if not found.
     */
    virtual const IWindow* getWindowById(wid_t id) const;

    /**
     * @brief Gets the position of the window.
     * @return The absolute window position as a vec2i.
     */
    virtual vec2i getPos() const;

    /**
     * @brief Gets the size of the window.
     * @return The window size as a vec2i.
     */
    virtual vec2u getSize() const;

    /**
     * @brief Sets the parent of this window.
     * @param parent A pointer to the parent window.
     */
    virtual void setParent(const IWindow* parent);

    /**
     * @brief Forces the window to activate.
     */
    virtual void forceActivate();

    /**
     * @brief Forces the window to deactivate.
     */
    virtual void forceDeactivate();

    virtual bool isActive() const;

    /**
     * @brief Checks if the window is a window container.
     * @return True if the window is a window container, false otherwise.
     */
    virtual bool isWindowContainer() const;

    virtual ILayer*       getLayer(size_t index);
    virtual const ILayer* getLayer(size_t index) const;

    virtual ILayer*       getTempLayer();
    virtual const ILayer* getTempLayer() const;

    virtual void cleanTempLayer();

    virtual size_t getNumLayers() const;

    virtual size_t getActiveLayerIndex() const;
    virtual void   setActiveLayerIndex(size_t index);

    virtual bool insertLayer     (size_t index, std::unique_ptr<ILayer> layer);
    virtual bool removeLayer     (size_t index);
    virtual bool insertEmptyLayer(size_t index);

    virtual void setPos  (sfm::vec2i pos);
    virtual void setSize (sfm::vec2u size);
    virtual void setScale(sfm::vec2f scale);

    virtual sfm::vec2i getMousePosition() const;
    virtual bool isPressed() const;

    virtual void setActiveTool(BarButton* tool) {
        activeTool = tool;
    }
    virtual BarButton* getActiveTool() {
        return activeTool;
    }

    virtual bool scroll(const vec2i& vec);
    virtual void setShift(vec2i shift_);
};

} // namespace

#endif // API_CANVAS_HPP

