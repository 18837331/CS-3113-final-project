#include "Level1.h"
#define LEVEL1_WIDTH 11
#define LEVEL1_HEIGHT 30
unsigned int level1_data[] =
{
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};


void Level1::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("plate.jpg");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 1, 1);
	state.player.entityType = PLAYER;
	state.player.isStatic = false;
	state.player.width = 0.5f;
	state.player.position = glm::vec3(7, -24, 0);
	state.player.acceleration = glm::vec3(0, -9.81f, 0);
	state.player.textureID = Util::LoadTexture("playerx.png");
	state.nextLevel = -1;

	GLuint devilTextureID = Util::LoadTexture("enemy.png");
	state.enemies[0].entityType = ENEMY;
	state.enemies[0].textureID = devilTextureID;
	state.enemies[0].isStatic = false;
	state.enemies[0].position = glm::vec3(4, -28, 0);
	state.enemies[0].acceleration = glm::vec3(0, 0, 0);
	state.enemies[0].aiState = IDLE;


	GLuint tileTextureID = Util::LoadTexture("plate.jpg");
	state.platforms[0].textureID = tileTextureID;
	state.platforms[0].position = glm::vec3(7, -25, 0);


	state.platforms[1].textureID = tileTextureID;
	state.platforms[1].position = glm::vec3(3, -25, 0);


	state.platforms[2].textureID = tileTextureID;
	state.platforms[2].position = glm::vec3(2, -21, 0);

	state.platforms[3].textureID = tileTextureID;
	state.platforms[3].position = glm::vec3(4, -13, 0);

	state.platforms[4].textureID = tileTextureID;
	state.platforms[4].position = glm::vec3(5, -17, 0);


	state.platforms[5].textureID = tileTextureID;
	state.platforms[5].position = glm::vec3(2, -10, 0);

	state.platforms[6].textureID = tileTextureID;
	state.platforms[6].position = glm::vec3(6, -8, 0);

	state.platforms[7].textureID = tileTextureID;
	state.platforms[7].position = glm::vec3(8, -5, 0);

	state.platforms[8].textureID = tileTextureID;
	state.platforms[8].position = glm::vec3(5, -2, 0);

}

void Level1::Update(float deltaTime) {

	state.player.Update(deltaTime, state.player, NULL, 0, state.enemies, ENEMY_COUNT, state.platforms, PLATFORM_COUNT, state.map);
	if (state.player.position.y > 2) {
		state.nextLevel = 2;
	}
	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Update(deltaTime, state.player, NULL, 0, state.enemies, ENEMY_COUNT, state.platforms, PLATFORM_COUNT, state.map);
	}

	for (int i = 0; i < 1; i++) {
		state.platforms[i].Update(deltaTime, state.player, NULL, 0, state.enemies, ENEMY_COUNT, state.platforms, PLATFORM_COUNT, state.map);
	}

}

void Level1::Render(ShaderProgram *program) {
	//state.map->Render(program);

	if (state.player.isActive == true) {
		state.player.Render(program);
	}

	if (state.player.position.y < -27) {
		state.player.life = 0;
		state.player.dead = true;
	}

	for (int i = 0; i < ENEMY_COUNT; i++) {
		if (state.enemies[i].isActive == true) {
			state.enemies[i].Render(program);
		}
	}

	for (int i = 0; i < PLATFORM_COUNT; i++) {
		if (state.platforms[i].isActive == true) {
			state.platforms[i].Render(program);
		}
	}
}