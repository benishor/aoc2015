#include "solution.h"
#include <memory>
#include <utility>

const bool debug = false;

enum spell_type {
    magic_missile = 0,
    drain = 1,
    shield = 2,
    poison = 3,
    recharge = 4
};

std::vector<spell_type> all_spells = {magic_missile, drain, shield, poison, recharge};

struct game_state {
    int hp;
    int mana;
    int armor;

    int boss_hp;
    int boss_damage;

    std::map<spell_type, int> active_spells; // spell => timer
};

template<typename TK, typename TV>
std::vector<TK> extract_keys(std::map<TK, TV> const& input_map) {
    std::vector<TK> retval;
    for (auto const& element: input_map) {
        retval.push_back(element.first);
    }
    return retval;
}

struct spell {
    static std::string name(spell_type type) {
        switch (type) {
            case magic_missile:
                return "Magic Missile";
            case drain:
                return "Drain";
            case shield:
                return "Shield";
            case poison:
                return "Poison";
            case recharge:
                return "Recharge";
            default:
                return "!!!! Unknown";
        }
    }

    static int mana_cost(spell_type type) {
        switch (type) {
            case magic_missile:
                return 53;
            case drain:
                return 73;
            case shield:
                return 113;
            case poison:
                return 173;
            case recharge:
                return 229;
            default:
                return -1;
        }
    }

    static int active_turns(spell_type type) {
        switch (type) {
            case magic_missile:
            case drain:
                return 0;
            case shield:
            case poison:
                return 6;
            case recharge:
                return 5;
            default:
                return 0;
        }
    }


    static void on_cast(spell_type type, game_state& state) {
        state.mana -= spell::mana_cost(type);

        switch (type) {
            case magic_missile:
                state.boss_hp -= 4;
                if (debug) std::cout << "Player casts Magic Missile, dealing 4 damage." << std::endl;
                break;
            case drain:
                state.boss_hp -= 2;
                state.hp += 2;
                if (debug) std::cout << "Player casts Drain, dealing 2 damage, and healing 2 hit points." << std::endl;
                break;
            case shield:
                state.armor += 7;
                if (debug) std::cout << "Player casts Shield, increasing armor by 7." << std::endl;
                break;
            case poison:
                if (debug) std::cout << "Player casts Poison." << std::endl;
                break;
            case recharge:
                if (debug) std::cout << "Player casts Recharge." << std::endl;
                break;
            default:
                if (debug) std::cout << "!!!!!!! Casting unknown effect." << std::endl;
                break;
        }

        int turns = active_turns(type);
        if (turns > 0) {
            state.active_spells[type] = turns;
        }
    }

    static void apply_all(game_state& state) {
        for (auto& key: extract_keys(state.active_spells)) {
            apply(key, state);
        }
    }

    static void apply(spell_type type, game_state& state) {
        state.active_spells[type]--;
        switch (type) {
            case poison:
                state.boss_hp -= 3;
                if (debug)
                    std::cout << "Poison deals 3 damage; its timer is now " << state.active_spells[type] << "."
                              << std::endl;
                break;
            case recharge:
                state.mana += 101;
                if (debug)
                    std::cout << "Recharge provides 101 mana; its timer is now " << state.active_spells[type] << "."
                              << std::endl;
                break;
            default:
                break;
        }
        if (state.active_spells[type] == 0) {
            on_timer_zero(type, state);
        }
    }

    static void on_timer_zero(spell_type type, game_state& state) {
        if (debug) std::cout << name(type) << " wears off." << std::endl;

        if (type == shield) {
            state.armor -= 7;
        }

        auto it = state.active_spells.find(type);
        state.active_spells.erase(it);
    }
};

enum class game_difficulty {
    easy,
    hard
};

class game_play {
public:

    explicit game_play(game_difficulty _difficulty) : difficulty{_difficulty} {}

    int play(game_state state) {
        int minimum_mana = std::numeric_limits<int>::max();
        player_round(std::move(state), minimum_mana, 0);
        return minimum_mana;
    }

private:

    const game_difficulty difficulty;

    static std::vector<spell_type> available_spells(game_state& state) {
        std::vector<spell_type> result;
        for (auto current_spell: all_spells) {
            if (state.active_spells.find(current_spell) == state.active_spells.end() &&
                state.mana >= spell::mana_cost(current_spell)) {
                result.push_back(current_spell);
            }
        }
        return result;
    }

    static void print_stats(game_state& state) {
        std::cout << "- Player has " << state.hp << " hit points, " << state.armor << " armor, " << state.mana
                  << " mana." << std::endl;
        std::cout << "- Boss has " << state.boss_hp << " hit points." << std::endl;
    }

    static void on_player_win(int& minimum_mana, int spent_mana) {
        if (debug) std::cout << "Player won with " << spent_mana << std::endl;
        if (minimum_mana > spent_mana) {
            minimum_mana = spent_mana;
            if (debug) std::cout << "New minimum win found! " << minimum_mana << std::endl;
        }
    }

    void player_round(game_state state, int& minimum_mana, int spent_mana) {
        if (difficulty == game_difficulty::hard) {
            state.hp--;
        }

        // early out. no point in continuing
        if (state.hp <= 0 || spent_mana >= minimum_mana) return;

        if (debug) {
            std::cout << "-- Player turn --" << std::endl;
            print_stats(state);
        }

        // run effects
        spell::apply_all(state);
        if (state.boss_hp <= 0) {
            minimum_mana = std::min(minimum_mana, spent_mana);
            return;
        }

        // player moves
        auto valid_spells = available_spells(state);
        if (debug) {
            std::cout << "Available spells: ";
            for (auto available_spell: valid_spells) {
                std::cout << spell::name(available_spell) << ", ";
            }
            std::cout << std::endl;
        }

        for (auto available_spell: valid_spells) {
            game_state new_state = state;
            spell::on_cast(available_spell, new_state);
            auto new_spent_mana = spent_mana + spell::mana_cost(available_spell);

            if (state.boss_hp <= 0) {
                on_player_win(minimum_mana, spent_mana);
            } else {
                boss_round(new_state, minimum_mana, new_spent_mana);
            }
        }
    }

    void boss_round(game_state state, int& minimum_mana, int spent_mana) {
        if (debug) {
            std::cout << std::endl << "-- Boss turn --" << std::endl;
            print_stats(state);
        }
        // run effects
        spell::apply_all(state);
        if (state.boss_hp <= 0) {
            on_player_win(minimum_mana, spent_mana);
            return;
        }

        // boss hits us
        int hp_loss = state.boss_damage - state.armor;
        if (hp_loss < 1) hp_loss = 1;
        if (debug) {
            std::cout << "Boss attacks for " << state.boss_damage << " - " << state.armor << " = " << hp_loss
                      << " damage; "
                      << "Player goes down to " << (state.hp - hp_loss) << " hit points" << std::endl;
        }
        state.hp -= hp_loss;

        if (debug) {
            std::cout << std::endl;
        }

        if (state.hp > 0) {
            player_round(state, minimum_mana, spent_mana);
        }
    }
};


class day22 : public aoc::solution {
protected:

    game_state new_state() {
        game_state state = {
                .hp = 50,
                .mana = 500,
                .armor = 0,
                .boss_hp = 55,
                .boss_damage = 8,
                .active_spells = {}
        };
        return state;
    }

    void run(std::istream& in, std::ostream& out) override {
        game_play game1{game_difficulty::easy};
        std::cout << game1.play(new_state()) << std::endl;

        game_play game2{game_difficulty::hard};
        std::cout << game2.play(new_state()) << std::endl;
    }
};

int main(int argc, char **argv) {
    day22 solution{};
    return solution.execute(argc, argv);
}
