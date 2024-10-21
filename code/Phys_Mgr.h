#ifndef PHYS_MGR_GUARD
#define PHYS_MGR_GUARD

#include "Chem_Mgr.h"

class Phys_Mgr {
public:
    std::vector<Molecule*>& molecules;
    Chem_Mgr chem_mgr;

    Phys_Mgr(std::vector<Molecule*>& molecules_) : molecules(molecules_) {};

    void Collision_with_wall(Molecule& molecule) {
        if (molecule.icon.getPosition().x <= 0 ||
            molecule.icon.getPosition().x + MOLECULE_RADIUS * 2 >= WINDOW_WIDTH) {
            molecule.speed.x = -molecule.speed.x;
        }
        if (molecule.icon.getPosition().y <= 0 ||
            molecule.icon.getPosition().y + MOLECULE_RADIUS * 2 >= WINDOW_HEIGHT) {
            molecule.speed.y = -molecule.speed.y;
        }
    }

    float dist(const Molecule& a, const Molecule& b) {
        float dx = a.icon.getPosition().x - b.icon.getPosition().x;
        float dy = a.icon.getPosition().y - b.icon.getPosition().y;
        return std::sqrt(dx * dx + dy * dy);
    }

    void Collision_molecules(int index_a, int index_b) {
        Molecule& a = *molecules[index_a];
        Molecule& b = *molecules[index_b];

        if (dist(a, b) <= MOLECULE_RADIUS * 2) {
            if(a.weight * std::sqrt(a.speed.x * a.speed.x + a.speed.y + a.speed.y) +
               b.weight * std::sqrt(b.speed.x * b.speed.x + b.speed.y + b.speed.y) > MAX_IMPULSE) {
                chem_mgr.check_reaction(index_a, index_b, molecules);
            } else {
                sf::Vector2f delta = a.icon.getPosition() - b.icon.getPosition();
                float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y) + EPSILON;
                sf::Vector2f normal = delta / dist;

                float a_proj = (a.speed.x * normal.x + a.speed.y * normal.y);
                float b_proj = (b.speed.x * normal.x + b.speed.y * normal.y);

                a.speed -= normal * (a_proj - b_proj);
                b.speed -= normal * (b_proj - a_proj);
            }
        }
    }

    void create_molecules(std::vector<Molecule*> &molecules) {
        for (int i = 0; i < MOLECULE_COUNT; i++) {
            Circle_Molecule* molecule = new Circle_Molecule();
            molecules.push_back(molecule);
        }
        for (int i = 0; i < MOLECULE_COUNT; i++) {
            Square_Molecule* molecule = new Square_Molecule();
            molecules.push_back(molecule);
        }
    }

    void check_collisions(std::vector<Molecule*> &molecules, float delta_time) {
        for (auto& molecule : molecules) {
            molecule -> icon.move(molecule -> speed * delta_time);
            Collision_with_wall(*molecule);
        }

        for (size_t i = 0; i < molecules.size(); ++i) {
            for (size_t j = i + 1; j < molecules.size(); ++j) {
                Collision_molecules(i, j);
            }
        }
    }

    // void draw_all(sf::RenderWindow& window, std::vector<Molecule*> molecules, Buttons_Manager buttons){
    //     window.clear(sf::Color::Black);
    //     for (const auto& molecule : molecules) {
    //         molecule -> draw(window);
    //     }
    //     buttons.show_buttons(window);
    //     window.display();
    // }

    ~Phys_Mgr() {
        // for (Molecule* molecule : molecules) {
        //     delete molecule;
        // }
    }
};

#endif
