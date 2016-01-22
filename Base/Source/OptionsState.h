#pragma once

#include "gamestate.h"
#include "SceneManager2D_Options.h"
#include "SceneManager2D.h"
#include "timer.h"
//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>
class COptionsState : public CGameState
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

	static COptionsState* Instance() {
		return &theOptionsState;
	}

	enum OPTIONS_BUTTON_STATE
	{
		MOUNTAIN_DEW,
		NORMAL,
		BACK,
		MAX_TYPES,
	};

	bool dew;	//mountain dew mode

	void SetOptionsButtonState(COptionsState::OPTIONS_BUTTON_STATE);
	COptionsState::OPTIONS_BUTTON_STATE GetOptionsButtonState(void);

protected:
	COptionsState() {
		m_Options_Button_State = BACK;
	}

private:
	static COptionsState theOptionsState;
	int counter;
	short stateID = 5;
	//Declare variables to store the last and current mouse position
	double mouse_last_x, mouse_last_y, mouse_diff_x, mouse_diff_y;
	double camera_yaw, camera_pitch;

	GLFWwindow* m_window;

	CSceneManager2D *scene;	// Use this for 2D gameplay

	//for menu states
	COptionsState::OPTIONS_BUTTON_STATE m_Options_Button_State;
};
