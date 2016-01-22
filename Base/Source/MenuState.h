#pragma once

#include "gamestate.h"
#include "SceneManager2D.h"
#include "SceneManager2D_Menu.h"

class CMenuState : public CGameState
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

	void Exit();
	short GetStateID() { return stateID; };
	
	static CMenuState* Instance() {
		return &theMenuState;
	}

	enum MENU_STATES
	{
		LEVEL_SELECT = 0,
		INSTRUCTIONS,
		OPTIONS,
		HIGHSCORE,
		EXIT,
		MAX_STATES,
	};

	void SetMenuButtonState(CMenuState::MENU_STATES);
	CMenuState::MENU_STATES GetMenuButtonState(void);

	void SetQuit(bool);
	bool GetQuit(void);

	//void UpdateMenu(CGameStateManager* theGSM, double &dt);

protected:
	CMenuState() { bQuit = false; }

private:
	static CMenuState theMenuState;
	CSceneManager2D *scene;
	short stateID = 2;
	//for menu states
	CMenuState::MENU_STATES m_Menu_State;

	//the flag for closing the application
	bool bQuit;
};
