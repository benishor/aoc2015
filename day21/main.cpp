#include "solution.h"

const bool debug = false;

struct character {
    int hit_points{100};
    int armor{0};
    int damage_score{0};
    std::string name;

    [[nodiscard]] bool is_dead() const {
        return hit_points <= 0;
    }

    void attack(character& other) const {
        int hp_loss = damage_score - other.armor;
        if (hp_loss < 1) {
            hp_loss = 1;
        }
        other.hit_points -= hp_loss;
        if (debug)
            std::cout << name << " deals " << damage_score << " - " << other.armor << " = " << hp_loss
                      << " damage; "
                      << other.name << " goes down to " << other.hit_points << " hit points" << std::endl;
    }
};

enum class winner {
    player,
    boss
};

struct item {
    std::string name;
    int cost;
    int damage;
    int armor;
};

std::ostream& operator<<(std::ostream& out, const item& i) {
    out << i.name << ", cost: " << i.cost << ", damage: " << i.damage << ", armor: " << i.armor;
    return out;
}

struct item_set {
    void add_item(const item& i) {
        items.push_back(i);
        cost += i.cost;
        damage += i.damage;
        armor += i.armor;
    }

    std::vector<item> items;

    int cost{0};
    int damage{0};
    int armor{0};
};

std::ostream& operator<<(std::ostream& out, const item_set& set) {
    out << "Item set: " << std::endl;
    out << "\tcost: " << set.cost << std::endl;
    out << "\tdamage: " << set.damage << std::endl;
    out << "\tarmor: " << set.armor << std::endl;
    out << "Items: " << std::endl;
    for (auto& item : set.items) {
        out << "\t" << item << std::endl;
    }
    return out;
}


struct store {
    std::vector<item> weapons;
    std::vector<item> armor;
    std::vector<item> rings;

    store() {
        weapons.push_back(item{.name = "Dagger", .cost = 8, .damage = 4, .armor = 0});
        weapons.push_back(item{.name = "Shortsword", .cost = 10, .damage = 5, .armor = 0});
        weapons.push_back(item{.name = "Warhammer", .cost = 25, .damage = 6, .armor = 0});
        weapons.push_back(item{.name = "Longsword", .cost = 40, .damage = 7, .armor = 0});
        weapons.push_back(item{.name = "Greataxe", .cost = 74, .damage = 8, .armor = 0});

        armor.push_back(item{.name = "No armor", .cost = 0, .damage = 0, .armor = 0});
        armor.push_back(item{.name = "Leather", .cost = 13, .damage = 0, .armor = 1});
        armor.push_back(item{.name = "Chainmail", .cost = 31, .damage = 0, .armor = 2});
        armor.push_back(item{.name = "Splintmail", .cost = 53, .damage = 0, .armor = 3});
        armor.push_back(item{.name = "Bandedmail", .cost = 75, .damage = 0, .armor = 4});
        armor.push_back(item{.name = "Platemail", .cost = 102, .damage = 0, .armor = 5});

        rings.push_back(item{.name = "Damage +1", .cost = 25, .damage = 1, .armor = 0});
        rings.push_back(item{.name = "Damage +2", .cost = 50, .damage = 2, .armor = 0});
        rings.push_back(item{.name = "Damage +3", .cost = 100, .damage = 3, .armor = 0});
        rings.push_back(item{.name = "Defense +1", .cost = 20, .damage = 0, .armor = 1});
        rings.push_back(item{.name = "Defense +2", .cost = 40, .damage = 0, .armor = 2});
        rings.push_back(item{.name = "Defense +3", .cost = 80, .damage = 0, .armor = 3});

        prepare_combinations();
        std::sort(std::begin(item_sets), std::end(item_sets), [](const item_set& a, const item_set& b) {
            return a.cost < b.cost;
        });
    }

    std::vector<item_set> item_sets;

    void prepare_combinations() {
        // - exactly one weapon
        // - 0 or 1 armor
        // - 0, 1 or 2 armor

        item_set set{};
        for (auto& w : weapons) {
            for (auto& a : armor) {
                // no ring
                set = {};
                set.add_item(w);
                set.add_item(a);
                item_sets.push_back(set);

                // one ring
                for (auto& r : rings) {
                    set = {};
                    set.add_item(w);
                    set.add_item(a);
                    set.add_item(r);
                    item_sets.push_back(set);
                }

                // two rings
                auto ring_combinations = aoc::combinations({0, 1, 2, 3, 4, 5}, 2);
                do {
                    auto current_rings = ring_combinations.get();
                    set = {};
                    set.add_item(w);
                    set.add_item(a);
                    set.add_item(rings[current_rings[0]]);
                    set.add_item(rings[current_rings[1]]);
                    item_sets.push_back(set);
                } while (ring_combinations.next());
            }
        }
    }
};


class day21 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {
        int boss_hp{0}, boss_armor{0}, boss_damage{0};

        for (std::string line; std::getline(in, line);) {
            auto pieces = aoc::split_string(line, ":");
            auto name = aoc::trim(pieces[0]);
            auto value = std::stoi(aoc::trim(pieces[1]));
            if (name == "Hit Points")
                boss_hp = value;
            else if (name == "Damage")
                boss_damage = value;
            else if (name == "Armor")
                boss_armor = value;
        }

        // part 1
        store st{};
        for (auto& set : st.item_sets) {
            if (debug) std::cout << set << std::endl;
            character player{
                    .hit_points = 100,
                    .armor = set.armor,
                    .damage_score = set.damage,
                    .name = "player"
            };

            character boss{
                    .hit_points = boss_hp,
                    .armor = boss_armor,
                    .damage_score = boss_damage,
                    .name = "boss"
            };

            if (run_fight(player, boss) == winner::player) {
                out << set.cost << std::endl;
                break;
            }
        }

        // part 2
        for (auto it = st.item_sets.rbegin(); it != st.item_sets.rend(); it++) {
            auto set = *it;
            if (debug) std::cout << set << std::endl;
            character player{
                    .hit_points = 100,
                    .armor = set.armor,
                    .damage_score = set.damage,
                    .name = "player"
            };

            character boss{
                    .hit_points = 100,
                    .armor = 2,
                    .damage_score = 8,
                    .name = "boss"
            };

            if (run_fight(player, boss) == winner::boss) {
                out << set.cost << std::endl;
                break;
            }
        }
    }

    static winner run_fight(character player, character boss) {
        if (debug) std::cout << std::endl << "Running fight!" << std::endl;
        while (true) {
            player.attack(boss);
            if (boss.is_dead()) {
                if (debug) std::cout << "The winner is: player!" << std::endl;
                return winner::player;
            }

            boss.attack(player);
            if (player.is_dead()) {
                if (debug) std::cout << "The winner is: boss!" << std::endl;
                return winner::boss;
            }
        }
    }
};

int main(int argc, char** argv) {
    day21 solution;
    return solution.execute(argc, argv);
}
