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

#include "gameplay/PandemicShop/level.h"
#include "gameplay/PandemicShop/level_system.h"


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

    engine_.AddSystem<LevelSystem>();

#if defined(DAGGER_DEBUG)
    engine_.AddPausableSystem<ping_pong::PingPongTools>();
#endif //defined(DAGGER_DEBUG)
}

void PandemicShopGame::WorldSetup(Engine& engine_)
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    // camera->size = { 800, 600 };
    // camera->zoom = 1;
    // camera->position = { 0, 0, 0 };
    // camera->Update();
    camera->size = { 1280, 720 };
    camera->zoom = 1.5;
    camera->position = { 0, 0, 0 };
    camera->Update();

    SetupWorld(engine_);
}
//---------------------------------------------------------


//--------------------------------------------

void pandemic_shop::SetupWorld(Engine& engine_)
{
    //Vector2 scale(1, 1);

    auto& reg = engine_.Registry();
/*
    // field
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    constexpr int height = 18;
    constexpr int width = 27;
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
                // auto entity = reg.create();
                // auto& sprite = reg.emplace<Sprite>(entity);
                // AssignSprite(sprite, "EmptyWhitePixel");
                // sprite.size = scale * tileSize;

                sprite.color.r = 0.0f;
                sprite.color.g = 0.0f;
                sprite.color.b = 0.0f;

                auto &col = reg.emplace<SimpleCollision>(entity);
                reg.emplace<CollisionType::Wall>(entity);
                col.size.x = tileSize;
                col.size.y = tileSize;

                // auto& transform = reg.emplace<Transform>(entity);
                // transform.position.x = (0.5f + j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
                // transform.position.y = (0.5f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
                // transform.position.z = -1.f;//zPos
                
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
*/
 
  //scene
    {
        // Character::Create("Pandemic", {1, 1, 1}, {64, 0});
        // KarenCharacter::Create("Pandemic", {1, 1, 1}, {0, 0});
        // KarenCharacter::Create("Pandemic", {0.5, 0.5, 0.5}, {128, 0});
        // KarenCharacter::Create("Pandemic", {0.7, 0.7, 0.7}, {-128, 0});


//******************************SHELVES****************************************
/*
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
*/

//****************************PLANTS*******************************************
/*
        auto entTR1 = reg.create();
        auto& spriteTR1 = reg.emplace<Sprite>(entTR1);
        AssignSprite(spriteTR1, "PandemicShop:tree1");
        auto &colliderTR1 = reg.emplace<SimpleCollision>(entTR1);
        reg.emplace<CollisionType::Wall>(entTR1);
        colliderTR1.size.x = 32;
        colliderTR1.size.y = 32;
        auto& transformTR1 = reg.emplace<Transform>(entTR1);
        transformTR1.position.x = -265;
        transformTR1.position.y =  185;
        transformTR1.position.z = 0.f;

        auto entTR2 = reg.create();
        auto& spriteTR2 = reg.emplace<Sprite>(entTR2);
        AssignSprite(spriteTR2, "PandemicShop:plant");
        auto &colliderTR2 = reg.emplace<SimpleCollision>(entTR2);
        reg.emplace<CollisionType::Wall>(entTR2);
        colliderTR2.size.x = 32;
        colliderTR2.size.y = 32;
        auto& transformTR2 = reg.emplace<Transform>(entTR2);
        transformTR2.position.x = -260;
        transformTR2.position.y = -160;
        transformTR2.position.z = 0.f;

        auto entTR3 = reg.create();
        auto& spriteTR3 = reg.emplace<Sprite>(entTR3);
        AssignSprite(spriteTR3, "PandemicShop:plant");
        auto &colliderTR3 = reg.emplace<SimpleCollision>(entTR3);
        reg.emplace<CollisionType::Wall>(entTR3);
        colliderTR3.size.x = 32;
        colliderTR3.size.y = 32;
        auto& transformTR3 = reg.emplace<Transform>(entTR3);
        transformTR3.position.x = 253;
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
        transformTR4.position.x = 260;
        transformTR4.position.y = 185;
        transformTR4.position.z = 0.f;
*/

//*****************************OTHER SHELVES*******************************************
/*
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
*/

//************************TILES***************************************************************
/*
        int y_s = 144;
        for (int j = 0; j < 6; j++)
        {
            for(int i = -245; i <= 253; i+=61){

                auto tileEnt = reg.create();
                auto& tileSprite = reg.emplace<Sprite>(tileEnt);
                AssignSprite(tileSprite, "PandemicShop:tile");
                auto& tileTransform = reg.emplace<Transform>(tileEnt);
                tileTransform.position.x =  i;
                tileTransform.position.y =  y_s - (j*60);
                tileTransform.position.z = 1.f;

           }
        }
*/
//*********************************************BORDERS*********************************************
/*
        //left border
        for (int i = 115; i >=-173; i-=120)
        {
            auto borderEnt = reg.create();
            auto& borderSprite = reg.emplace<Sprite>(borderEnt);
            AssignSprite(borderSprite, "PandemicShop:border");
            // auto &borderCollider = reg.emplace<SimpleCollision>(borderEnt);
            // reg.emplace<CollisionType::Wall>(borderEnt);
            // borderCollider.size.x = 32;
            // borderCollider.size.y = 32;
            auto& borderTransform = reg.emplace<Transform>(borderEnt);
            borderTransform.position.x = -282;
            borderTransform.position.y =  i;
            borderTransform.position.z = 0.f;
        }

        //right border
        for (int i = 115; i >=-173; i-=120)
        {
            auto borderEnt = reg.create();
            auto& borderSprite = reg.emplace<Sprite>(borderEnt);
            AssignSprite(borderSprite, "PandemicShop:border");
            // auto &borderCollider = reg.emplace<SimpleCollision>(borderEnt);
            // reg.emplace<CollisionType::Wall>(borderEnt);
            // borderCollider.size.x = 32;
            // borderCollider.size.y = 32;
            auto& borderTransform = reg.emplace<Transform>(borderEnt);
            borderTransform.position.x = 272;//275
            borderTransform.position.y =  i;
            borderTransform.position.z = 0.f;
        }

        //top border
        for (int i = -230; i <= 220; i+=90)
        {
            auto borderEnt = reg.create();
            auto& borderSprite = reg.emplace<Sprite>(borderEnt);
            AssignSprite(borderSprite, "PandemicShop:borderH");
            // auto &borderCollider = reg.emplace<SimpleCollision>(borderEnt);
            // reg.emplace<CollisionType::Wall>(borderEnt);
            // borderCollider.size.x = 32;
            // borderCollider.size.y = 32;
            auto& borderTransform = reg.emplace<Transform>(borderEnt);
            borderTransform.position.x = i;
            borderTransform.position.y = 180;
            borderTransform.position.z = 0.f;
        }

        //bottom border
        for (int i = -230; i <= 220; i+=90)
        {
            auto borderEnt = reg.create();
            auto& borderSprite = reg.emplace<Sprite>(borderEnt);
            AssignSprite(borderSprite, "PandemicShop:borderH");
            // auto &borderCollider = reg.emplace<SimpleCollision>(borderEnt);
            // reg.emplace<CollisionType::Wall>(borderEnt);
            // borderCollider.size.x = 32;
            // borderCollider.size.y = 32;
            auto& borderTransform = reg.emplace<Transform>(borderEnt);
            borderTransform.position.x = i;
            borderTransform.position.y = -189;
            borderTransform.position.z = 0.f;
        }
*/

//******************************************ITEMS**************************************************
/*
        std::vector<std::string> item {"spritesheets:pixel_mart:tuna_can", "spritesheets:pixel_mart:hand_sanitiser", 
                                        "spritesheets:pixel_mart:sliced_bread_p", "spritesheets:pixel_mart:soap_box",
                                        "spritesheets:pixel_mart:water", "spritesheets:pixel_mart:cereal1",
                                        "spritesheets:pixel_mart:flour", "spritesheets:pixel_mart:cooking_oil"};
       

        int y = -160;
        int c = 0;

        for (int i = 0; i < item.size(); i++)
        {
            if(i%2 == 1){
                c = 20;
            }else{
                c = 0;
            }

            if(i < 4){
                auto ent4 = reg.create();
                auto item4 = reg.emplace<Item>(ent4);
                item4.Create(ent4, item[i], {1, 1, 1}, {-150, y+(100*i)+c});
                Logger::info(item[i]);
            }else{
                auto ent4 = reg.create();
                auto item4 = reg.emplace<Item>(ent4);
                item4.Create(ent4, item[i], {1, 1, 1}, {150, y+(100*(i-4))+c});
                Logger::info(item[i]);
            }
        }
    */    



    }
    
   Level::Load("default");

}
