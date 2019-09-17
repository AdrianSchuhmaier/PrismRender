#include <Prism.h>

class Sandbox : public Prism::Application
{
	void OnEvent(Prism::Event&) override {};
	void OnTick(float delta) override {};
};

PRISM_ENTRY_POINT{
	return new Sandbox();
}