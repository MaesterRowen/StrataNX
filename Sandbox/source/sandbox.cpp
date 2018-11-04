#include <strata.h>

class Game : public Strata::Application
{
public:
	float x;
	Game() {
		TRACE("Welcome!");
	}

	void Update(float deltaTime) override
	{
		//TRACE("Delta Time:  %4.4f\n", deltaTime);
	}
};

Strata::Application* Strata::CreateApplication()
{
	return new Game();
}