#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "Vector3.h"

void CGameStateManager::Init(const char* title, int width, int height, 
						 int bpp, bool fullscreen)
{
	m_window_width	= width;
	m_window_height	= height;

	m_bFullscreen = fullscreen;
	m_bRunning = true;

	//MainMenu = NULL;
	//create the new sound system
	//TheSound = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_MULTI_THREADED | ESEO_LOAD_PLUGINS | ESEO_USE_3D_BUFFERS);

#if GSM_DEBUG_MODE
	cout << "CGameStateManager::Init\n" << endl;
#endif
}

void CGameStateManager::Cleanup()
{
	// cleanup the all states
	while (!StackOfStates.empty()) {
		StackOfStates.back()->Cleanup();
		StackOfStates.pop_back();
		//drop the sound system when exiting

	}

	// switch back to windowed mode so other 
	// programs won't get accidentally resized
	if (m_bFullscreen) {
	}
	//TheSound->drop();

	//remove the sound from the system
	//if (MainMenu)
	//{
	//	//MainMenu->stop();
	//	delete MainMenu;
	//	MainMenu = NULL;
	//}
	
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::Cleanup\n" << endl;
#endif
}

void CGameStateManager::ChangeState(CGameState* state) 
{
	// cleanup the current state
	if ( !StackOfStates.empty() ) {
		StackOfStates.back()->Cleanup();
		StackOfStates.pop_back();
	}

	// store and init the new state
	StackOfStates.push_back(state);
	StackOfStates.back()->Init(m_window_width, m_window_height);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::ChangeState\n" << endl;
#endif
}

void CGameStateManager::PushState(CGameState* state)
{
	// pause current state
	if ( !StackOfStates.empty() ) {
		StackOfStates.back()->Pause();
	}

	// store and init the new state
	StackOfStates.push_back(state);
	StackOfStates.back()->Init(m_window_width, m_window_height);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::PushState\n" << endl;
#endif
}

void CGameStateManager::PopState()
{
	// cleanup the current state
	if ( !StackOfStates.empty() ) {
		StackOfStates.back()->Cleanup();
		StackOfStates.pop_back();
	}

	// resume previous state
	if ( !StackOfStates.empty() ) {
		StackOfStates.back()->Resume();
	}
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::PopState\n" << endl;
#endif
}

void CGameStateManager::HandleEvents() 
{
	// let the state handle events
	StackOfStates.back()->HandleEvents(this);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::HandleEvents\n" << endl;
#endif
}

void CGameStateManager::HandleEvents(const unsigned char key, const bool status)
{
	// let the state handle events
	StackOfStates.back()->HandleEvents(this, key, status);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::HandleEvents\n" << endl;
#endif
}

void CGameStateManager::HandleEvents(const double mouse_x, const double mouse_y,
									 const int button_Left, const int button_Middle, const int button_Right)
{
	// let the state handle events
	StackOfStates.back()->HandleEvents(this, mouse_x, mouse_y, button_Left, button_Middle, button_Right);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::HandleEvents\n" << endl;
#endif

}
void CGameStateManager::UpdateSound(const double m_dElapsedTime)
{
	short i = StackOfStates.back()->GetStateID();
	//cout << i << endl;
	//load the main menu background sound
	//if (MainMenu == NULL)
	//{
	//	//MainMenu = TheSound->play2D("../irrKlang/media/Unravel.ogg", true, true);
	//	MainMenu = TheSound->play2D("../irrKlang/media/Kisetsu wa Tsugitsugi Shindeiku.ogg", true, true);
	//}
	//switch (i)
	//{
	//case 7:
	//{
	//	//if the sound of main menu is still playing
	//	if (MainMenu != NULL)
	//	{
	//		MainMenu->stop();
	//	}
	//}
	//default:
	//{
	//	if (MainMenu != NULL)
	//	{
	//		if (MainMenu->getIsPaused())
	//		{
	//			MainMenu->setIsPaused(false);
	//		}
	//		if (MainMenu->isFinished())
	//		{
	//			MainMenu = NULL;
	//		}
	//	}
	//}
	//}
}
void CGameStateManager::Update(const double m_dElapsedTime) 
{
	// let the state update the theGSM
	StackOfStates.back()->Update(this, m_dElapsedTime);

	UpdateSound(m_dElapsedTime);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::Update\n" << endl;
#endif
}

void CGameStateManager::Draw() 
{
	// let the state draw the screen
	StackOfStates.back()->Draw(this);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::Draw\n" << endl;
#endif
}
void CGameStateManager::Test()
{
	//StackOfStates.back()->Draw(this);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::Draw\n" << endl;
#endif
}