#include <string>
#include <vector>

#include "json.hpp"
#include "factorioBase.hpp"

#include <iostream>

using nlohmann::json;
using std::string;
using std::vector;
using namespace factorio;

namespace factorio {

void technology::showEffects() {
    for(auto it = effects.begin(); it != effects.end(); ++it) {
        std::cout << it->type << "," << it->recipeName << "; -";
    }
}

itemSerializer item::get_serializer() { return itemSerializer(); }

void itemSerializer::from_json(const json& j, item& i) {
    //i.name = j.key();
}

item itemSerializer::deserialize(const gameContext& context, const string& name, const json& j) {
    item i;

    i.name = name;
    from_json(j, i);
    return i;
}

recipeSerializer recipe::get_serializer() { return recipeSerializer(); }

void recipeItemSerializer::from_json(const json& j, recipeItem& i) {
    //i.name = j.key();
    j.at("amount").get_to(i.amount);
}

void recipeSerializer::from_json(const json& j, recipe& r) {
    //r.name = j.key();
    j.at("category").get_to(r.category);
    j.at("energy").get_to(r.energy);
    j.at("enabled").get_to(r.enabled);

    auto ji = j.at("ingredients");
    auto jp = j.at("products");
    
    for (json::iterator it1 = ji.begin(); it1 != ji.end(); ++it1) {
        recipeItem item;
        recipeItemSerializer().from_json(*it1, item);
        r.ingredients.push_back(item);
    }

    for (json::iterator it2 = jp.begin(); it2 != jp.end(); ++it2) {
        recipeItem item;
        recipeItemSerializer().from_json(*it2, item);
        r.products.push_back(item);
    }
}

recipe recipeSerializer::deserialize(const gameContext& context, const string& name, const json& j) {
    recipe i;

    i.name = name;
    from_json(j, i);
    return i;
}

factorySerializer factory::get_serializer() { return factorySerializer(); }

void factorySerializer::from_json(const gameContext& context, const json& j, factory& f) {
    //r.name = j.key();
    j.at("crafting_speed").get_to(f.speed);

    auto ji = j.at("crafting_categories");
    
    for (json::iterator it = ji.begin(); it != ji.end(); ++it) {

        auto name = it.key();
        f.categories.push_back(name);
    }
}

factory factorySerializer::deserialize(const gameContext& context, const string& name, const json& j) {
    factory i;

    i.name = name;
    from_json(context, j, i);
    return i;
}

void effectSerializer::from_json(const json& j, effect& e) {
    j.at("type").get_to(e.type);
    j.at("recipe").get_to(e.recipeName);
}


technologySerializer technology::get_serializer() { return technologySerializer(); }

void technologySerializer::from_json(const gameContext& context, const json& j, technology& t) {
    //r.name = j.key();
 
    auto ji = j.at("ingredients");
    auto jp = j.at("prerequisites");
    auto je = j.at("effects");
    
    for (json::iterator it1 = ji.begin(); it1 != ji.end(); ++it1) {
        recipeItem item;
        recipeItemSerializer().from_json(*it1, item);
        t.ingredients.push_back(item);
    }
       
    for (json::iterator it2 = jp.begin(); it2 != jp.end(); ++it2) {
        
        string preName = *it2;
        t.prerequisitesAsStringList.push_back(preName);
    }

    for (json::iterator it3 = je.begin(); it3 != je.end(); ++it3) {
        effect item;
        effectSerializer().from_json(*it3, item);
        t.effects.push_back(item);
    }
}

technology technologySerializer::deserialize(const gameContext& context, const string& name, const json& j) {
    technology i;

    i.name = name;
    from_json(context, j, i);
    return i;
}

void technology::postDeserialize(const gameContext& context) {

    auto it = prerequisitesAsStringList.begin();

    for(; it != prerequisitesAsStringList.end(); it++) {
        string techName = *it;

        auto iter = context.technologies.find(techName);
        if (iter != context.technologies.end()) {
            auto item = iter->second;
            prerequisites.push_back(item);
        }
        else {
            std::cout << techName << "not found!!" << std::endl;
        }
    }
}

}