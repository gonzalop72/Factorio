#include <string>
#include <vector>
#include <map>
#include "json.hpp"

using nlohmann::json;
using std::string;
using std::vector;

#pragma once

namespace factorio {

class item;
class recipe;
class factory;
class technology;

class itemSerializer;
class recipeSerializer;
class recipeItemSerializer;
class factorySerializer;
class effectSerializer;
class technologySerializer;

class gameContext {
public:
    std::map<string, std::shared_ptr<item>> items;
    std::map<string, std::shared_ptr<recipe>> recipes;
    std::map<string, std::shared_ptr<factory>> factories;
    std::map<string, std::shared_ptr<technology>> technologies;
};

class item {
    public:
        const string& get_name() { return name; }

        static itemSerializer get_serializer();

    private:
        string name;

    friend itemSerializer;
};

class recipeItem {
    private:
        item item;
        int amount;

    friend recipeItemSerializer;
};

class recipe {
    public:
        const string& get_name() { return name; }
        const string& get_category() { return category; }

        static recipeSerializer get_serializer();

    private:
        string name;
        string category;
        int energy;
        vector<recipeItem> ingredients; 
        vector<recipeItem> products;
        bool enabled;

    friend recipeSerializer;
};

class factory {
    public:
        const string& get_name() { return name; }
        const double get_speed() { return speed; }

        static factorySerializer get_serializer();

    private:
        string name;
        double speed; //crafting speed
        vector<string> categories;

    friend factorySerializer;
};

class effect {
    private:
        string type;
        string recipeName;
    
    friend effectSerializer;
    friend technology;
};

class technology {
    public:
        const string& get_name() { return name; }

        static technologySerializer get_serializer();

    private:
        string name;
        vector<string> prerequisitesAsStringList;
        vector<std::shared_ptr<technology>> prerequisites;
        vector<recipeItem> ingredients;
        vector<effect> effects;

    public:
        void postDeserialize(const gameContext& context);
        void processEffects(const gameContext& context);

        void showEffects();

        bool is_available(const gameContext& curState) const;

    friend technologySerializer;
};


class itemSerializer {
public:
    static void from_json(const json& j, item& i);

    static item deserialize(const gameContext& context, const string& name, const json& j);
};

class recipeItemSerializer {
public:
    void from_json(const json& j, recipeItem& i);
};

class recipeSerializer {
public:
    void to_json(json& j, const recipe& i);

    static void from_json(const json& j, recipe& r);

    static recipe deserialize(const gameContext& context, const string& name, const json& j);
};

class factorySerializer {
public:
    void to_json(json& j, const factory& i);

    static void from_json(const gameContext& context, const json& j, factory& r);

    static factory deserialize(const gameContext& context, const string& name, const json& j);
};

class effectSerializer {
public:
    void from_json(const json& j, effect& i);
};

class technologySerializer {
public:
    void to_json(json& j, const technology& i);

    static void from_json(const gameContext& context, const json& j, technology& r);

    static technology deserialize(const gameContext& context, const string& name, const json& j);
};

}
