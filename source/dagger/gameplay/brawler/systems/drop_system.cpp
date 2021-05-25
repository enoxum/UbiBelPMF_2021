#include "drop_system.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/systems/shooting_system.h"

using namespace dagger;
using namespace brawler;

void DropSystem::Run()
{
    auto View = Engine::Registry().view<Player, InputReceiver>();

    for(auto entity : View){

        auto& input = Engine::Registry().get<InputReceiver>(entity);
        auto& player = Engine::Registry().get<Player>(entity);

        if (EPSILON_NOT_ZERO(input.Get("drop_weapon")))
        {   
            if(player.pickedUpWeapons > 0){
                
                int x = -1;

                if(player.active_weapon_idx == player.pickedUpWeapons-1){
                    x = player.active_weapon_idx - 1; 
                }

                for(int i = player.active_weapon_idx; i < player.pickedUpWeapons-1; i++){
                    player.weapons[i] = player.weapons[i+1];
                }
                if(x != -1){
                    player.active_weapon_idx = x;
                }
                player.pickedUpWeapons--;
                player.weapons.pop_back();

                if(player.weapons.size() == 0)
                    player.active_weapon_idx = -1;

                auto& sprite = Engine::Registry().get<Sprite>(player.currentWeapon);

                if(player.pickedUpWeapons == 0){
                    sprite.color = {1.0f, 1.0f, 1.0f, 0.0f};
                }else{
                    Weapon wp = player.weapons[player.active_weapon_idx];
                    ShootingSystem::editSprite(player.currentWeapon, wp, 1.0f);
                }
            }    
        }
    }
}