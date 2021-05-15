
#include "gameplay/PandemicShop/item.h"
#include "gameplay/PandemicShop/pandemic_character_controller.h"
#include "core/graphics/animation.h"

using namespace pandemic;


void Pickable::Run(){

    
    auto &reg = Engine::Registry();
    auto items = reg.view<SimpleCollision, Transform, Sprite, Item>();
    auto hero = reg.view<SimpleCollision, Transform, Sprite, Animator, PandemicCharacter>().begin();

    items.each([&] (Entity ent, SimpleCollision &col_, Transform &transf_, Sprite &sprite_, Item &item_){
                    auto msg = fmt::format("\n{}\n", col_.colided);
                    printf(msg.c_str());
                    auto &hero_transf = reg.get<Transform>(*hero);
                    auto &hero_col = reg.get<SimpleCollision>(*hero);
                    if(col_.colided){
                        printf("\ncollided\n");
                        auto possibly_hero = col_.colidedWith;
                        if(reg.has<SimpleCollision, Transform, Animator, PandemicCharacter>(possibly_hero)){
                            printf("\ncollided with item\n");
                            item_.hidden = true;
                            reg.remove<SimpleCollision, Sprite>(ent);
                            // Character::addItem(ent);
                            // item.setHidden(ent);   
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


