#include "SceneManager2D_Options.h"


CSceneManager2D_Options::CSceneManager2D_Options()
	: m_window_width(0)
	, m_window_height(0)
{
	CSceneManager2D::CSceneManager2D();
}
CSceneManager2D_Options::CSceneManager2D_Options(const int m_window_width, const int m_window_height)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
}

CSceneManager2D_Options::~CSceneManager2D_Options()
{
}
void CSceneManager2D_Options::Init()
{
	CSceneManager2D::Init();
}
void CSceneManager2D_Options::Update(double dt)
{
	CSceneManager2D::Update(dt);
}
void CSceneManager2D_Options::Render()
{
	CSceneManager2D::Render();
	CSceneManager2D::RenderOptions();
}
void CSceneManager2D_Options::Exit()
{
	CSceneManager2D::Exit();
}