#include "editor_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "gameplay/common/simple_collisions.h"
#include "tools/diagnostics.h"

#include "core/savegame.h"
#include "gameplay/editor/savegame_system.h"
#include <iostream>
#include <cstring>

using namespace dagger;
using namespace editor;

ECommonSaveArchetype EditorTestGame::Save(Entity entity_, JSON::json& saveTo_)
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

void EditorTestGame::Load(ECommonSaveArchetype archetype_, Entity entity_, JSON::json& loadFrom_)
{
    auto& registry = Engine::Registry();

    if (IS_ARCHETYPE_SET(archetype_, ECommonSaveArchetype::Camera))
        DeserializeComponent<Camera>(loadFrom_["camera"], *Engine::GetDefaultResource<Camera>());

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

void EditorTestGame::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();
    engine.AddSystem<EditorToolSystem>();
    engine.AddSystem<SaveGameSystem<ECommonSaveArchetype>>(this);
}

void EditorTestGame::WorldSetup()
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::ShowAsMuchAsPossible;
    camera->position = { 0, 0, 0 };
    camera->Update();
}

void EditorToolSystem::OnToolMenuRender()
{
    if (ImGui::BeginMenu("Scene"))
    {
        if (ImGui::MenuItem("Clear"))
        {
            Engine::Registry().clear();
        }

        if (ImGui::MenuItem("Save"))
        {
            Engine::Dispatcher().trigger<SaveGameSystem<ECommonSaveArchetype>::SaveRequest>(
                SaveGameSystem<ECommonSaveArchetype>::SaveRequest{ m_Filename });
        }

        if (ImGui::MenuItem("Load"))
        {
            Engine::Dispatcher().trigger<SaveGameSystem<ECommonSaveArchetype>::LoadRequest>(
                SaveGameSystem<ECommonSaveArchetype>::LoadRequest{ m_Filename });
        }

        ImGui::EndMenu();
    }
}

void EditorToolSystem::OnKeyboardEvent(KeyboardEvent event_)
{
    if (event_.key == EDaggerKeyboard::KeyTab && event_.action == EDaggerInputState::Released)
        if (Input::IsInputDown(EDaggerKeyboard::KeyLeftControl))
            m_IsInEditor = !m_IsInEditor;
}

void EditorToolSystem::Run()
{
    if (m_IsInEditor)
    {
        auto& knob = m_Registry.get<Sprite>(m_Focus);
        knob.color.a = 1;
        auto& focus = m_Registry.get<EditorFocus>(m_Focus);

        if (Input::IsInputDown(EDaggerMouse::MouseButton3))
        {
            auto camera = Engine::GetDefaultResource<Camera>();
            auto cam = camera->position;
            auto cur = Input::CursorPositionInWorld();
            Vector3 xy{ (cur.x - cam.x) * camera->zoom, (cur.y + cam.y) * camera->zoom, 0 };
            knob.position = xy;
            focus.dirty = true;
        }

        if (Input::IsInputDown(EDaggerKeyboard::KeyLeftShift))
        {
            auto& reg = Engine::Registry();
            if (reg.valid(m_Selected.entity))
            {
                if (reg.has<Transform>(m_Selected.entity))
                {
                    auto& transform = reg.get<Transform>(m_Selected.entity);
                    knob.position = Vector3{ Input::CursorPositionInWorld(), 0 };
                    transform.position = knob.position;
                }
                else if (reg.has<Sprite>(m_Selected.entity))
                {
                    auto& sprite = reg.get<Sprite>(m_Selected.entity);
                    knob.position = Vector3{ Input::CursorPositionInWorld(), 0 };
                    sprite.position = knob.position;
                }
            }
        }

        if (!Input::IsInputDown(EDaggerMouse::MouseButton1) && focus.dirty)
        { 
            m_Targets.clear();
            focus.dirty = false;
            const auto* cam = Engine::GetDefaultResource<Camera>();

            const auto knobx = (knob.position.x + cam->position.x);
            const auto knoby = (knob.position.y + cam->position.y);

            Engine::Registry().view<Sprite, SaveGame<ECommonSaveArchetype>>()
                .each([&](Entity entity_, const Sprite& sprite_, const SaveGame<ECommonSaveArchetype>& save_)
                    {
                        const auto left = sprite_.position.x - (sprite_.size.x / 2) * sprite_.scale.x * cam->zoom;
                        const auto top = sprite_.position.y - (sprite_.size.y / 2) * sprite_.scale.y * cam->zoom;

                        const auto right = sprite_.position.x + (sprite_.size.x / 2) * sprite_.scale.x * cam->zoom;
                        const auto bottom = sprite_.position.y + (sprite_.size.y / 2) * sprite_.scale.y * cam->zoom;

                        if (knobx >= left && knoby >= top && knobx <= right && knoby <= bottom)
                        {
                            m_Targets.push_back(EditorFocusTarget{ entity_, sprite_.image->Name() });
                        }
                    });
        }
    }
    else
    {
        auto& knob = m_Registry.get<Sprite>(m_Focus);
        knob.color.a = 0;
    }
}

void EditorToolSystem::GUIExecuteCreateEntity()
{
    auto& reg = Engine::Registry();
    auto newEntity = reg.create();
    auto& newSprite = reg.emplace<Sprite>(newEntity);
    AssignSprite(newSprite, "tools:knob2");
    reg.emplace<Transform>(newEntity);
    newSprite.UseAsUI();
    auto& newSavegame = reg.emplace<SaveGame<ECommonSaveArchetype>>(newEntity);
}

void EditorToolSystem::GUIDrawSpriteEditor()
{
    static String filter;

    auto& reg = Engine::Registry();

    if (reg.has<Sprite>(m_Selected.entity) && ImGui::CollapsingHeader("Sprite"))
    {
        ImGui::InputText("Filter", filter.data(), 80);

        Sprite& compSprite = reg.get<Sprite>(m_Selected.entity);

        /* Sprite texture */ {
            static int selectedTexture = 0;
            Sequence<const char*> textures;
            int i = 0;
            int currentSelected = 0;
            for (auto& [k, n] : Engine::Res<Texture>())
            {
                if (strstr(k.data(), filter.data()) != nullptr)
                    textures.push_back(k.c_str());

                if (k == compSprite.image->Name())
                {
                    selectedTexture = i;
                }
                ++i;
            }

            currentSelected = selectedTexture;
            String title{};
            title.reserve(100);
            sprintf(title.data(), "Image (%d)", textures.size());
            if (ImGui::Combo(title.data(), &selectedTexture, textures.data(), textures.size()))
            {
                if (currentSelected != selectedTexture)
                {
                    AssignSprite(compSprite, textures[selectedTexture]);
                }
            }
        }

        /* Size values */ {
            float size[]{ compSprite.size.x, compSprite.size.y };
            ImGui::SliderFloat2("Pixel Size", size, -10000.0f, 10000.0f, "%f", 1);
            compSprite.size.x = size[0];
            compSprite.size.y = size[1];
        }

        /* Position values */ {
            float pos[]{ compSprite.position.x, compSprite.position.y, compSprite.position.z };
            ImGui::InputFloat3("Sprite Position", pos, "%f", 1);
            compSprite.position.x = pos[0];
            compSprite.position.y = pos[1];
            compSprite.position.z = pos[2];
        }

        /* Rotation value */ {
            ImGui::SliderFloat("Sprite Rotation", &compSprite.rotation, 0, 360, "%f", 1);
        }

        /* Scale values */ {
            float size[]{ compSprite.scale.x, compSprite.scale.y };
            ImGui::DragFloat2("Sprite Scale", size, 1, -10, 10, "%f", 1);
            compSprite.scale.x = size[0];
            compSprite.scale.y = size[1];
        }

        /* Pivot values */ {
            float pivot[]{ compSprite.pivot.x, compSprite.pivot.y };
            ImGui::DragFloat2("Sprite Pivot", pivot, 1, -0.5f, 0.5f, "%f", 1);
            compSprite.pivot.x = pivot[0];
            compSprite.pivot.y = pivot[1];
        }
    }
    else if (!reg.has<Sprite>(m_Selected.entity))
    {
        if (ImGui::Button("Attach Sprite"))
        {
            reg.emplace<Sprite>(m_Selected.entity);
        }
    }
}

void EditorToolSystem::GUIDrawTransformEditor()
{
    auto& reg = Engine::Registry();

    if (reg.has<Transform>(m_Selected.entity) && ImGui::CollapsingHeader("Transform"))
    {
        Transform& compTransform = reg.get<Transform>(m_Selected.entity);
        /* Transform position value */ {
            float pos[]{ compTransform.position.x, compTransform.position.y, compTransform.position.z };
            ImGui::InputFloat3("Transform Position", pos, "%f", 1);
            compTransform.position.x = pos[0];
            compTransform.position.y = pos[1];
            compTransform.position.z = pos[2];
        }
    }
    else if (!reg.has<Transform>(m_Selected.entity))
    {
        if (ImGui::Button("Attach Transform"))
        {
            reg.emplace<Transform>(m_Selected.entity);
        }
    }
}

void EditorToolSystem::GUIDrawAnimationEditor()
{
    auto& reg = Engine::Registry();

    if (reg.has<Animator>(m_Selected.entity) && ImGui::CollapsingHeader("Animator"))
    {
        Animator& compAnim = reg.get<Animator>(m_Selected.entity);
        /* Animation */ {
            static int selectedAnim = 0;
            Sequence<const char*> animations;
            int i = 0;
            int currentSelected = 0;
            for (auto& [k, n] : Engine::Res<Animation>())
            {
                animations.push_back(k.c_str());
                if (k == compAnim.currentAnimation)
                {
                    selectedAnim = i;
                }
                ++i;
            }

            currentSelected = selectedAnim;
            if (ImGui::Combo("Animation", &selectedAnim, animations.data(), animations.size()))
            {
                if (currentSelected != selectedAnim)
                {
                    AnimatorPlay(compAnim, animations[selectedAnim]);
                }
            }
        }

        /* Is playing? */ {
            ImGui::Checkbox("Is playing?", &compAnim.animationPlaying);
        }
    }
    else if (!reg.has<Animator>(m_Selected.entity))
    {
        if (ImGui::Button("Attach Animator"))
        {
            reg.emplace<Animator>(m_Selected.entity);
        }
    }
}

void EditorToolSystem::GUIDrawPhysicsEditor()
{
    auto& reg = Engine::Registry();

    if (reg.has<SimpleCollision>(m_Selected.entity) && ImGui::CollapsingHeader("Collision"))
    {
        SimpleCollision& compCol = reg.get<SimpleCollision>(m_Selected.entity);

        /* Pivot values */ {
            float pivot[]{ compCol.pivot.x, compCol.pivot.y };
            ImGui::DragFloat2("Collision Pivot", pivot, 1, -0.5f, 0.5f, "%f", 1);
            compCol.pivot.x = pivot[0];
            compCol.pivot.y = pivot[1];
        }

        /* Size values */ {
            float size[]{ compCol.size.x, compCol.size.y };
            ImGui::DragFloat2("Collision Size", size, 1, -0.5f, 0.5f, "%f", 1);
            compCol.size.x = size[0];
            compCol.size.y = size[1];
        }
    }
    else if (!reg.has<SimpleCollision>(m_Selected.entity))
    {
        if (ImGui::Button("Attach Collision"))
        {
            reg.emplace<SimpleCollision>(m_Selected.entity);
        }
    }
}

bool EditorToolSystem::GUIDrawEntityFocusSelection(int& selectedItem)
{
    auto& reg = Engine::Registry();

    Sequence<const char*> items;

    items.push_back("<none>");
    for (auto& item : m_Targets)
    {
        items.push_back(item.name.c_str());
    }

    ImGui::ListBox("In Focus", &selectedItem, items.data(), items.size(), 10);

    if (selectedItem - 1 >= m_Targets.size())
    {
        selectedItem = 0;
        m_Selected.entity = entt::null;
    }

    if (selectedItem > 0)
    {
        const int index = selectedItem - 1;
        m_Selected = m_Targets[index];

        if (!reg.valid(m_Selected.entity))
        {
            return false;
        }

        return true;
    }

    return false;
}

void EditorToolSystem::GUIDrawCameraEditor()
{
    auto& reg = Engine::Registry();
    auto* camera = Engine::GetDefaultResource<Camera>();
    static int mode = 0;
    static const char* modes[]{ "Fixed Resoulution", "Show As Much As Possible" };

    if (ImGui::CollapsingHeader("Camera"))
    {
        ImGui::Combo("Mode", &mode, modes, 2);

        switch (mode)
        {
        case 0:
            camera->mode = ECameraMode::FixedResolution;
            break;
        case 1:
            camera->mode = ECameraMode::ShowAsMuchAsPossible;
            break;
        }

        /* Position values */ {
            float position[]{ camera->position.x, camera->position.y };
            ImGui::DragFloat2("Camera Position", position);
            camera->position.x = position[0];
            camera->position.y = position[1];
        }
         
        /* Zoom value */ {
            ImGui::DragFloat("Zoom", &camera->zoom, 0.1f, 0.01f, 10.0f, "%f", 1);
        }
    }

}

void EditorToolSystem::OnRenderGUI()
{
    if (m_IsInEditor)
    {
        auto& reg = Engine::Registry();

        static int selectedItem = 0;

        ImGui::Begin("Scene Editor");

        ImGui::InputText("Filename", m_Filename.data(), m_Filename.length());
        ImGui::Separator();

        if (ImGui::Button("Create entity"))
        {
            GUIExecuteCreateEntity();
            m_Registry.get<EditorFocus>(m_Focus).dirty = true;
        }

        GUIDrawCameraEditor();

        if (GUIDrawEntityFocusSelection(selectedItem))
        {
            GUIDrawSpriteEditor();
            GUIDrawTransformEditor();
            GUIDrawAnimationEditor();
            GUIDrawPhysicsEditor();

            // to add more components, replicate the above functions carefully
            // if you're lost, ping Mika on discord :)
            // ps. DON'T forget to make your components serializable by replicating the following:
            //  - de/serialiazation in savegame.h
            //  - changing the savegame enum and editor GUI in editor_main.h/cpp
        }

        ImGui::End();
    }
}