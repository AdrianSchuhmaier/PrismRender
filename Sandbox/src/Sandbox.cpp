#include <Prism.h>

class Sandbox : public Prism::Application
{

};

PRISM_ENTRY_POINT{
	return new Sandbox();
}