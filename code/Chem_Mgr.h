#ifndef CHEM_MGR_GUARD
#define CHEM_MGR_GUARD

#include <typeinfo>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Molecules.h"

const std::type_info& CIRCLE_MOLECULE = typeid(Circle_Molecule);
const std::type_info& SQUARE_MOLECULE = typeid(Square_Molecule);
const int NUM_MOL_T = 2;

class Chem_Mgr {
public:
    std::vector<const std::type_info*> molecule_types;
    std::vector<std::vector<void (Chem_Mgr::*)(int index_a, int index_b, std::vector<Molecule*>& molecules)>> chem_mgr;

    Chem_Mgr() {
        set_molecule_types_vector();
        chem_mgr.resize(NUM_MOL_T, std::vector<void (Chem_Mgr::*)(int, int, std::vector<Molecule*>&)>(NUM_MOL_T, nullptr));
        set_chem_mgr();
    }

    void Circle_and_Circle(int index_a, int index_b, std::vector<Molecule*>& molecules) {
        Molecule* molecule_a = molecules[index_a];
        Molecule* molecule_b = molecules[index_b];

        float sum_weight = molecule_a->weight + molecule_b->weight;
        sf::Vector2f new_speed = (molecule_a->speed * molecule_a->weight
                                + molecule_b->speed * molecule_b->weight) / sum_weight;
        sf::Vector2f new_position = (molecule_a->icon.getPosition() + molecule_b->icon.getPosition()) / 2.f;

        Square_Molecule* new_molecule = new Square_Molecule(new_position, new_speed, sum_weight);
        molecules.erase(molecules.begin() + std::max(index_a, index_b));
        molecules.erase(molecules.begin() + std::min(index_a, index_b));
        molecules.push_back(new_molecule);
    }

    void Circle_and_Square(int index_a, int index_b, std::vector<Molecule*>& molecules) {
        Molecule* molecule_a = molecules[index_a];
        Molecule* molecule_b = molecules[index_b];

        float sum_weight = molecule_a->weight + molecule_b->weight;
        sf::Vector2f new_speed = (molecule_a->speed * molecule_a->weight
                              + molecule_b->speed * molecule_b->weight) / sum_weight;
        sf::Vector2f new_position = (molecule_a->icon.getPosition() + molecule_b->icon.getPosition()) / 2.f;

        Square_Molecule* new_molecule = new Square_Molecule(new_position, new_speed, sum_weight);
        molecules.erase(molecules.begin() + std::max(index_a, index_b));
        molecules.erase(molecules.begin() + std::min(index_a, index_b));
        molecules.push_back(new_molecule);
    }

    void Square_and_Square(int index_a, int index_b, std::vector<Molecule*>& molecules) {
        Molecule* molecule_a = molecules[index_a];
        Molecule* molecule_b = molecules[index_b];

        float sum_weight = molecule_a->weight + molecule_b->weight;
        sf::Vector2f avg_speed = (molecule_a->speed + molecule_b->speed) / 2.0f;

        float new_weight = sum_weight / 6.0f;
        molecules.erase(molecules.begin() + std::max(index_a, index_b));
        molecules.erase(molecules.begin() + std::min(index_a, index_b));

        for (int i = 0; i < 6; ++i) {
            float angle = (i * 60.0f) * (M_PI / 180.0f);
            sf::Vector2f new_speed = avg_speed + sf::Vector2f(std::cos(angle), std::sin(angle)) * 200.0f;

            Circle_Molecule* new_molecule = new Circle_Molecule((molecule_a->icon.getPosition() + molecule_b->icon.getPosition()) / 2.0f
                                                                 + new_speed / 5.f, new_speed, new_weight);
            molecules.push_back(new_molecule);
        }
    }

    void set_chem_mgr() {
        chem_mgr[0][0] = &Chem_Mgr::Circle_and_Circle;
        chem_mgr[0][1] = &Chem_Mgr::Circle_and_Square;
        chem_mgr[1][0] = &Chem_Mgr::Circle_and_Square;
        chem_mgr[1][1] = &Chem_Mgr::Square_and_Square;
    }

    void set_molecule_types_vector() {
        molecule_types.push_back(&CIRCLE_MOLECULE);
        molecule_types.push_back(&SQUARE_MOLECULE);
    }

    void check_reaction(int index_a, int index_b, std::vector<Molecule*>& molecules) {
        Molecule* a = molecules[index_a];
        Molecule* b = molecules[index_b];
        const std::type_info& a_type = typeid(*a);
        const std::type_info& b_type = typeid(*b);

        auto iter_1 = std::find_if(molecule_types.begin(), molecule_types.end(),
                                   [&a_type](const std::type_info* type) { return *type == a_type; });
        auto iter_2 = std::find_if(molecule_types.begin(), molecule_types.end(),
                                   [&b_type](const std::type_info* type) { return *type == b_type; });

        if (iter_1 != molecule_types.end() && iter_2 != molecule_types.end()) {
            int index_1 = std::distance(molecule_types.begin(), iter_1);
            int index_2 = std::distance(molecule_types.begin(), iter_2);

            (this->*chem_mgr[index_1][index_2])(index_a, index_b, molecules);
        } else {
            std::cout << "Молекулы не могут взаимодействовать.\n";
        }
    }

    ~Chem_Mgr() = default;
};

#endif
