#include "SceneManager2D_Highscore.h"


CSceneManager2D_Highscore::CSceneManager2D_Highscore()
	: m_window_width(0)
	, m_window_height(0)
{
	CSceneManager2D::CSceneManager2D();
}
CSceneManager2D_Highscore::CSceneManager2D_Highscore(const int m_window_width, const int m_window_height)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
}

CSceneManager2D_Highscore::~CSceneManager2D_Highscore()
{
}
void CSceneManager2D_Highscore::Init()
{
	CSceneManager2D::Init();
	CSceneManager2D::LoadHighscore();
}
void CSceneManager2D_Highscore::Update(double dt)
{
	CSceneManager2D::Update(dt);
}
void CSceneManager2D_Highscore::Render()
{
	CSceneManager2D::Render();
	CSceneManager2D::RenderMenu();
	CSceneManager2D::RenderHighscore();
}
void CSceneManager2D_Highscore::Exit()
{
	CSceneManager2D::Exit();
}