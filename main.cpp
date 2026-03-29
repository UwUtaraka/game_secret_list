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
        cout << "Игра: " << name << " | Код: " << code << endl;
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
        cout << "Игра: " << name << " | Баг: " << bug << " | Сложность выполнения бага: " << dificulty << endl;
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
            else if(choice == 4){
                addGame(data);
            }
            else if(choice == 5){
                break;
            }
        }
    }

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
        
    }

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

    static void saveData(json& data){
        ofstream f("da.json");
        f << data.dump(4);
        f.close();
    }

    static void addGame(json& data){
        int type;
        cout << "добавить: [1] - чит, [2] - баг: ";
        cin >> type;
        cin.ignore();
        
        json newGame;
        string text;
        cout << "введите название игры: ";
        getline(cin, text);
        newGame["game"] = text;

        if (type == 1){
            cout << "введите чит код(комбинацию): ";
            getline(cin, text);
            newGame["cheat"] = text;
        }
        else if (type == 2){
            cout << "напишите название бага, если оно есть и опишите кратко его суть: ";
            getline(cin, text);
            newGame["bug"] = text;
            cout << "напишите предположительную сложность выполнениия бага от 0 до 10: ";
            getline(cin, text);
            newGame["dificulty"] = text;
        }
        else{
            cout << "error" << endl;
            return;
        }

        data.push_back(newGame);
        saveData(data);

    }
};



int main(){
    ifstream f("da.json");
    json data = json::parse(f);

    // Interface::showAllGames(data);
    Interface::showMenu(data);

}