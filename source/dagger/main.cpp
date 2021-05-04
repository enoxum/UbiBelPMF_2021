
#include "core/engine.h"
#include "gameplay/PandemicShop/pandemic_shop_main.h"

int main(int argc_, char** argv_)
{
	dagger::Engine engine;
	// return engine.Run<tiles_example::TilesExampleMain>();
	//	return engine.Run<team_game::TeamGame>();
	//return engine.Run<ping_pong::PingPongGame>();
	//	return engine.Run<racing_game::RacingGame>();
	//return engine.Run<platformer::Platformer>();
	return engine.Run<pandemic_shop::PandemicShopGame>();
}