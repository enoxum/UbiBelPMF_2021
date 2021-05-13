#include "atonement_main.h"

#include "core/core.h"
#include "core/engine.h"
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
#include "gameplay/editor/savegame_system.h"
#include "core/savegame.h"
#include <iostream>
#include <cstring>

#include "gameplay/atonement/char_controller_fsm.h"
#include "gameplay/atonement/atonement_controller.h"
#include "gameplay/atonement/systems/groundedness_detection_system.h"
#include "gameplay/atonement/systems/collision_handler_system.h"
#include "gameplay/atonement/systems/character_collisions.h"
#include "gameplay/atonement/systems/atonement_pause_system.h"
#include "gameplay/atonement/systems/cooldown_manager.h"
#include "gameplay/atonement/systems/atonement_start_menu.h"

using namespace dagger;
using namespace atonement;

struct Character
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    AtonementController::AtonementCharacter& character;
    Transform& transform;
    CharacterCollision& collision;

    static Character Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<AtonementController::AtonementCharacter>(entity);

        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto acs = Engine::GetDefaultResource<AtonementController::AtonementControllerSystem>();
        anim.onAnimationEnded.connect<&CharControllerFSM::OnAnimationEnd>(acs->characterFSM);

        auto& transform = reg.get_or_emplace<Transform>(entity);
        auto& collision = reg.get_or_emplace<CharacterCollision>(entity);

        return Character{ entity, sprite, anim, input, character, transform, collision };
    }

    static Character Create(
        String input_ = "",
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 0, 0 },
        Vector2 collision_size_ = {0, 0})
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();

        ATTACH_TO_FSM(CharControllerFSM, entity);

        auto chr = Character::Get(entity);

        chr.sprite.scale = { 1, 1 };
        chr.sprite.position = { position_, 0.0f };
        chr.sprite.color = { color_, 1.0f };

        chr.collision.size = collision_size_;
        chr.transform.position = { position_, 0.0f };

        AssignSprite(chr.sprite, "BlueWizard:IDLE:idle1");
        AnimatorPlay(chr.animator, "BlueWizard:IDLE");

        if (input_ != "")
            chr.input.contexts.push_back(input_);

        //chr.character.speed = 50;

        return chr;
    }
};

void AtonementGame::CoreSystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<WindowSystem>();
    engine.AddSystem<InputSystem>();
    engine.AddSystem<ShaderSystem>();
    engine.AddSystem<TextureSystem>();
    engine.AddSystem<SpriteRenderSystem>();
    engine.AddPausableSystem<TransformSystem>();
    engine.AddPausableSystem<AnimationSystem>();
#if !defined(NDEBUG)
    engine.AddSystem<DiagnosticSystem>();
    engine.AddSystem<GUISystem>();
    engine.AddSystem<ToolMenuSystem>();
#endif //!defined(NDEBUG)
}

void AtonementGame::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<AtonementStartMenu>();
    engine.AddPausableSystem<SimpleCollisionsSystem>();
    engine.AddPausableSystem<CharacterCollisionsSystem>();
    engine.AddSystem<SaveGameSystem<ECommonSaveArchetype>>(this);
    engine.AddPausableSystem<AtonementController::AtonementControllerSystem>();
    engine.AddPausableSystem<GroundednessDetectionSystem>();
    engine.AddPausableSystem<CollisionHandlerSystem>();
    engine.AddPausableSystem<CooldownManager>();
    engine.AddSystem<AtonementPauseSystem>();


#if defined(DAGGER_DEBUG)
#endif //defined(DAGGER_DEBUG)
}

void atonement::SetUpWorld()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 1920, 1080 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    //trenutno ucitavamo test scenu, TODO: znapraviti prave velike nivoe
    Engine::Dispatcher().trigger<SaveGameSystem<ECommonSaveArchetype>::LoadRequest>(

    
    
    SaveGameSystem<ECommonSaveArchetype>::LoadRequest{ "test_scene.json" });
    auto mainChar = Character::Create("ATON", { 1, 1, 1 }, { -100, -100 }, {50, 130});
    mainChar.sprite.scale = { 0.5, 0.5 };
    //Engine::Registry().emplace<CameraFollowFocus>(mainChar.entity);

    auto& reg = Engine::Registry();
    constexpr int width = 4;
    constexpr int height = 2;
    float tileSize = 300.f;
    constexpr float Space = 0.3f;

    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "start_test");

    sprite.size.x = tileSize;
    sprite.size.y = tileSize/2;

    auto& transform = reg.emplace<Transform>(entity);

        transform.position.x = (1.0f + 1 + 1 * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
        transform.position.y = (1.0f + 2 + 2 * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize/2;
        transform.position.z = 0.f;

        auto& controller = reg.emplace<SelectionMapping>(entity);

    for (int i = 0; i < 2; i++){
        auto entity2 = reg.create();
        auto& sprite2 = reg.emplace<Sprite>(entity2);
        AssignSprite(sprite2, "start_test2");
        
        auto& transform2 = reg.emplace<Transform>(entity2);

        transform2.position.x = (1.0f+1 + 1 * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
        transform2.position.y = (1.0f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize/2;
        transform2.position.z = 1.f;
    }

}

void AtonementGame::WorldSetup(){



     auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    atonement::SetUpWorld();
}



ECommonSaveArchetype AtonementGame::Save(Entity entity_, JSON::json& saveTo_)
{
    auto& registry = Engine::Registry();

    ECommonSaveArchetype archetype = ECommonSaveArchetype::None;

    if (registry.has<Sprite>(entity_))
    {
        saveTo_["sprite"] = SerializeComponent<Sprite>(registry.get<Sprite>(entity_));
        archetype = archetype | ECommonSaveArchetype::Sprite;
    }

    if (registry.has<Transform>(entity_))
    {
        saveTo_["transform"] = SerializeComponent<Transform>(registry.get<Transform>(entity_));
        archetype = archetype | ECommonSaveArchetype::Transform;
    }

    if (registry.has<Animator>(entity_))
    {
        saveTo_["animator"] = SerializeComponent<Animator>(registry.get<Animator>(entity_));
        archetype = archetype | ECommonSaveArchetype::Animator;
    }

    if (registry.has<SimpleCollision>(entity_))
    {
        saveTo_["simple_collision"] = SerializeComponent<SimpleCollision>(registry.get<SimpleCollision>(entity_));
        archetype = archetype | ECommonSaveArchetype::Physics;
    }

    // todo: add new if-block here and don't forget to change archetype

    return archetype;
}

void AtonementGame::Load(ECommonSaveArchetype archetype_, Entity entity_, JSON::json& loadFrom_)
{
    auto& registry = Engine::Registry();

    if (IS_ARCHETYPE_SET(archetype_, ECommonSaveArchetype::Sprite))
        DeserializeComponent<Sprite>(loadFrom_["sprite"], registry.emplace<Sprite>(entity_));

    if (IS_ARCHETYPE_SET(archetype_, ECommonSaveArchetype::Transform))
        DeserializeComponent<Transform>(loadFrom_["transform"], registry.emplace<Transform>(entity_));

    if (IS_ARCHETYPE_SET(archetype_, ECommonSaveArchetype::Animator))
        DeserializeComponent<Animator>(loadFrom_["animator"], registry.emplace<Animator>(entity_));

    if (IS_ARCHETYPE_SET(archetype_, ECommonSaveArchetype::Physics))
        DeserializeComponent<SimpleCollision>(loadFrom_["simple_collision"], registry.emplace<SimpleCollision>(entity_));

    // todo: add new if-block here and don't forget to change archetype
}