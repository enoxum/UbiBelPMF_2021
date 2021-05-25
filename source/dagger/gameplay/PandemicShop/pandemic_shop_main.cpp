#include "pandemic_shop_main.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "tools/diagnostics.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/ping_pong/pingpong_ball.h"
#include "gameplay/ping_pong/player_scores.h"
#include "gameplay/ping_pong/pingpong_tools.h"
#include "gameplay/PandemicShop/pandemic_player_input.h"
#include "gameplay/PandemicShop/pandemic_tools.h"
#include "gameplay/PandemicShop/pandemic_character_controller.h"
#include "gameplay/PandemicShop/karen_controller.h"
#include "gameplay/PandemicShop/item.h"
#include "gameplay/PandemicShop/player.h"
#include "gameplay/PandemicShop/karen.h"
#include "gameplay/PandemicShop/ai_system.h"


using namespace dagger;
using namespace pandemic_shop;
//---------------------------
using namespace pandemic;


void PandemicShopGame::CoreSystemsSetup(Engine& engine_)
{
    engine_.AddSystem<WindowSystem>();
    engine_.AddSystem<InputSystem>();
    engine_.AddSystem<ShaderSystem>();
    engine_.AddSystem<TextureSystem>();
    engine_.AddSystem<SpriteRenderSystem>();
    engine_.AddPausableSystem<TransformSystem>();
    engine_.AddPausableSystem<AnimationSystem>();
#if !defined(NDEBUG)
    engine_.AddSystem<DiagnosticSystem>();
    // engine_.AddSystem<CollisionDetectionSystem>();
    engine_.AddSystem<GUISystem>();
    engine_.AddSystem<ToolMenuSystem>();
#endif //!defined(NDEBUG)
}

void PandemicShopGame::GameplaySystemsSetup(Engine& engine_)
{
    engine_.AddPausableSystem<SimpleCollisionsSystem>();
    engine_.AddPausableSystem<PandemicControllerSystem>();
    engine_.AddPausableSystem<KarenControllerSystem>();
    engine_.AddPausableSystem<CollisionDetectionSystem>();
    engine_.AddPausableSystem<Pickable>();
    engine_.AddPausableSystem<AISystem>();
#if defined(DAGGER_DEBUG)
    engine_.AddPausableSystem<ping_pong::PingPongTools>();
#endif //defined(DAGGER_DEBUG)
}

void PandemicShopGame::WorldSetup(Engine& engine_)
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    SetupWorld(engine_);
}
//---------------------------------------------------------


//--------------------------------------------

void pandemic_shop::SetupWorld(Engine& engine_)
{
    Vector2 scale(1, 1);

    auto& reg = engine_.Registry();

    // field
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    constexpr int height = 19;
    constexpr int width = 26;
    constexpr float tileSize = 20.f;// / static_cast<float>(Width);

    float zPos = 1.f;

    constexpr float Space = 0.1f;

    for (int i = 0; i < height; i++)
    {
        
        for (int j = 0; j < width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, "EmptyWhitePixel");
            sprite.size = scale * tileSize;

            if (i % 2 != j % 2)
            {
                sprite.color.r = 0.4f;
                sprite.color.g = 0.4f;
                sprite.color.b = 0.4f;
            }
            else
            {
                sprite.color.r = 0.6f;
                sprite.color.g = 0.6f;
                sprite.color.b = 0.6f;
                
            }

            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                sprite.color.r = 0.0f;
                sprite.color.g = 0.0f;
                sprite.color.b = 0.0f;

                auto &col = reg.emplace<SimpleCollision>(entity);
                reg.emplace<CollisionType::Wall>(entity);
                col.size.x = tileSize;
                col.size.y = tileSize;
                
            }

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = (0.5f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = zPos;
        }
    }

    zPos -= 1.f;

    // collisions
    {
        // up
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = tileSize * (width - 2)* (1 + Space);
            col.size.y = tileSize;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 0;
            transform.position.y = (0.5f + (height - 1) + (height - 1) * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = zPos;
        }

        // down
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = tileSize * (width - 2) * (1 + Space);
            col.size.y = tileSize;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 0;
            transform.position.y = (0.5f - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = zPos;
        }

        // left
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = tileSize;
            col.size.y = tileSize * (height - 2) * (1 + Space);

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = 0;
            transform.position.z = zPos;
        }

        // right
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = tileSize;
            col.size.y = tileSize * (height - 2) * (1 + Space);

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + (width - 1) + (width - 1) * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = 0;
            transform.position.z = zPos;

        }
    }

    const Float32 playerSize = tileSize;

    // player controller setup
    {
        auto* stats = new pandemic_shop::GameStats;
        stats->SetupPlayerBorders((height - 2)* (tileSize + Space + 5) / 2.f,
            -(height - 2) * (tileSize + Space + 5) / 2.f,
            (width - 2)* (tileSize + Space + 5) / 2.f,
            -(width - 2) * (tileSize + Space + 5) / 2.f);
        stats->PlayerSpeed = tileSize * 14.f;
        Engine::Instance().PutDefaultResource<pandemic_shop::GameStats>(stats);
        Engine::Instance();
    }

    //1st player
    {
        Character::Create("Pandemic", {1, 1, 1}, {64, 0});
        KarenCharacter::Create("Pandemic", {1, 1, 1}, {0, 0});
        KarenCharacter::Create("Pandemic", {0.5, 0.5, 0.5}, {128, 0});
        KarenCharacter::Create("Pandemic", {0.7, 0.7, 0.7}, {-128, 0});

        // auto ent1 = reg.create();
        // auto item1 = reg.emplace<Item>(ent1);
        
        // item1.Create(ent1, "spritesheets:pixel_mart:green_apple", {1, 1, 1}, {164, 164});
        // auto& collider = reg.get<SimpleCollision>(ent1);
        // collider.size = {32, 32};

        // auto entTS = reg.create();
        // auto itemTS = reg.emplace<Item>(entTS);
        
        // itemTS.Create(entTS, "spritesheets:shelf1:three_shelfs", {1, 1, 1}, {100, 100});
        // auto& collider = reg.get<SimpleCollision>(entTS);
        // collider.size = {32, 32};

//******************************SHELVES****************************************
        auto entTS = reg.create();
        auto& spriteTS = reg.emplace<Sprite>(entTS);
        AssignSprite(spriteTS, "PandemicShop:shelf1");
        auto &colliderTS = reg.emplace<SimpleCollision>(entTS);
        reg.emplace<CollisionType::Wall>(entTS);
        colliderTS.size.x = 100;
        colliderTS.size.y = 64;
        auto& transformTS = reg.emplace<Transform>(entTS);
        transformTS.position.x = -150;
        transformTS.position.y = 110;
        transformTS.position.z = 0.f;


        auto entTS1 = reg.create();
        auto& spriteTS1 = reg.emplace<Sprite>(entTS1);
        AssignSprite(spriteTS1, "PandemicShop:shelf1");
        auto &colliderTS1 = reg.emplace<SimpleCollision>(entTS1);
        reg.emplace<CollisionType::Wall>(entTS1);
        colliderTS1.size.x = 100;
        colliderTS1.size.y = 64;
        auto& transformTS1 = reg.emplace<Transform>(entTS1);
        transformTS1.position.x = -150;
        transformTS1.position.y = 0;
        transformTS1.position.z = 0.f;

        auto entTS2 = reg.create();
        auto& spriteTS2 = reg.emplace<Sprite>(entTS2);
        AssignSprite(spriteTS2, "PandemicShop:shelf1");
        auto &colliderTS2 = reg.emplace<SimpleCollision>(entTS2);
        reg.emplace<CollisionType::Wall>(entTS2);
        colliderTS2.size.x = 100;
        colliderTS2.size.y = 64;
        auto& transformTS2 = reg.emplace<Transform>(entTS2);
        transformTS2.position.x = -150;
        transformTS2.position.y = -110;
        transformTS2.position.z = 0.f;

        auto entTS3 = reg.create();
        auto& spriteTS3 = reg.emplace<Sprite>(entTS3);
        AssignSprite(spriteTS3, "PandemicShop:shelf1");
        auto &colliderTS3 = reg.emplace<SimpleCollision>(entTS3);
        reg.emplace<CollisionType::Wall>(entTS3);
        colliderTS3.size.x = 100;
        colliderTS3.size.y = 64;
        auto& transformTS3 = reg.emplace<Transform>(entTS3);
        transformTS3.position.x =  150;
        transformTS3.position.y = -110;
        transformTS3.position.z = 0.f;

        auto entTS4 = reg.create();
        auto& spriteTS4 = reg.emplace<Sprite>(entTS4);
        AssignSprite(spriteTS4, "PandemicShop:shelf1");
        auto &colliderTS4 = reg.emplace<SimpleCollision>(entTS4);
        reg.emplace<CollisionType::Wall>(entTS4);
        colliderTS4.size.x = 100;
        colliderTS4.size.y = 64;
        auto& transformTS4 = reg.emplace<Transform>(entTS4);
        transformTS4.position.x = 150;
        transformTS4.position.y = 0;
        transformTS4.position.z = 0.f;

        auto entTS5 = reg.create();
        auto& spriteTS5 = reg.emplace<Sprite>(entTS5);
        AssignSprite(spriteTS5, "PandemicShop:shelf1");
        auto &colliderTS5 = reg.emplace<SimpleCollision>(entTS5);
        reg.emplace<CollisionType::Wall>(entTS5);
        colliderTS5.size.x = 100;
        colliderTS5.size.y = 64;
        auto& transformTS5 = reg.emplace<Transform>(entTS5);
        transformTS5.position.x = 150;
        transformTS5.position.y = 110;
        transformTS5.position.z = 0.f;


//****************************PLANTS*******************************************

        auto entTR1 = reg.create();
        auto& spriteTR1 = reg.emplace<Sprite>(entTR1);
        AssignSprite(spriteTR1, "PandemicShop:tree1");
        auto &colliderTR1 = reg.emplace<SimpleCollision>(entTR1);
        reg.emplace<CollisionType::Wall>(entTR1);
        colliderTR1.size.x = 32;
        colliderTR1.size.y = 32;
        auto& transformTR1 = reg.emplace<Transform>(entTR1);
        transformTR1.position.x = -250;
        transformTR1.position.y =  200;
        transformTR1.position.z = 0.f;

        auto entTR2 = reg.create();
        auto& spriteTR2 = reg.emplace<Sprite>(entTR2);
        AssignSprite(spriteTR2, "PandemicShop:tree1");
        auto &colliderTR2 = reg.emplace<SimpleCollision>(entTR2);
        reg.emplace<CollisionType::Wall>(entTR2);
        colliderTR2.size.x = 32;
        colliderTR2.size.y = 32;
        auto& transformTR2 = reg.emplace<Transform>(entTR2);
        transformTR2.position.x = -250;
        transformTR2.position.y = -160;
        transformTR2.position.z = 0.f;

        auto entTR3 = reg.create();
        auto& spriteTR3 = reg.emplace<Sprite>(entTR3);
        AssignSprite(spriteTR3, "PandemicShop:tree1");
        auto &colliderTR3 = reg.emplace<SimpleCollision>(entTR3);
        reg.emplace<CollisionType::Wall>(entTR3);
        colliderTR3.size.x = 32;
        colliderTR3.size.y = 32;
        auto& transformTR3 = reg.emplace<Transform>(entTR3);
        transformTR3.position.x = 250;
        transformTR3.position.y = -160;
        transformTR3.position.z = 0.f;


        auto entTR4 = reg.create();
        auto& spriteTR4 = reg.emplace<Sprite>(entTR4);
        AssignSprite(spriteTR4, "PandemicShop:tree1");
        auto &colliderTR4 = reg.emplace<SimpleCollision>(entTR4);
        reg.emplace<CollisionType::Wall>(entTR4);
        colliderTR4.size.x = 32;
        colliderTR4.size.y = 32;
        auto& transformTR4 = reg.emplace<Transform>(entTR4);
        transformTR4.position.x = 250;
        transformTR4.position.y = 200;
        transformTR4.position.z = 0.f;

//*****************************OTHER SHELVES*******************************************

        auto entS1 = reg.create();
        auto& spriteS1 = reg.emplace<Sprite>(entS1);
        AssignSprite(spriteS1, "PandemicShop:shelf2");
        auto &colliderS1 = reg.emplace<SimpleCollision>(entS1);
        reg.emplace<CollisionType::Wall>(entS1);
        colliderS1.size.x = 32;
        colliderS1.size.y = 32;
        auto& transformS1 = reg.emplace<Transform>(entS1);
        transformS1.position.x = 0;
        transformS1.position.y = 70;
        transformS1.position.z = 0.f;

        auto entS2 = reg.create();
        auto& spriteS2 = reg.emplace<Sprite>(entS2);
        AssignSprite(spriteS2, "PandemicShop:shelf2");
        auto &colliderS2 = reg.emplace<SimpleCollision>(entS2);
        reg.emplace<CollisionType::Wall>(entS2);
        colliderS2.size.x = 32;
        colliderS2.size.y = 32;
        auto& transformS2 = reg.emplace<Transform>(entS2);
        transformS2.position.x =  0;
        transformS2.position.y = -60;
        transformS2.position.z = 0.f;

//****************************************************************************************
       
        auto ent4 = reg.create();
        auto item4 = reg.emplace<Item>(ent4);
        item4.Create(ent4, "spritesheets:pixel_mart:tuna_can", {1, 1, 1}, {-200, -200});

        // auto ent2 = reg.create();
        // auto item2 = reg.emplace<Item>(ent2);
        // item2.Create(ent2, "spritesheets:pixel_mart:tuna_can", {1, 1, 1}, {128, -128});

        // auto ent3 = reg.create();
        // auto item3 = reg.emplace<Item>(ent3);
        // item3.Create(ent3, "spritesheets:pixel_mart:tuna_can", {1, 1, 1}, {-128, 128});



        // if(item.hidden){
        //     printf("\nhidden\n");
        // }
        // else{

        //     printf("\n not hidden \n");
        // }
    }
    
    

}
