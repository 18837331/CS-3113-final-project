#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"
#include "Map.h"
#include "Util.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "MainMenu.h"

Scene *currentScene;
Scene *sceneList[4];


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
glm::vec3 lifePos, gameOverPos;

GLuint fontTextureID;

int nextLevelLife;
Mix_Music *bgm;

void SwitchToScene(Scene *scene) {
	currentScene = scene;
	currentScene->Initialize();
}


void Initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	displayWindow = SDL_CreateWindow("Up!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context); 
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	bgm = Mix_LoadMUS("bgm.wav");

	Mix_PlayMusic(bgm, -1);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);


	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");


	fontTextureID = Util::LoadTexture("font1.png");

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	glUseProgram(program.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	sceneList[0] = new MainMenu();
	sceneList[1] = new Level1();
	sceneList[2] = new Level2();
	sceneList[3] = new Level3();
	SwitchToScene(sceneList[0]);
	nextLevelLife = 1;

	int total_life = currentScene->state.player.life;

}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				currentScene->state.player.Jump();
				break;
			case SDLK_RETURN:
				if (currentScene->state.nextLevel == -4) {
					SwitchToScene(sceneList[1]);
				}
				break;
			}
		}
	}

	currentScene->state.player.velocity.x = 0;



	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_A])
	{
		currentScene->state.player.velocity.x = -3.0f;
		
	}
	else if (keys[SDL_SCANCODE_D])
	{
		currentScene->state.player.velocity.x = 3.0f;
		
	}
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	deltaTime += accumulator;
	if (deltaTime < FIXED_TIMESTEP) {
		accumulator = deltaTime;
		return;
	}

	while (deltaTime >= FIXED_TIMESTEP) {

		currentScene->Update(FIXED_TIMESTEP);

		deltaTime -= FIXED_TIMESTEP;
	}

	accumulator = deltaTime;


	viewMatrix = glm::mat4(1.0f);

	gameOverPos = glm::vec3(1, -0.5, 0);
	
	if (currentScene->state.player.position.y > -25.5) {
		viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, -currentScene->state.player.position.y, 0));
		gameOverPos = glm::vec3(3, currentScene->state.player.position.y + 0.5, 0);
	}
	else {
		viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 25.5, 0));
		gameOverPos= glm::vec3(3, -25, 0);
	}


	nextLevelLife = currentScene->state.player.life;

}


void Render() {
	glUseProgram(program.programID);
	program.SetViewMatrix(viewMatrix);

	glClear(GL_COLOR_BUFFER_BIT);


	if (currentScene->state.player.life <= 0 ) {
		Util::DrawText(&program, fontTextureID, "You Lose", 1.0f, -0.5f, gameOverPos);
	}

	if (currentScene->state.player.win) {
		Util::DrawText(&program, fontTextureID, "You Win!", 1.0f, -0.5f, gameOverPos);
	}

	if (currentScene->state.nextLevel == -4) {
		Util::DrawText(&program, fontTextureID, "Jump up", 1.0f, -0.5f, glm::vec3(2.5, 2, 0));
		Util::DrawText(&program, fontTextureID, "Press Enter to Play", 1.0f, -0.5f, glm::vec3(0.5, -1, 0));
		Util::DrawText(&program, fontTextureID, "Press 'A' and 'D' to move", 0.8f, -0.5f, glm::vec3(1, -2, 0));
		Util::DrawText(&program, fontTextureID, "Press 'Space' to jump", 0.8f, -0.5f, glm::vec3(1, -3, 0));
	}
	if (currentScene->state.nextLevel == -1 && currentScene->state.player.life > 0) {
		Util::DrawText(&program, fontTextureID, "Next Level", 0.8f, -0.5f, glm::vec3(3, 2, 0));
	}
	if (currentScene->state.nextLevel == -2 && currentScene->state.player.life > 0) {
		Util::DrawText(&program, fontTextureID, "Next Level", 0.8f, -0.5f, glm::vec3(3, 2, 0));
	}
	if (currentScene->state.nextLevel == -3 && currentScene->state.player.life > 0) {

		Util::DrawText(&program, fontTextureID, "GOAL!", 0.8f, -0.4f, glm::vec3(3, 2, 0));
	}

	currentScene->Render(&program);

	SDL_GL_SwapWindow(displayWindow);
} 

void Shutdown() {
	Mix_FreeMusic(bgm);
	Mix_FreeChunk(currentScene->state.player.jump);
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Initialize();

	while (gameIsRunning) {
		if (currentScene->state.nextLevel >= 0) {
			SwitchToScene(sceneList[currentScene->state.nextLevel]);
			currentScene->state.player.life = nextLevelLife;
		}
		if ((!currentScene->state.player.dead) && (!currentScene->state.player.win)) {
			ProcessInput();
		}
		else{
			currentScene->state.player.velocity.x = 0;
		}
		Update();
		Render();
	}

	Shutdown();
	return 0;
}
