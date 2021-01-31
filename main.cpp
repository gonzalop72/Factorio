#include <iostream>
#include <fstream>
#include <string>
#include <map>
//#include <memory>

#include "json.hpp"
#include "factorioBase.hpp"

// for convenience
using json = nlohmann::json;
using std::string;
using namespace factorio;

template<class T> std::map<string, std::shared_ptr<T>> load(gameContext& context, const string& fileName) {

  std::map<string, std::shared_ptr<T>> items;

  std::ifstream file(fileName);
 
  json jsonData;
  file >> jsonData;

  auto serializer = T::get_serializer();

  for (json::iterator it = jsonData.begin(); it != jsonData.end(); ++it) {
    T newItem = serializer.deserialize(context, it.key(), *it);
    auto item_p = std::make_shared<T>(newItem);
    items.insert({newItem.get_name(), item_p});
  }
  return items;
}

int main(int, char**) {
 
    std::cout << "Hello world!" << std::endl;

    json j2 = {
  {"pi", 3.141},
  {"happy", true},
  {"name", "Niels"},
  {"nothing", nullptr},
  {"answer", {
    {"everything", 42}
  }},
  {"list", {1, 0, 2}},
  {"object", {
    {"currency", "USD"},
    {"value", 42.99}
  }}
};

    std::cout << j2;

  factorio::gameContext context;

  std::ifstream myfile ("../jsons/factorio-data/item.json");
  context.items = load<factorio::item>(context, "../jsons/factorio-data/item.json");
  context.recipes = load<factorio::recipe>(context, "../jsons/factorio-data/recipe.json");
  context.factories = load<factorio::factory>(context, "../jsons/factorio-data/factory.json");
  context.technologies = load<factorio::technology>(context, "../jsons/factorio-data/technology.json");
  
  for(auto it2 = context.technologies.begin(); it2 != context.technologies.end(); ++it2) {
    it2->second->postDeserialize(context);
  }

  /*if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      //use line here
    }
    myfile.close();
  }

  else cout << "Unable to open file"; */

    //std::vector itemsv;
    json itemsAsJson;
    myfile >> itemsAsJson;
/*
for (json::iterator it = itemsAsJson.begin(); it != itemsAsJson.end(); ++it) {
  std::cout << it.key() << "," << it.value() << '\n';
  
}*/
for (const auto& [name, ii] : context.technologies) {
      //std::cout << std::setw(10) << name << " | " << ii->get_category() << "\n";
      std::cout << std::setw(10) << name << " | "; ii->showEffects(); std::cout << "\n";
      //std::cout << std::setw(10) << name << " | " << ii->get_speed() << "\n";
}
    char chr;
    std::cin >> chr;
    return 0;
}