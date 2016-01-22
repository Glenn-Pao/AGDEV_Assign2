#include "SceneManager2D_Menu.h"


CSceneManager2D_Menu::CSceneManager2D_Menu()
	: m_window_width(0)
	, m_window_height(0)
{
	CSceneManager2D::CSceneManager2D();
}
CSceneManager2D_Menu::CSceneManager2D_Menu(const int m_window_width, const int m_window_height)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
}

CSceneManager2D_Menu::~CSceneManager2D_Menu()
{
}
void CSceneManager2D_Menu::Init()
{
	CSceneManager2D::Init();
}
void CSceneManager2D_Menu::Update(double dt)
{
	CSceneManager2D::Update(dt);
}
void CSceneManager2D_Menu::Render()
{
	CSceneManager2D::Render();
	CSceneManager2D::RenderMenu();
}
void CSceneManager2D_Menu::Exit()
{
	CSceneManager2D::Exit();
}