#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "menustate.h"
#include "playstate.h"
#include "Application.h"
#include "InstructionState.h"

CInstructionState CInstructionState::theInstructionState;

void CInstructionState::Init()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
	scene = new CSceneManager2D_Instructions(800,600);
#endif
	scene->Init();
}

void CInstructionState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;

	scene = new CSceneManager2D_Instructions(width, height);
#endif
	scene->Init();
	m_Instruction_Button_State = BACK;
	m_Instruction_Page_State = ONE;
}

void CInstructionState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CInstructionState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Pause\n" << endl;
#endif
}

void CInstructionState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Resume\n" << endl;
#endif
}

void CInstructionState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
#endif
}

void CInstructionState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	/*switch (m_Instruction_Button_State)
	{
	case BACK:
		cout << "BACK" << endl;
		break;
	case NEXT:
		cout << "NEXT" << endl;
		break;
	}*/

	cout << dTimer << endl;
#endif
	//Using the down button
	static bool bLeftButton = false;
	if (!bLeftButton && Application::IsKeyPressed(VK_LEFT))
	{
		bLeftButton = true;
		switch (m_Instruction_Button_State)
		{
		case BACK:
			m_Instruction_Button_State = BACK;
			break;
		}
	}
	else if (bLeftButton && !Application::IsKeyPressed(VK_LEFT))
	{
		bLeftButton = false;
	}

	//Using the up button
	static bool bRightButton = false;
	if (!bRightButton && Application::IsKeyPressed(VK_RIGHT))
	{
		bRightButton = true;

		switch (m_Instruction_Button_State)
		{
		case BACK:
			m_Instruction_Button_State = BACK;
			break;
		}
	}
	else if (bRightButton && !Application::IsKeyPressed(VK_RIGHT))
	{
		bRightButton = false;
	}
	
	//Using enter button
	static bool bEnterButton = false;
	if (!bEnterButton && Application::IsKeyPressed(VK_RETURN))
	{
		switch (m_Instruction_Page_State)
		{
		case ONE:
		{
			switch (m_Instruction_Button_State)
			{
			case BACK:
				theGSM->ChangeState(CMenuState::Instance());	//back to main menu
				break;
			}
		}
		}
	}
	else if (bEnterButton && !Application::IsKeyPressed(VK_RETURN))
	{
		bEnterButton = false;
	}
}

void CInstructionState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							   const int button_Left, const int button_Middle, const int button_Right)
{
#if GSM_DEBUG_MODE
#endif
}

void CInstructionState::Update(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Update\n" << endl;
#endif
}

void CInstructionState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
}

void CInstructionState::Draw(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
#endif
	// Render the scene
	scene->Render();
	if (m_Instruction_Page_State == ONE)
	{
		//render 1st page
		scene->RenderInstructions1();

		if (m_Instruction_Button_State == BACK)
		{
			scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Back", Color(1, 1, 1), 85, 5, 0);
		}
		else
			scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Back", Color(1, 0, 0), 80, 5, 0);
	}
}
void CInstructionState::SetInstructionButtonState(CInstructionState::INSTRUCTIONS_BUTTON_STATE m_Instruction_Button_State)
{
	this->m_Instruction_Button_State = m_Instruction_Button_State;
}
CInstructionState::INSTRUCTIONS_BUTTON_STATE CInstructionState::GetInstructionButtonState(void)
{
	return m_Instruction_Button_State;
}

void CInstructionState::SetInstructionPageState(CInstructionState::INSTRUCTION_PAGE_STATE m_Instruction_Page_State)
{
	this->m_Instruction_Page_State = m_Instruction_Page_State;
}
CInstructionState::INSTRUCTION_PAGE_STATE CInstructionState::GetInstructionPageState(void)
{
	return m_Instruction_Page_State;
}