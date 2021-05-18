#include "gameplay/PandemicShop/ai_system.h"
#include "gameplay/PandemicShop/item.h"
#include "gameplay/PandemicShop/karen.h"

void AISystem::Run(){


    auto &reg = Engine::Registry();
    auto item_view = reg.view<Item>();
    auto karens = reg.view<PandemicKarenCharacter>();
    
    counter += Engine::DeltaTime();
	if (counter >= delay) {
		counter = 0.0f;
        for(auto it_karen:karens){
            auto karen = KarenCharacter::Get(it_karen);
            
            Vector2 karen_pos = {karen.transform.position.x, karen.transform.position.y};

            if(karen.command.finished){
                // Vector2 next_position = {(rand() % border_width), rand() % border_height};
                Vector2 next_position = {-32, -32};
                karen.command.finished = false;
                karen.command.next = next_position;
            }
            auto &prev = karen.command.previous;
            auto &curr = karen.command.current;
            auto &next = karen.command.next;
            
            Vector2 distanceVec = Vector2(next.x - curr.x, next.y - curr.y);
            Vector2 distancePrev = Vector2(next.x - prev.x, next.y - prev.y);
            auto curr_distance = glm::length(distanceVec);
            auto prev_distance = glm::length(distancePrev);

            Logger::info("\nNext position: {} {}", karen.command.next.x, karen.command.next.y);
            Logger::info("\nCurrent position: {} {}", karen.command.current.x, karen.command.current.y);


            Logger::info("\nDISTANCE {}\n", curr_distance);
            if(abs(curr_distance) < 1){
                // prev = curr;
                karen.command.previous = karen.command.current;
                // curr = next;
                karen.command.current = karen.command.next;
                karen.command.curr_action = Action::IDLE;
                karen.command.finished=true;
                continue;
            }

            if(abs(curr_distance) <= abs(prev_distance)){
                Logger::info("\nCURR DIST IS LESS THEN PREV DIST\n");
                if (abs(distanceVec.x) > abs(distanceVec.y)) {
                    Logger::info("\nMOVING HORIZONTALLY\n");
                    if (distanceVec.x < 0) 
                        karen.command.curr_action = Action::LEFT;
                    else 
                        karen.command.curr_action = Action::RIGHT;

                }
                else {
                    Logger::info("\nMOVING VERTICALY\n");
                    if (distanceVec.y < 0) 
                        karen.command.curr_action = Action::UP;
                    else 
                        karen.command.curr_action = Action::DOWN;

                }
            }
            else{
                Logger::info("\nCURR DIST IS BIGGER THEN PREV DIST\n");
                if (abs(distanceVec.x) > abs(distanceVec.y)) {
                    if (distanceVec.x < 0) 
                        karen.command.curr_action = Action::RIGHT;
                    else 
                        karen.command.curr_action = Action::LEFT;

                }
                else {
                    if (distanceVec.y < 0) 
                        karen.command.curr_action = Action::DOWN;
                    else 
                        karen.command.curr_action = Action::UP;

                }
            }

        }
    }
    
}

// void AISystem::Run()
// {
// 	counter += Engine::DeltaTime();
// 	if (counter >= delay) {
// 		counter = 0.0f;
// 			auto viewKarens = Engine::Registry().view<PandemicKarenCharacter,Transform,Sprite, KarenAI>();
// 			// auto viewHero = Engine::Registry().view<TeamGameCharacter, Transform>();
// 			// auto hero = viewHero.begin();
// 			// auto& heroTransform = viewHero.get<Transform>(*hero);
			
// 			for (auto karen : viewKarens)
// 			{
// 				auto& karenAI = viewKarens.get<KarenAI>(karen);
// 				auto& karenTransform = viewKarens.get<Transform>(karen);
// 				auto& karenSprite = viewKarens.get<Sprite>(karen);

//                 Vector2 next_position = {33, 32};

// 				Vector2 distanceVec = Vector2(karenTransform.position.x - next_position.x, karenTransform.position.y - next_position.y);
// 				if (karenAI.previous.size() > depth)karenAI.previous.pop_front();
// 				karenAI.previous.push_back(karenAI.current);
// 				KarenAI newCommand;
// 			// 	// newCommand.attack = false;
// 			// 	// if (!slimeAi.alive) {
// 			// 	// 	newCommand.move = STAY;
// 			// 	// 	continue;
// 			// 	// }
// 				float distance = glm::length(distanceVec);
//                 // Logger::info("\nDistance {}\n", distance);
//                 Logger::info("\nDistanceVector {} {}\n", distanceVec.x, distanceVec.y);
//                 Logger::info("Karen position {} {}\n", karenTransform.position.x, karenTransform.position.y);

// 				if (distance <= karenSprite.size.x * 13) {
// 					if (distance <= karenSprite.size.x * 1.5) {
// 			// 			// if (distance <= karenSprite.size.x * 0.55) {
// 			// 			// 	newCommand.attack = true;
// 			// 			// }

// 						if (abs(distanceVec.x) > abs(distanceVec.y)) {
// 							if (distanceVec.x > 0) newCommand.current.curr_action = Action::RIGHT;
// 							else newCommand.current.curr_action = Action::LEFT;
// 						}
// 						else {
// 							if (distanceVec.y > 0) newCommand.current.curr_action = Action::DOWN;
// 							else newCommand.current.curr_action = Action::UP;

// 						}
// 					}
// 					else {
// 						if (rand() % 10 < 3) {

// 							newCommand.current.curr_action = Action((rand() % 4) + 1);

// 						}
// 					// 	else {
// 					// 		SInt16 previous[5] = { 0,0,0,0,0 };
// 					// 		for (size_t i = 0; i < karenAI.previous.size(); i++)
//                     //             previous[(int)karenAI.previous.at(i).curr_action]++;
// 					// 		SInt16 moveMax = 0;
// 					// 		SInt16 maxMoved = previous[0];
// 					// 		for (SInt16 i = 0; i < 5; i++) {
// 					// 			if (maxMoved < previous[i]) {
// 					// 				maxMoved = previous[i];
// 					// 				moveMax = i;
// 					// 			}
// 					// 		}
// 					// 		newCommand.current.curr_action = Action(moveMax);

// 					// 	}
// 					}
// 					karenAI = newCommand;

// 				}
// 				else {
// 					newCommand.current.curr_action = Action::IDLE;
// 				}
// 			}
	
			
// 	}
	
// }






