#include <iostream>
#include <string>
#include <fstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Game {
    public:
    string name;
    virtual void showInfo() = 0;
    virtual ~Game(){}
};

class Cheat : public Game {
    public:
    string code;
    Cheat(string name, string code) {
        this->name = name;
        this->code = code;
    }
    void showInfo() override {
        cout << "Игра: " << name << "\nЧит(ы):\n" << code << endl;
    }
};

class Bug : public Game {
    public:
    string bug;
    string dificulty;
    Bug(string name, string bug, string dificulty) {
        this->name = name;
        this->bug = bug;
        this->dificulty = dificulty;

    }
    void showInfo() override {
        cout << "Игра: " << name << "\nБаг(и):\n" << bug << "\nСложность выполнения бага(багов): " << dificulty << endl;
    }
};








class Interface{
    public:
    static void showMenu(json& data){
        while(true){
            int choice;
            cout << "Выбор действия:" << endl;
            cout << "[1] - просмотреть весь список игр" << endl;
            cout << "[2] - найти игры в категории" << endl;
            cout << "[3] - найти по названию" << endl;
            cout << "[4] - добавить игру" << endl;
            cout << "[5] - выход" << endl;
            cout << "выберите действие: ";
            cin >> choice;

            if (choice == 1){
                showAllGames(data);
            }
            else if(choice == 2){
                showCategory(data);
            }
            else if(choice == 3){
                searchByName(data);
            }
            else if(choice == 4){
                addGame(data);
            }
            else if(choice == 5){
                break;
            }
        }
    }

    /////////////////////////////////////////////////

    static void searchByName(json& data) {
        string searchName;
        cout << "Введите название игры: ";
        cin.ignore();
        getline(cin, searchName);

        string lower = searchName;
        for (char& c : lower) c = tolower(c);

        bool found = false;

        for (auto& item : data) {
            string currentGame = item["game"].get<string>();
            string lowerGame = currentGame;
            for (char& c : lowerGame){
                c = tolower(c);
            }

            if (lowerGame.find(lower) < lowerGame.length()){
                found = true;
                Game* game;

                if (item.contains("cheat")){
                    game = new Cheat(item["game"], item["cheat"]);
                } 
                else if (item.contains("bug")){
                    game = new Bug(item["game"], item["bug"], item["dificulty"]);
                }

                if (game){
                    game->showInfo();
                    delete game;
                }
            }
        }

        if (!found){
            cout << "Ничего не найдено" << endl;
        }
    }

    /////////////////////////////////////////////////

    static void showCategory(json& data){
        int type;

        cout << "Категории:" << endl;
        cout << "[1] - cheat" << endl;
        cout << "[2] - bug" << endl;
        cout << "Введите номер категории: ";
        cin >> type;
         
        for (auto item : data){
            Game* game = nullptr;
            if (type == 1 && item.contains("cheat")){
                game = new Cheat(item["game"], item["cheat"]);
            }
            else if (type == 2 && item.contains("bug")){
                game = new Bug(item["game"], item["bug"], item["dificulty"]);
            }
            if (game != nullptr){
                game->showInfo();
                delete game;
            }
        }
    }

    /////////////////////////////////////////////////

    static void showAllGames(json& data){
        int i = 0;
        int type;
        for (auto item : data){
            if (item.contains("cheat")){
                cout << "[" << i << "] - " << item["game"] << endl;
            }
            else if (item.contains("bug")){
                cout << "[" << i << "] - " << item["game"] << endl;
            }
            i++;
        }
        cout << "\nвыбор действия:\n[1] - назад\n[2] - перейти к игре по номеру\n[3] - поиск игры по названию: ";
        cin >> type;
        if (type == 1){
            showMenu(data);
        }
        else if (type == 2){
            showIndexGame(data);
        }
        else if (type == 3){
            searchByName(data);
        }
        
    }

    /////////////////////////////////////////////////

    static void showIndexGame(json& data){
        int numGame;
        int numType;
        cout << "введите номер игры: ";
        cin >> numGame;
        cout << "что показать по игре " << data[numGame]["game"] << ":\n[0]читы\n[1]баги\n[2]пасхалки\n:";
        cin >> numType;
        string keys[] = {"cheat", "bug", "easter_egg"};
        string currentKey = keys[numType];

        if (data[numGame].contains(currentKey)){
            cout << "Результат: " << data[numGame][currentKey] << endl;
        } 
        else{cout << "У этой игры нет поля " << currentKey << "!" << endl;}
    }

    /////////////////////////////////////////////////

    static void saveData(json& data){
        ofstream f("da.json");
        f << data.dump(4);
        f.close();
    }

    /////////////////////////////////////////////////

    static void addGame(json& data){
        int type;
        cout << "добавить: [1] - чит, [2] - баг: ";
        cin >> type;
        cin.ignore();
        
        string text;
        cout << "введите название игры: ";
        getline(cin, text);

        string lowerInput = text;
        for (char& c : lowerInput){
            c = tolower(c);
        }

        int foundIndex = -1;
        
        for (size_t i = 0; i < data.size(); i++){
            string lowerGame = data[i]["game"].get<string>();
            for (char& c : lowerGame){
                c = tolower(c);
            }

            if (lowerInput == lowerGame){
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1){
            cout << "\nТакая игра уже есть, выберите действие:\n";
            cout << "[1] - Дополнить существующую\n";
            cout << "[2] - Перезаписать данные\n";
            cout << "[3] - Отмена\n: ";
            int action;
            cin >> action;
            cin.ignore();

            if (action == 3){
                return;
            } 
            else if (action == 1 || action == 2){ 
                string inputStr;
                if (type == 1){
                    if (action == 2 && data[foundIndex].contains("cheat")){ 
                        cout << "Текущий чит: " << data[foundIndex]["cheat"] << endl;
                    }
                    
                    cout << "Введите чит код: ";
                    getline(cin, inputStr);
                    
                    if (action == 1 && data[foundIndex].contains("cheat")){ 
                        data[foundIndex]["cheat"] = data[foundIndex]["cheat"].get<string>() + "\n" + inputStr;
                    } 
                    else{
                        data[foundIndex]["cheat"] = inputStr; 
                    }
                }
                else if (type == 2){
                    if (action == 2 && data[foundIndex].contains("bug")){
                        cout << "Текущий баг: " << data[foundIndex]["bug"] << endl;
                        cout << "Текущая сложность: " << data[foundIndex].value("dificulty", "Нет данных") << endl;
                    }

                    cout << "Напишите название бага и опишите кратко его суть: ";
                    getline(cin, inputStr);
                    
                    if (action == 1 && data[foundIndex].contains("bug")){
                        data[foundIndex]["bug"] = data[foundIndex]["bug"].get<string>() + "\n" + inputStr;
                    }
                    else{
                        data[foundIndex]["bug"] = inputStr;
                    }

                    cout << "Напишите предположительную сложность (0-10): ";
                    getline(cin, inputStr);
                    
                    if (action == 1 && data[foundIndex].contains("dificulty")){
                        data[foundIndex]["dificulty"] = data[foundIndex]["dificulty"].get<string>() + "; " + inputStr;
                    } 
                    else{
                        data[foundIndex]["dificulty"] = inputStr;
                    }
                }
                
                saveData(data);
                return;
            }
        }

        json newGame;
        newGame["game"] = text;

        string inputStr;
        if (type == 1){
            cout << "введите чит код: ";
            getline(cin, inputStr);
            newGame["cheat"] = inputStr;
        }
        else if (type == 2){
            cout << "напишите название бага, если оно есть и опишите кратко его суть: ";
            getline(cin, inputStr);
            newGame["bug"] = inputStr;
            cout << "напишите предположительную сложность выполнениия бага от 0 до 10: ";
            getline(cin, inputStr);
            newGame["dificulty"] = inputStr;
        }
        else{
            cout << "error" << endl;
            return;
        }

        data.push_back(newGame);
        saveData(data);
    }
};

    /////////////////////////////////////////////////

int main(){
    ifstream f("da.json");
    json data = json::parse(f);

    // Interface::showAllGames(data);
    Interface::showMenu(data);

}