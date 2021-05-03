#include "core/engine.h"
#include "gameplay/atonement/atonement_main.h"

int main(int argc_, char** argv_)
{
	dagger::Engine engine;
	return engine.Run<atonement::AtonementGame>();
}
