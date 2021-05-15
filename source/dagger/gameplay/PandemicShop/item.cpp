
#include "gameplay/PandemicShop/item.h"
#include "gameplay/PandemicShop/pandemic_character_controller.h"
#include "core/graphics/animation.h"
#include "core/input/inputs.h"
#include "gameplay/PandemicShop/player.h"
using namespace pandemic;

void Pickable::Run(){

    
    auto &reg = Engine::Registry();
    auto items = reg.view<SimpleCollision, Transform, Sprite, Item>();
    auto hero_view = reg.view<SimpleCollision, Transform, Sprite, Animator, PandemicCharacter>().begin();

    items.each([&] (Entity ent, SimpleCollision &col_, Transform &transf_, Sprite &sprite_, Item &item_){
                    auto &hero_transf = reg.get<Transform>(*hero_view);
                    auto &hero_col = reg.get<SimpleCollision>(*hero_view);
                    if(col_.colided){
                        printf("\ncollided\n");
                        item_.pickable = true;
                        auto possibly_hero = col_.colidedWith;
                        if(reg.has<SimpleCollision, Transform, Animator, PandemicCharacter>(possibly_hero)){
                            printf("\ncollided with item\n");
                            auto hero = Character::Get(possibly_hero);
                            auto &inventory = hero.inventory;
                            item_.hidden = true;
                            reg.remove<SimpleCollision>(ent);
                            sprite_.scale = {0, 0};
                            inventory.emplace_back(ent);
                            printf("Inventory %d", inventory[0]);
                        }
                    }
                });

}

Item Item::Create(entt::entity &entity, String sprite_, ColorRGB color_ ,
                        Vector2 position_) {
    auto &reg = Engine::Registry();

    
    auto &sprite = reg.get_or_emplace<Sprite>(entity);
    auto &transform = reg.get_or_emplace<Transform>(entity);
    auto &collision = reg.get_or_emplace<SimpleCollision>(entity);
    auto item = Item{};
    
    transform.position = {position_, 0.0f};
    collision.size = {16, 16};
     
    sprite.scale = {1, 1};
    sprite.position = {position_, 0.0f};
    sprite.color = {color_, 1.0f};
 
    AssignSprite(sprite, sprite_);
    return item;
    
}


