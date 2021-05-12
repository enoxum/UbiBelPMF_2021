#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/engine.h"

#include <map>
#include <utility>
#include <string>

using namespace dagger;

namespace atonement {


    class CooldownManager : public System
    {

    public:
        CooldownManager();

        String SystemName() override {
            return "Groundedness Detection System";
        }

        void Run() override;

        /* entity and name are a combination of entity to which the cooldown 
        applies and a name of a cooldown, if two cooldowns are registered 
        with the same entity-name combo the older one will be overwritten
        
        * once a cooldown is registered, the timer starts immediately */
        void registerCooldown(Entity entity, std::string name, Float32 duration);

        Bool isOnCooldown(Entity entity, std::string name);

    private:
        class Cooldown;

        std::map<std::pair<Entity, std::string>, Cooldown> lookup = std::map<std::pair<Entity,String>, Cooldown>();

        class Cooldown
        {
        public:
            Cooldown(Entity entity, std::string name, Float32 duration);
            Cooldown(const Cooldown& other);

            const Entity entity;
            const std::string name;
            const Float32 duration;
            Float32 time = 0;
        };
    };

}