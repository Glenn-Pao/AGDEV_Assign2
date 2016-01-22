#include "SceneManager2D_Splash.h"


CSceneManager2D_Splash::CSceneManager2D_Splash()
	: m_window_width(0)
	, m_window_height(0)
{
	CSceneManager2D::CSceneManager2D();
}
CSceneManager2D_Splash::CSceneManager2D_Splash(const int m_window_width, const int m_window_height)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
}

CSceneManager2D_Splash::~CSceneManager2D_Splash()
{
}
void CSceneManager2D_Splash::Init()
{
	CSceneManager2D::Init();
}
void CSceneManager2D_Splash::Update(double dt)
{
	CSceneManager2D::Update(dt);
	//CSceneManager2D::UpdateSounds(dt);
}
void CSceneManager2D_Splash::Render()
{
	CSceneManager2D::Render();
	CSceneManager2D::RenderSplash();
}
void CSceneManager2D_Splash::Exit()
{
	CSceneManager2D::Exit();
}