#include "cooldown_manager.h"
#include <iostream>

using namespace dagger;

void CooldownManager::Run() {
    for (auto& [key, val] : lookup) {
        val.time += Engine::DeltaTime();
    }
}

void CooldownManager::registerCooldown(const Entity& entity, const std::string& name, Float32 duration) {
    auto key = std::make_pair(entity, name);
    auto res = lookup.find(key);

    if (res != lookup.end()) {
        lookup.erase(res);
    }

    lookup.insert(std::make_pair( key, Cooldown(entity, name, duration) ));
}

Bool CooldownManager::isOnCooldown(const Entity& entity, const std::string& name) {
    auto key = std::make_pair(entity, name);
    auto res = lookup.find(key);
    if (res == lookup.end()) {  // not found, so never registered
        return false;
    }
    else { // found

        auto& cd = res->second;
        if (cd.time < cd.duration) { // hasn't expired
            return true;
        }
        else { // expired
            return false;
        }
    }


}


CooldownManager::Cooldown::Cooldown(const Entity& entity, const std::string& name, Float32 duration)
    : entity(entity), name(name), duration(duration) {}


CooldownManager::Cooldown::Cooldown(const Cooldown& other)
    : entity(other.entity), name(other.name), duration(other.duration) {}