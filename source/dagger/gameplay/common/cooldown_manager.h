#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/engine.h"

#include <map>
#include <utility>
#include <string>

using namespace dagger;

class CooldownManager : public System
{

public:

    String SystemName() override {
        return "Cooldown Manager";
    }

    void Run() override;

    void registerCooldown(const Entity& entity, const std::string& name, Float32 duration);

    Bool isOnCooldown(const Entity& entity, const std::string& name);

private:
    class Cooldown;

    std::map<std::pair<Entity, std::string>, Cooldown> lookup = std::map<std::pair<Entity,String>, Cooldown>();

    class Cooldown
    {
    public:
        Cooldown(const Entity& entity, const std::string& name, Float32 duration);
        Cooldown(const Cooldown& other);

        Entity entity;
        std::string name;
        Float32 duration;
        Float32 time = 0;
    };
};