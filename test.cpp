#define UNIT_TESTING
#include <gtest/gtest.h>
#include "json.hpp"
#include "main.cpp"

using namespace std;
using namespace testing;
using json = nlohmann::json;

void toLowerCase(string& text){
    for (char &symbol : text){
        symbol = tolower(symbol); 
    }
}

class GameTest : public Test{
    protected:
    json test_data;

    void SetUp() override{
        test_data = json::array(
            {
                {{"game", "Doom"}, {"cheat", "IDDQD"}},
                {{"game", "Minecraft"}, {"bug", "Сломанный бедрок"}, {"dificulty", "5"}},
                {{"game", "Portal"}, {"easter_egg", "The cake is a lie"}}
            }
        );
    }
};

TEST_F(GameTest, SearchCaseInsensitive){//поиск игры по названию с разным регистром 
    string userInput = "dOoM"; 
    bool wasFound = false;
    
    string lowerSearch = userInput; 
    toLowerCase(lowerSearch);

    for (auto& item : test_data){
        string gameName = item["game"].get<string>();
        string lowerGameName = gameName;
        toLowerCase(lowerGameName);

        if (lowerGameName.find(lowerSearch) < lowerGameName.length()){
            wasFound = true;
            EXPECT_EQ(item["cheat"], "IDDQD");
        }
    }
    EXPECT_TRUE(wasFound);
}

TEST_F(GameTest, SearchPartialMatch){//поиск игры по неполному названию
    string partOfName = "craft";
    bool wasFound = false;
    
    for (auto& item : test_data){
        string fullName = item["game"].get<string>();
        
        if (fullName.find(partOfName) < fullName.length()){
            wasFound = true;
            EXPECT_EQ(item["game"], "Minecraft");
        }
    }
    EXPECT_TRUE(wasFound);
}

TEST_F(GameTest, DeleteGame){//удаление игры из списка
    ASSERT_EQ(test_data.size(), 3);
    
    test_data.erase(0);
    
    EXPECT_EQ(test_data.size(), 2);
    EXPECT_EQ(test_data[0]["game"], "Minecraft");
}

TEST_F(GameTest, AddNewGame){//добавление новой игры
    json newGame;
    newGame["game"] = "Skyrim";
    newGame["bug"] = "Giant Space Program";
    
    test_data.push_back(newGame);
    
    EXPECT_EQ(test_data.size(), 4);
    EXPECT_EQ(test_data[3]["game"], "Skyrim");
}

TEST_F(GameTest, ChangeData){//изменение данных
    for (auto& item : test_data){
        if (item["game"] == "Minecraft"){
            item["bug"] = "New Bug Found";
        }
    }
    EXPECT_EQ(test_data[1]["bug"], "New Bug Found");
}

int main(){
    InitGoogleTest();
    return RUN_ALL_TESTS();
}