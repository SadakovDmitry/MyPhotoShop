#ifndef MOLECULES_GUARD
#define MOLECULES_GUARD

#define src_dir "/Users/dima/MIPT/SecondSem/MyPaint/source/"

const int MOLECULE_RADIUS  = 10;
const int MOLECULE_COUNT   = 1;
const float MOLECULE_SPEED = 150.0f;
const float EPSILON        = 0.01f;
const int WINDOW_WIDTH     = 1256;
const int WINDOW_HEIGHT    = 700;
const float MAX_IMPULSE    = 200.f;

class Molecule {
public:
    Molecule() = default;
    sf::Sprite icon;
    sf::Texture molecule_texture;
    sf::Vector2f speed;
    float weight;

    virtual void draw(sf::RenderWindow& window)    = 0;
    virtual void set_sprite(sf::Vector2f position) = 0;
    virtual Molecule* add_molecule()               = 0;
};

class Square_Molecule : public Molecule {
public:

    std::string file = src_dir "Rect.png";

    Square_Molecule() {
        weight = 1;
        sf::Vector2f position = sf::Vector2f(static_cast<float>((rand() % (WINDOW_WIDTH  - MOLECULE_RADIUS * 2)) + MOLECULE_RADIUS),
                                             static_cast<float>((rand() % (WINDOW_HEIGHT - MOLECULE_RADIUS * 2)) + MOLECULE_RADIUS));
        float angle = static_cast<float>(rand() % 360) * 3.14f / 180.0f;
        set_sprite(position);
        speed = sf::Vector2f(std::cos(angle), std::sin(angle)) * MOLECULE_SPEED;
    }

    Square_Molecule(sf::Vector2f position, sf::Vector2f speed_, int weight_) {
        set_sprite(position);
        weight = weight_;
        speed = speed_;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(icon);
    }

    void set_sprite(sf::Vector2f position) {
        molecule_texture.loadFromFile(file);
        if(!molecule_texture.loadFromFile(file)) {
            throw std::runtime_error("ошибка открытия файла > " + file + "!");
        }
        icon.setTexture(molecule_texture);
        icon.setTextureRect(sf::IntRect(0, 0, MOLECULE_RADIUS * 2, MOLECULE_RADIUS * 2));
        //icon.setScale(2.0, 2.0);
        icon.setColor(sf::Color(255, 255, 255, 255));
        icon.setPosition(position.x, position.y);
    }

    Molecule* add_molecule() {
        return (new Square_Molecule());
    }
};

class Circle_Molecule : public Molecule {
public:
    std::string file = src_dir "Red_Molecule.png";

    Circle_Molecule() {
        weight = 1;
        sf::Vector2f position = sf::Vector2f(static_cast<float>((rand() % (WINDOW_WIDTH  - MOLECULE_RADIUS * 2)) + MOLECULE_RADIUS),
                                             static_cast<float>((rand() % (WINDOW_HEIGHT - MOLECULE_RADIUS * 2)) + MOLECULE_RADIUS));
        float angle = static_cast<float>(rand() % 360) * 3.14f / 180.0f;
        set_sprite(position);
        speed = sf::Vector2f(std::cos(angle), std::sin(angle)) * MOLECULE_SPEED;
    }

    Circle_Molecule(sf::Vector2f position, sf::Vector2f speed_, int weight_) {
        set_sprite(position);
        weight = weight_;
        speed = speed_;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(icon);
    }

    void set_sprite(sf::Vector2f position) {
        molecule_texture.loadFromFile(file);
        icon.setTexture(molecule_texture);
        icon.setTextureRect(sf::IntRect(0, 0, MOLECULE_RADIUS * 2, MOLECULE_RADIUS * 2));
        icon.setColor(sf::Color(255, 255, 255, 255));
        icon.setPosition(position.x, position.y);
    }

    Molecule* add_molecule() {
        return (new Circle_Molecule());
    }
};

#endif
