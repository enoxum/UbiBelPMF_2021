#include "fightEnemy.h"

#include "gameplay/roboship/selectTileController.h"
#include <gameplay/roboship/roboship_player_move.h>
#include <gameplay/roboship/roboship_main.h>

using namespace robo_game;
using namespace inventory;
using namespace roboship;

static int numOfComb = 0;

void fightEnemy::findCombination(std::vector<int> comb)
{
    auto entity = Engine::Registry().view<InventoryMatrix>()[0];
    auto& matrix = Engine::Registry().get<InventoryMatrix>(entity);

    int start_i;
    int start_j;
    bool found = false;
    std::pair<int, int> end;

    auto len = comb.size();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (matrix.matrix[i][j] == comb[0] && i+len<=4 && !found)
            {
                start_i = i;
                start_j = j;
                end = checkNeighborRow(i+1, j , 1, len, comb);
                if (end.first != -1)
                {
                    found = true;
                    numOfComb++;
                    changeTiles(start_i, start_j, end.first, end.second, 1);
                    return;
                }     
            }

        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (matrix.matrix[i][j] == comb[0] && j + len <= 4 && !found)
            {
                start_i = i;
                start_j = j;
                end = checkNeighborCol(i, j + 1, 1, len, comb);
                if (end.first != -1)
                {
                    numOfComb++;
                    found = true;
                    changeTiles(start_i, start_j, end.first, end.second, 2);
                    return;
                }

            }

        }
    }

}

std::pair<int, int> fightEnemy::checkNeighborRow(int i, int j, int k, int len, std::vector<int> comb)
{
    auto entity = Engine::Registry().view<InventoryMatrix>()[0];
    auto& matrix = Engine::Registry().get<InventoryMatrix>(entity);


    if (matrix.matrix[i][j] == comb[k])
    {
        if (k == len - 1)
            return std::make_pair(i, j);
        else
        {
            return checkNeighborRow(i+1, j, k+1, len, comb);
        }
    }
    else 
        return std::make_pair(-1, -1);


    return std::pair<int, int>();
}

std::pair<int, int> fightEnemy::checkNeighborCol(int i, int j, int k, int len, std::vector<int> comb)
{
    auto entity = Engine::Registry().view<InventoryMatrix>()[0];
    auto& matrix = Engine::Registry().get<InventoryMatrix>(entity);


    if (matrix.matrix[i][j] == comb[k])
    {
        if (k == len - 1)
            return std::make_pair(i, j);
        else
        {
            return checkNeighborCol(i, j + 1, k + 1, len, comb);
        }
    }
    else
        return std::make_pair(-1, -1);


    return std::pair<int, int>();
}
void fightEnemy::destroyTile()
{

}

void fightEnemy::changeTiles(int a, int b, int c, int d, int k)
{
    if (c == -1)
        return;
    if (d == -1)
        return;

    if (numOfComb <= 3) {
        addShipPart();
    }

    Engine::Dispatcher().trigger<FightEnded>();

    //ovde poslati signal
    auto entity = Engine::Registry().view<InventoryMatrix>()[0];
    auto& matrix = Engine::Registry().get<InventoryMatrix>(entity);
    int height = 4;
    int width = 4;
    float tileSize = 60.f;
    float Space = 0.3f;

    auto view2 = Engine::Registry().view<Sprite, RoboshipPlayer>();
    Sprite pos;

    for (auto e : view2)
    {
        pos = Engine::Registry().get<Sprite>(e);
    }


    auto info = Engine::Registry().view<RFightModeOn>()[0];

    auto& fightMode = Engine::Registry().get<RFightModeOn>(info);
    fightMode.found = true;
   
    auto view = Engine::Registry().view<Transform, Sprite>();
    if(k == 1)
    { 
        for (int i = a; i < c; i++)
        {
            
            matrix.matrix[i][b] = 1 + rand() % 6;
            float posI = (-1.0f + i + i * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize + pos.position.x;
            float posB = (2.5f + b + b * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;


            for (auto entity : view)
            {
                if (Engine::Registry().has<ControllerMapping>(entity))
                    continue;
         
                auto& t = view.get<Transform>(entity);
                auto& s = view.get<Sprite>(entity);

                if (abs(t.position.x - posI) < 10 && abs(t.position.y - posB) < 10 && s.size.x == tileSize/2)
                {
                    AssignSprite(s, fmt::format("robot:INVENTORY:part_{}", matrix.matrix[i][b]));
                    s.size.x = tileSize/2;
                    s.size.y = tileSize/2;
                }

            }
        }
    }

    if (k == 2)
    {
        for (int i = b; i < d; i++)
        {
            matrix.matrix[a][i] = 1 + rand() % 6;
            float posA = (-1.0f + a + a * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize + pos.position.x;
            float posI = (2.5f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;


            for (auto entity : view)
            {
                if (Engine::Registry().has<ControllerMapping>(entity))
                    continue;

                auto& t = view.get<Transform>(entity);
                auto& s = view.get<Sprite>(entity);

                if (abs(t.position.x - posA) < 10 && abs(t.position.y - posI) < 10 && s.size.x == tileSize/2)
                {
                    AssignSprite(s, fmt::format("robot:INVENTORY:part_{}", matrix.matrix[a][i]));
                    s.size.x = tileSize/2;
                    s.size.y = tileSize/2;
                }

            }
        }
    }


}

void fightEnemy::addShipPart() 
{
    int height = 4;
    int width = 4;
    float tileSize = 60.f;
    float Space = 0.3f;

    auto entity = Engine::Registry().view<EmptySprite>()[0];

    auto& t = Engine::Registry().get<Transform>(entity);
    auto& s = Engine::Registry().get<Sprite>(entity);


    auto entityNew = Engine::Registry().create();
    auto& sprite = Engine::Registry().emplace<Sprite>(entityNew);

    AssignSprite(sprite, "robot:INVENTORY:spaceshipPart_5");

    sprite.size.x = tileSize/2;
    sprite.size.y = tileSize/2;

    auto& transform = Engine::Registry().emplace<Transform>(entityNew);
    transform.position.x = t.position.x;
    transform.position.y = t.position.y;
    transform.position.z = 1.f;

    Engine::Registry().emplace<MoveWithRobot>(entityNew);


    Engine::Registry().remove<EmptySprite>(entity);


}
