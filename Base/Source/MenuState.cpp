#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "menustate.h"
#include "playstate.h"
#include "LevelSelectState.h"
#include "InstructionState.h"
#include "OptionsState.h"
#include "HighscoreState.h"
#include "Application.h"

CMenuState CMenuState::theMenuState;

void CMenuState::Init()
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Init\n" << endl;
	scene = new CSceneManager2D_Menu(800, 600);
#endif
	scene->Init();
	m_Menu_State = LEVEL_SELECT;
}

void CMenuState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Init\n" << endl;

	scene = new CSceneManager2D_Menu(width, height);
#endif
	scene->Init();
	m_Menu_State = LEVEL_SELECT;
}

void CMenuState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;

	//m_Menu_State = LEVEL_SELECT;
}
void CMenuState::Exit()
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
}
void CMenuState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Pause\n" << endl;
#endif
}

void CMenuState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Resume\n" << endl;
#endif
}
void CMenuState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
#endif
}
void CMenuState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	/*switch (m_Menu_State)
	{
	case LEVEL_SELECT:
		cout << "LEVEL SELECT" << endl;
		break;
	case INSTRUCTIONS:
		cout << "INSTRUCTIONS" << endl;
		break;
	case OPTIONS:
		cout << "OPTIONS" << endl;
		break;
	case HIGHSCORE:
		cout << "HIGHSCORE" << endl;
		break;
	case EXIT:
		cout << "EXIT" << endl;
		break;
	}*/

#endif
	//UpdateMenu(theGSM, dt);
	//Using the down button
	static bool bDownButton = false;
	if (!bDownButton && Application::IsKeyPressed(VK_DOWN))
	{
		bDownButton = true;

		//check its current position
		switch (m_Menu_State)
		{
		case LEVEL_SELECT:
			m_Menu_State = INSTRUCTIONS;
			break;
		case INSTRUCTIONS:
			m_Menu_State = OPTIONS;
			break;
		case OPTIONS:
			m_Menu_State = HIGHSCORE;
			break;
		case HIGHSCORE:
			m_Menu_State = EXIT;
			break;
		case EXIT:
			m_Menu_State = LEVEL_SELECT;
			break;
		}
	}
	else if (bDownButton && !Application::IsKeyPressed(VK_DOWN))
	{
		bDownButton = false;
	}

	//Using the Up button, reverse of down
	static bool bUpButton = false;
	if (!bUpButton && Application::IsKeyPressed(VK_UP))
	{
		bUpButton = true;

		//check its current position
		switch (m_Menu_State)
		{
		case LEVEL_SELECT:
			m_Menu_State = EXIT;
			break;
		case INSTRUCTIONS:
			m_Menu_State = LEVEL_SELECT;
			break;
		case OPTIONS:
			m_Menu_State = INSTRUCTIONS;
			break;
		case HIGHSCORE:
			m_Menu_State = OPTIONS;
			break;
		case EXIT:
			m_Menu_State = HIGHSCORE;
			break;
		}
	}
	else if (bUpButton && !Application::IsKeyPressed(VK_UP))
	{
		bUpButton = false;
	}

	//Using the Enter button
	static bool bEnterButton = false;
	if (!bEnterButton && Application::IsKeyPressed(VK_RETURN))
	{
		bEnterButton = true;

		//when the player decides to do something
		//check its current position
		switch (m_Menu_State)
		{
		case LEVEL_SELECT:
			//theGSM->ChangeState(CPlayState::Instance());	//show the instructions
			theGSM->ChangeState(CLevelSelectState::Instance());	//go to level selection
			break;
		case INSTRUCTIONS:
			theGSM->ChangeState(CInstructionState::Instance());	//show the instructions
			break;
		case OPTIONS:
			theGSM->ChangeState(COptionsState::Instance());	//options screen
			break;
		case HIGHSCORE:
			theGSM->ChangeState(CHighscoreState::Instance());	//highscore screen
			break;
		case EXIT:
			theGSM->Quit();
			break;
		}
	}
	else if (bEnterButton && !Application::IsKeyPressed(VK_RETURN))
	{
		bEnterButton = false;
	}
}

void CMenuState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{
#if GSM_DEBUG_MODE
	int m_iUserChoice = -1;
#endif
}

void CMenuState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Update\n" << endl;
#endif
}

void CMenuState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
}

void CMenuState::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//cout << "CMenuState::Draw\n" << endl;

#endif
	// Render the scene
	scene->Render();

	if (m_Menu_State == LEVEL_SELECT)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Level Select", Color(1, 1, 1), 85, 5, 250);
	}
	else
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Level Select", Color(1, 0, 0), 80, 5, 250);

	if (m_Menu_State == INSTRUCTIONS)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Instructions", Color(1, 1, 1), 75, 5, 200);
	}
	else
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Instructions", Color(1, 0, 0), 70, 5, 200);

	if (m_Menu_State == OPTIONS)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Options", Color(1, 1, 1), 85, 5, 150);
	}
	else
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Options", Color(1, 0, 0), 80, 5, 150);

	if (m_Menu_State == HIGHSCORE)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Highscore", Color(1, 1, 1), 85, 5, 100);
	}
	else
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Highscore", Color(1, 0, 0), 80, 5, 100);

	if (m_Menu_State == EXIT)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Exit", Color(1, 1, 1), 85, 5, 50);
	}
	else
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Exit", Color(1, 0, 0), 80, 5, 50);
}
void CMenuState::SetMenuButtonState(CMenuState::MENU_STATES m_Menu_State)
{
	this->m_Menu_State = m_Menu_State;
}
CMenuState::MENU_STATES CMenuState::GetMenuButtonState(void)
{
	return m_Menu_State;
}
void CMenuState::SetQuit(bool bQuit)
{
	this->bQuit = bQuit;
}
bool CMenuState::GetQuit(void)
{
	return bQuit;
}