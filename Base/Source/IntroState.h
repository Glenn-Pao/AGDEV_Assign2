#pragma once

#include "gamestate.h"
#include "SceneManager2D_Intro.h"
#include "SceneManager2D.h"
#include "timer.h"
//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>
class CIntroState : public CGameState
{
public:
	void Init();
	void Init(const int width, const int height);
	virtual void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameStateManager* theGSM);
	void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true);
	void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right);
	void Update(CGameStateManager* theGSM);
	void Update(CGameStateManager* theGSM, const double m_dElapsedTime);
	void Draw(CGameStateManager* theGSM);
	short GetStateID() { return stateID; };
	static CIntroState* Instance() {
		return &theIntroState;
	}

protected:
	CIntroState() { }

private:
	static CIntroState theIntroState;
	int counter;
	short stateID = 1;
	//Declare variables to store the last and current mouse position
	double mouse_last_x, mouse_last_y, mouse_diff_x, mouse_diff_y;
	double camera_yaw, camera_pitch;

	GLFWwindow* m_window;
#if TYPE_OF_VIEW == 3
	CSceneManager *scene;	// Use this for 3D gameplay
#else
	CSceneManager2D *scene;	// Use this for 2D gameplay
#endif
	
};
