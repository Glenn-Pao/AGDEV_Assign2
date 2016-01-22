#pragma once

#include "gamestate.h"
#include "SceneManager2D_Instructions.h"
#include "SceneManager2D.h"
#include "timer.h"
//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>
class CInstructionState : public CGameState
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

	static CInstructionState* Instance() {
		return &theInstructionState;
	}

	enum INSTRUCTIONS_BUTTON_STATE
	{
		BACK,
		MAIN_MENU,
		MAX_TYPES,
	};
	
	enum INSTRUCTION_PAGE_STATE
	{
		ONE,
		MAX_PAGES,
	};

	void SetInstructionButtonState(CInstructionState::INSTRUCTIONS_BUTTON_STATE);
	CInstructionState::INSTRUCTIONS_BUTTON_STATE GetInstructionButtonState(void);

	void SetInstructionPageState(CInstructionState::INSTRUCTION_PAGE_STATE);
	CInstructionState::INSTRUCTION_PAGE_STATE GetInstructionPageState(void);

protected:
	CInstructionState() {
		m_Instruction_Button_State = BACK;
		m_Instruction_Page_State = ONE;
		bButtonPressed = false;
	}

private:
	static CInstructionState theInstructionState;
	int counter;
	double dTimer;
	bool bButtonPressed;
	short GetStateID() { return stateID; };
	short stateID = 4;
	//Declare variables to store the last and current mouse position
	double mouse_last_x, mouse_last_y, mouse_diff_x, mouse_diff_y;
	double camera_yaw, camera_pitch;

	GLFWwindow* m_window;

	CSceneManager2D *scene;	// Use this for 2D gameplay

	//for menu states
	CInstructionState::INSTRUCTIONS_BUTTON_STATE m_Instruction_Button_State;

	//for swapping between pages
	CInstructionState::INSTRUCTION_PAGE_STATE m_Instruction_Page_State;
};
