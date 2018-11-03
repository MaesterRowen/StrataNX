#include <strata.h>

class Game : public Strata::Application
{
public:
	Game() {
		TRACE("Welcome!");
	}
};

Strata::Application* Strata::CreateApplication()
{
	return new Game();
}