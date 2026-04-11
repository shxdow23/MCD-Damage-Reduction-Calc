#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Additive Sources (xHP values)
float Protection = 1.18;
float Armour_35 = 1.538;
float Iron_Hide_Amulet = 2.000;
float Dense_Brew = 4.000;
float Potion_Barrier = 10.000;
float Oak_Brew = 2.000;
float Shulker_Dr = 2.000;
float Gohst_Cloak = 1.500;

// Multiplicative Sources (xHP values)
float Gong_of_Weakening = 1.333;
float Miss_30 = 1.429;
float Weakening = 1.667;
float Deflect = 1.818;
float Guarding_Strike = 2.000;
float Weakening_Gong_Combo = 2.000;
float Squid_Armour = 2.000;
float Environmental_Protection = 4.000;
float Invulnerable_Spam = 4.210;

float Life_Boost(int deaths) {
    return 1 + (0.33 * deaths);
}

struct DR_Option {
    string name;
    float xHP;
    bool isMultiplicative;
    bool isNC;
};

int main() {
    vector<DR_Option> options = {
        // Additive
        {"Protection",            Protection,              false, false},
        {"Armour (35% DR)",       Armour_35,               false, false},
        {"Iron Hide Amulet",      Iron_Hide_Amulet,        false, false},
        {"Potion Barrier",        Potion_Barrier,          false, false},
        {"Shulker Dr",            Shulker_Dr,              false, false},
        {"Ghost Cloak",           Gohst_Cloak,             false, false},
        {"Dense Brew",            Dense_Brew,              false, false},
        {"Oak Brew",              Oak_Brew,                false, false},
        // Multiplicative
        {"Gong of Weakening",     Gong_of_Weakening,       true,  false},
        {"Weakening",             Weakening,               true,  false},
        {"Guarding Strike",       Guarding_Strike,         true,  false},
        {"Weakening+Gong Combo",  Weakening_Gong_Combo,    true,  false},
        {"Environmental Protect", Environmental_Protection, true,  false},
        {"Life Boost",            0,                       true,  false},
        // NC (Negate Chance)
        {"Deflect",               Deflect,                 true,  true},
        {"Squid Armour",          Squid_Armour,            true,  true},
        {"Invulnerable Spam",     Invulnerable_Spam,       true,  true},
        {"30% Miss",              Miss_30,                 true,  true}
    };

    cout << "=== MCD DR Calculator ===" << endl;

    cout << "--- ADDITIVE SOURCES ---" << endl;
    for (int i = 0; i < options.size(); i++)
        if (!options[i].isMultiplicative && !options[i].isNC)
            cout << i + 1 << ". " << options[i].name << " (" << options[i].xHP << "xHP)" << endl;

    cout << "\n--- MULTIPLICATIVE SOURCES ---" << endl;
    for (int i = 0; i < options.size(); i++)
        if (options[i].isMultiplicative && !options[i].isNC)
            cout << i + 1 << ". " << options[i].name << (options[i].name == "Life Boost" ? " (scales with deaths)" : " (" + to_string(options[i].xHP).substr(0, 5) + "xHP)") << endl;

    cout << "\n--- NEGATE CHANCE (NC) SOURCES ---" << endl;
    for (int i = 0; i < options.size(); i++)
        if (options[i].isNC)
            cout << i + 1 << ". " << options[i].name << " (" << options[i].xHP << "xHP)" << endl;

    cout << "\nEnter source numbers one at a time (0 to finish):" << endl;

    float additive_bonus = 0.0;
    float multiplicative = 1.0;
    float nc_multiplier = 1.0;
    int choice;

    while (true) {
        cout << "> ";
        cin >> choice;
        if (choice == 0) break;
        if (choice < 1 || choice > (int)options.size()) {
            cout << "Invalid option." << endl;
            continue;
        }

        DR_Option& selected = options[choice - 1];

        if (selected.name == "Life Boost") {
            int deaths;
            cout << "Enter death count: ";
            cin >> deaths;
            float lb = Life_Boost(deaths);
            cout << "Added: Life Boost (" << lb << "xHP) [multiplicative]" << endl;
            multiplicative *= lb;
            continue;
        }

        if (selected.isNC) {
            cout << "Added: " << selected.name << " (" << selected.xHP << "xHP) [NC]" << endl;
            nc_multiplier *= selected.xHP;
        } else if (selected.isMultiplicative) {
            cout << "Added: " << selected.name << " (" << selected.xHP << "xHP) [multiplicative]" << endl;
            multiplicative *= selected.xHP;
        } else {
            cout << "Added: " << selected.name << " (" << selected.xHP << "xHP) [additive]" << endl;
            additive_bonus += selected.xHP - 1;
        }
    }

    // Calculate totals
    float additive_EHP = 1 + additive_bonus;
    float total_EHP = additive_EHP * multiplicative;
    float total_DR = (1 - (1 / total_EHP)) * 100;
    float total_NC = (1 - (1 / nc_multiplier)) * 100;

    cout << "\n=== RESULTS ===" << endl;
    cout << "Additive EHP:   " << additive_EHP << "xHP" << endl;
    cout << "Multiplicative: x" << multiplicative << endl;
    cout << "Total EHP:      " << total_EHP << "xHP" << endl;
    cout << "Total DR:       " << total_DR << "%" << endl;
    cout << "Total NC:       " << total_NC << "%" << endl;

    return 0;
}
