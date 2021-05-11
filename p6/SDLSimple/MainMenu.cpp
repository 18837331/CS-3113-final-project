#include "MainMenu.h"
#define MAINMENU_WIDTH 10
#define MAINMENU_HEIGHT 8
unsigned int mainmenu_data[] =
{
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void MainMenu::Initialize() {
	state.nextLevel = -4;
}
void MainMenu::Update(float deltaTime) {}
void MainMenu::Render(ShaderProgram* program) {
	//state.map->Render(program);
}