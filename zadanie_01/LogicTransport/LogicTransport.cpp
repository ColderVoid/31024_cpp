/*

Wykonał: Jakub Szubartowski (A01/C01/L02)   I Informatyka
                email: 31024@s.pm.szczecin.pl
              ZADANIE: KNAPSACK PR0BLEM SOLVER

*/

/*
Wedle zyczenia dodaje cos do pliku
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <string>
#include <limits>

using namespace std;

// options
const int gen_max_products = 15;                                                                                          // LINIJKA '20' DO ZMIANY OPCJI GENERATORA
const int upperLimit = 25;                                                                                                // NIE ZMIENIAĆ!! chyba ze doda sie unikalne nazwy do generatora \/
string template_products[upperLimit] = { "FlexiGrip", "TerraSync", "NovaSphere", "VitaNova", "SwiftLink", "MaxiLink", "EchoSync", "ZeniGlow", "AeroWave", "PulseMate", "PuraZen", "SwiftFlare", "TurboCharge", "EchoJet", "XeroGlide", "HyperSync", "LuminaTouch", "VitaSoul", "HyperPulse", "OptiFlex", "SnapFlare", "BlazeBurst", "SparkFusion", "LuminaLuxe", "LunaBloom"};
string ex_in_filename = "LogicTransport - products";
float weight_limit = 10;
bool isStarted = true;

struct ProductSpec
{

    string name;
    float price;
    float weight;
    
};

class ProductManager
{
    
public:
    
    vector<ProductSpec> products;

    void gen()
    {
        
        if (!products.empty()){ products.clear(); }
        
        random_device rd;
        mt19937 gen(rd());

        shuffle(template_products, template_products + 25, gen);
        string selected_product_names[gen_max_products];
        for (int i = 0; i < gen_max_products; ++i) {selected_product_names[i] = template_products[i];}

        uniform_real_distribution<double> distr_weight(0.5f, 3.0f);      // w
        uniform_real_distribution<double> distr_price(25.0f, 50.0f);      // p

        for (int i = 0; i < gen_max_products; ++i) {

            float weight_random_number = round(distr_weight(gen) * 100) / 100;
            float price_random_number = round(distr_price(gen) * 100) / 100;
            products.push_back({selected_product_names[i], price_random_number, weight_random_number});

        }
        
    }

    void show_struct()
    {
        if (!products.empty())
        {
            cout << "ITEMS IN INVENTORY: " << endl;
            for(int id = 0; id < products.size(); ++id)
            {
                cout << "[ID: " << id + 1 << "] NAME: " << products[id].name << ", WEIGHT: " << products[id].weight << " kg, PRICE: $" << products[id].price << endl;
            }
        }else{ cout << "Product struct is empty - nothing to show!" << endl;}
    }

    void export_struct() {

        if (!products.empty())
        {

            ofstream o(ex_in_filename + ".data");
            for (const auto& p : products) { o << p.name << " " << p.price << " " << p.weight << "\n"; }
            cout << endl << "[DATA] exported - filename: " << ex_in_filename << ".data" << endl;
            o.close();
            
        }else { cout << "STRUCTURE EMPTY - to generate select default RUN" << endl; }

    }

    void import_struct() {

        if (!products.empty()){ products.clear(); }

        const string filename = ex_in_filename + ".data";
        ifstream i(filename);
        if (i.good())
        {

            ProductSpec p;
            while (i >> p.name >> p.price >> p.weight) { products.push_back(p); }
            
        }else
        {

            cout << "Data file does not exist! (filename: " << ex_in_filename << ".data)" << endl;
            cout << "Generating default data.." << endl << endl;
            gen();
            
        }
        i.close();

    }
    
};

class ProblemSolver
{
public:
    
    float knapsack(vector<ProductSpec>& products, float limit)
    {
        unsigned int numberOfProducts = products.size();
        vector<vector<float>> optimalSolution(numberOfProducts + 1, vector<float>(int(limit) + 1, 0));

        for (int itemID = 1; itemID <= numberOfProducts; ++itemID)
        {
            for (int remainingCapacity = 0; remainingCapacity <= int(limit); ++remainingCapacity)
            {

                if (products[itemID - 1].weight > remainingCapacity)
                    optimalSolution[itemID][remainingCapacity] = optimalSolution[itemID - 1][remainingCapacity];
                else
                    optimalSolution[itemID][remainingCapacity] = max(optimalSolution[itemID -1][remainingCapacity], products[itemID - 1].price + optimalSolution[itemID - 1][int(remainingCapacity-products[itemID - 1].weight)]);
                
            }
            
        }

        cout << endl << "ITEMS IN KANPSACK: " << endl;
        int itemID = numberOfProducts, weight_limit = limit;
        while (itemID > 0 && weight_limit > 0)
        {

            if (optimalSolution[itemID][weight_limit] != optimalSolution[itemID - 1][weight_limit])
            {

                cout << products[itemID - 1].name << ": WEIGHT = " << products[itemID - 1].weight << " kg, PRICE = $" << products[itemID - 1].price << endl;
                weight_limit -= products[itemID - 1].weight;
                
            }
            itemID--;
        }

        return optimalSolution[numberOfProducts][int(limit)];
        
    }

    
};

class Options
{
public:

    vector<ProductSpec> products;
    ProductManager PMan;
    ProblemSolver PSolver;
    
    void logo()
    {

        system("cls");
        cout << endl << "MADE BY: Jakub Szubartowski (A01/C01/L02)   I Informatyka" << endl;
        cout << "               email: 31024@s.pm.szczecin.pl" << endl;
        cout << "               TASK: KNAPSACK PR0BLEM SOLVER" << endl << endl;
        
    }

    void menu()
    {
        
        cout << endl << "[RUN (random products)] -> 1" << endl;
        cout << "[RUN (from .data file)] -> 2" << endl;
        cout << "[EXPORT DATA] -> 3" << endl;
        cout << "[OPTIONS] -> 4" << endl;
        cout << "[EXIT] -> 0" << endl << endl;

        int option;
        while (true){
            
            cout << "[CHOICE] -> ";
            cin >> option;

            if (option >= 0 && option <= 4){ break; } else{

                cout << "SELECT AGAIN" << endl;
                continue;
                
            }
        }

        switch(option)
        {

        case 1:
            PMan.gen();
            cout << endl << "KNAPSACK COST: $" << PSolver.knapsack(PMan.products, weight_limit) << " WEIGHT LIMIT: " << weight_limit << " kg" << endl << endl;

            PMan.show_struct();
            break;

        case 2:
            PMan.import_struct();
            cout << endl << "KNAPSACK COST: $" << PSolver.knapsack(PMan.products, weight_limit) << " WEIGHT LIMIT: " << weight_limit << " kg" << endl << endl;
            
            PMan.show_struct();
            break;

        case 3:
            PMan.export_struct();
            break;

        case 4:
            change_values();
            break;

        case 0:
            isStarted = false;
            break;

        default:
            cout << "DEFAULT OPTION - NOT USED" << endl;
            isStarted = false;
            break;
            
        }
    }

    void change_values()
    {

        string new_filename;

        cout << endl << "[OPTIONS]: " << endl;
        cout << "[KNAPSACK: WEIGHT LIMIT (now: " << weight_limit << ")] -> 1" << endl;
        cout << "[GENERATOR: PRODUCT COUNT LIMIT(now: " << gen_max_products << ")] -> 2" << endl;
        cout << "[IMPORT/EXPORT: FILENAME(now: '" << ex_in_filename << "')] -> 3" << endl;
        cout << "[STRUCT: SHOW] -> 4" << endl;
        cout << "[STRUCT: GENERATE WITHOUT RUNNING] -> 5" << endl;
        cout << "[BACK] -> 0" << endl << endl;

        int option;
        while (true){
            
            cout << "[CHOICE] -> ";
            cin >> option;

            if (option >= 0 && option <= 5){ break; } else{

                cout << "SELECT AGAIN" << endl;
                continue;
                
            }
        }

        switch(option)
        {

        case 1:
            float new_weight_value;
            cout << "CURRENT KNAPSACK WEIGHT LIMIT: " << weight_limit << endl;
            cout << "ENTER NEW VALUE (float, positive && not zero): ";
            cin >> new_weight_value;
            if (new_weight_value > 0){ weight_limit = new_weight_value; }else{ cout << "WRONG VALUE!" << endl; }
            break;

        case 2:
            cout << "CURRENT GENERATOR LIMIT: " << gen_max_products << endl;
            cout << "CONST NUMBER! - ENTER NEW VALUE IN LINE '20' of LogicTransport.cpp";
            break;

        case 3:
            cout << "CURRENT FILENAME: '" << ex_in_filename << "'" << endl;
            cout << "ENTER NEW VALUE (string): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, new_filename);
            ex_in_filename = new_filename;
            break;

        case 4:
            PMan.show_struct();
            break;

        case 5:
            PMan.gen();
            cout << "Generating... Done!" << endl;
            break;

        case 0:
            cout << endl;
            break;

        default:
            cout << "DEFAULT OPTION - NOT USED" << endl;
            break;
            
        }
    }
};

int main()
{
    Options opt;
    opt.logo();
    while (isStarted){ opt.menu(); }

    return 0;
}
