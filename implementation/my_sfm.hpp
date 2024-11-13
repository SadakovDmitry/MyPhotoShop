#ifndef SFM_PROT_HPP
#define SFM_PROT_HPP

#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_sfm.hpp"
#include <SFML/Graphics.hpp>

namespace psapi {
namespace sfm {

//class RenderWindow;
class Sprite;

class RenderWindow : public IRenderWindow {
public:
    sf::RenderWindow window;
    RenderWindow();
    RenderWindow(unsigned int width_, unsigned int height_, const std::string& name_);
    virtual ~RenderWindow() = default;

    virtual bool isOpen() const;
    virtual void clear();
    virtual void display();
    virtual void close();

    virtual vec2u getSize() const;

    virtual bool pollEvent(Event& event);

    virtual void draw(Drawable *target);

    virtual void setFps(float fps);
    virtual float getFps() const;

    static std::unique_ptr<IRenderWindow> create(unsigned int width, unsigned int height, const std::string& name);
};

class Texture : public ITexture {
public:
    sf::Texture texture;

    virtual ~Texture() = default;

    virtual bool create(unsigned int width, unsigned int height);
    virtual bool loadFromFile  (const std::string& filename,        const IntRect& area = IntRect());
    virtual bool loadFromMemory(const void* data, std::size_t size, const IntRect& area = IntRect());
    virtual bool loadFromImage (const IImage *image,                const IntRect& area = IntRect());
    virtual vec2u getSize() const;
    virtual std::unique_ptr<IImage> copyToImage() const;
    virtual void update(const IImage *image);
    virtual void update(const Color *pixels);
    virtual void update(const Color *pixels, unsigned int width, unsigned int height,
                                             unsigned int x,     unsigned int y);
    ITexture& operator=(const ITexture& right);
};

class Sprite : public ISprite {
private:
    sf::Sprite sprite;

public:
    virtual ~Sprite() = default;

    virtual void setTexture(const ITexture *texture, bool reset_rect = false);
    virtual void setTextureRect(const IntRect &rectangle);

    virtual void setPosition(float x, float y);
    virtual void setPosition(const vec2f &pos);

    virtual void setScale(float factorX, float factorY);
    virtual vec2u getSize() const;

    virtual void setColor(const Color &color);

    virtual void setRotation(float angle);

    virtual const vec2f getPosition() const;
    virtual IntRect getGlobalBounds() const;

    virtual void draw(IRenderWindow *window) const;

    static std::unique_ptr<ISprite> create();
};

class Font : public IFont {
private:
    sf::Font font;

public:
    virtual ~Font() = default;

    virtual bool loadFromFile(const std::string& filename);

    const sf::Font& getFont() const;

    static IFont* create();
};

class Text : public IText {
private:
    sf::Text text;

public:
    enum Style
    {
        Regular       = 0,
        Bold          = 1 << 0,
        Italic        = 1 << 1,
        Underlined    = 1 << 2,
        StrikeThrough = 1 << 3
    };

    virtual ~Text() = default;

    virtual void draw(IRenderWindow *window);

    virtual void setString(const std::string& string);

    virtual void setFont(const IFont* font);

    virtual void setCharacterSize(unsigned int size);

    virtual void setStyle(uint32_t style);

    virtual void setFillColor(const Color* color);

    virtual void setOutlineColor(const Color* color);

    virtual void setOutlineThickness(float thickness);
};

};
};


#endif
