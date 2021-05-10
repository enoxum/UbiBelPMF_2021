#include "cooldown_manager.h"
#include <iostream>

using namespace dagger;
using namespace atonement;

CooldownManager::CooldownManager()
{
    canBePaused = true;
}

void CooldownManager::Run() {
    for (auto& [key, val] : lookup) {
        val.time += Engine::DeltaTime();
    }
}

void CooldownManager::registerCooldown(Entity entity, std::string name, Float32 duration) {
    std::pair<Entity, std::string> key = std::make_pair(entity, name);
    auto res = lookup.find(key);
    if (res != lookup.end()) {
        // found
        lookup.erase(res);
    }

    lookup.insert(std::make_pair( key, Cooldown(entity, name, duration) ));
}

Bool CooldownManager::isOnCooldown(Entity entity, std::string name) {
    std::pair<Entity, std::string> key = std::make_pair(entity, name);
    auto res = lookup.find(key);
    if (res == lookup.end()) {
        // not found
        return false;
    }
    else {
        // found
        auto& cd = res->second;
        if (cd.time < cd.duration) {
            return true;
        }
        else {
            return false;
        }
    }


}


CooldownManager::Cooldown::Cooldown(Entity entity, std::string name, Float32 duration)
    : entity(entity), name(name), duration(duration) {}


CooldownManager::Cooldown::Cooldown(const Cooldown& other)
    : entity(other.entity), name(other.name), duration(other.duration) {}