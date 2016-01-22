#include "SceneManager2D_Intro.h"


CSceneManager2D_Intro::CSceneManager2D_Intro()
	: m_window_width(0)
	, m_window_height(0)
{
	CSceneManager2D::CSceneManager2D();
}
CSceneManager2D_Intro::CSceneManager2D_Intro(const int m_window_width, const int m_window_height)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
}

CSceneManager2D_Intro::~CSceneManager2D_Intro()
{
}
void CSceneManager2D_Intro::Init()
{
	CSceneManager2D::Init();
}
void CSceneManager2D_Intro::Update(double dt)
{
	CSceneManager2D::Update(dt);
	CSceneManager2D::UpdateSounds(dt);
}
void CSceneManager2D_Intro::Render()
{
	CSceneManager2D::Render();
	CSceneManager2D::RenderIntro();
}
void CSceneManager2D_Intro::Exit()
{
	CSceneManager2D::Exit();
}