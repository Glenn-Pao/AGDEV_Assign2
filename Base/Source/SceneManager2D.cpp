#include "SceneManager2D.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "Strategy_Kill.h"

CSceneManager2D::CSceneManager2D()
	: offset(-720)
{
	
}
CSceneManager2D::CSceneManager2D(const int m_window_width, const int m_window_height)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
}
CSceneManager2D::~CSceneManager2D()
{
}

void CSceneManager2D::Init()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Switch on culling
	glEnable(GL_CULL_FACE);
	
	// Render mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Activate blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate and bind vertex array
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shaders
	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Text.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	// Initialise the camera
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Create the meshes
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//arialbold.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Load the ground mesh and texture
	meshList[GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//sky_background.tga");

	meshList[GEO_SPLASH] = MeshBuilder::Generate2DMesh("GEO_SPLASH", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_SPLASH]->textureID = LoadTGA("Image//splashscreen2.tga");
	meshList[GEO_SPLASH_MASK] = MeshBuilder::Generate2DMesh("GEO_SPLASH_MASK", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_SPLASH_MASK]->textureID = LoadTGA("Image//splashscreen2_mask.tga");
	meshList[GEO_SPLASH_FINAL] = MeshBuilder::Generate2DMesh("GEO_SPLASH_FINAL", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_SPLASH_FINAL]->textureID = LoadTGA("Image//splashscreen2_final.tga");
	meshList[GEO_INTRO] = MeshBuilder::Generate2DMesh("GEO_INTRO", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_INTRO]->textureID = LoadTGA("Image//splashscreen.tga");
	meshList[GEO_MAIN_MENU] = MeshBuilder::Generate2DMesh("GEO_MAIN_MENU", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_MAIN_MENU]->textureID = LoadTGA("Image//mainmenu.tga");
	meshList[GEO_HIGHSCORE] = MeshBuilder::Generate2DMesh("GEO_HIGHSCORE", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_HIGHSCORE]->textureID = LoadTGA("Image//highscore.tga");
	meshList[GEO_INSTRUCTIONS_1] = MeshBuilder::Generate2DMesh("GEO_INSTRUCTIONS_1", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_INSTRUCTIONS_1]->textureID = LoadTGA("Image//instructions1.tga");
	meshList[GEO_INSTRUCTIONS_2] = MeshBuilder::Generate2DMesh("GEO_INSTRUCTIONS_2", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_INSTRUCTIONS_2]->textureID = LoadTGA("Image//instructions2.tga");
	meshList[GEO_OPTIONS] = MeshBuilder::Generate2DMesh("GEO_OPTIONS", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_OPTIONS]->textureID = LoadTGA("Image//option.tga");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;
}
void CSceneManager2D::LoadHighscore()
{
	const int length = 10;
	std::string value[length];
	std::ifstream is("Text/highscore.txt");
	std::string::size_type size;

	int i = 0;
	while (!is.eof() && i < length)
	{
		is >> value[i++];
	}
	is.close();

	for (int j = 0; j < length; ++j)
	{
		m_highscore[j] = std::stoi(value[j], &size);
	}
}
void CSceneManager2D::UpdateSounds(double dt)
{
	/*if (MainMenu->isFinished())
	{
		MainMenu = NULL;
	}*/
}
void CSceneManager2D::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	rotateAngle -= (float)Application::camera_yaw;// += (float)(10 * dt);

	camera.Update(dt);

	offset += 7.f;
	
	UpdateSounds(dt);

	fps = (float)(1.f / dt);
}
void CSceneManager2D::setOffset(float offset)
{
	this->offset = offset;
}
float CSceneManager2D::getOffset(void)
{
	return offset;
}
/********************************************************************************
 Update Camera position
 ********************************************************************************/
void CSceneManager2D::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
 Update Weapon status
 ********************************************************************************/
void CSceneManager2D::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
	}
}

/********************************************************************************
 Render text onto the screen
 ********************************************************************************/
void CSceneManager2D::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
				glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
				//	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textureID);
				glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				for(unsigned i = 0; i < text.length(); ++i)
				{
					Mtx44 characterSpacing;
					characterSpacing.SetToTranslation((i*0.5f) + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
					Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
					glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
					mesh->Render((unsigned)text[i] * 6, 6);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

/********************************************************************************
 Render 2D Mesh
 ********************************************************************************/
void CSceneManager2D::Render2DMesh(Mesh *mesh, bool enableLight, int size, int x, int y, bool rotate, bool flip)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate( (float)x, (float)y, 0);
				modelStack.Scale( (float)size, (float)size, (float)size);
				if (rotate)
					modelStack.Rotate(rotateAngle, 0, 0, 1);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}
				else
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}
				mesh->Render();
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
}
/********************************************************************************
Render the intro
********************************************************************************/
void CSceneManager2D::RenderSplash()
{
	// Render the splash screen
	if (offset < 0)
	{
		Render2DMesh(meshList[GEO_SPLASH], false, 1);
		Render2DMesh(meshList[GEO_SPLASH_MASK], false, 1, offset);
	}
	else if (offset >= 0)
	{
		Render2DMesh(meshList[GEO_SPLASH_FINAL], false, 1);
	}

	//Debug feature
	std::ostringstream ss;
	ss << "Splash Screen";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);
}
void CSceneManager2D::RenderHighscore()
{
	Render2DMesh(meshList[GEO_HIGHSCORE], false, 1);

	std::ostringstream ss;
	const int length = 10;
	for (int i = 0; i < length; i++)
	{
		ss.str(std::string());
		ss << i + 1 << ". " << m_highscore[i];

		if (i == 9)
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 60, 255, 400 - (i * 40));
		else
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 60, 270, 400 - (i * 40));
	}
}
/********************************************************************************
 Render the intro
 ********************************************************************************/
void CSceneManager2D::RenderIntro()
{
	// Render the intro screen
	Render2DMesh(meshList[GEO_INTRO], false, 1);

	//Debug feature
	std::ostringstream ss;
	ss << "Intro Screen";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);
}
void CSceneManager2D::RenderMenu()
{
	// Render the crosshair
	Render2DMesh(meshList[GEO_MAIN_MENU], false, 1);

	//On screen text
	std::ostringstream ss;
	ss << "Menu Screen";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);
}
void CSceneManager2D::RenderInstructions1()
{
	Render2DMesh(meshList[GEO_INSTRUCTIONS_1], false, 1);
}
void CSceneManager2D::RenderInstructions2()
{
	Render2DMesh(meshList[GEO_INSTRUCTIONS_2], false, 1);
}
void CSceneManager2D::RenderOptions()
{
	Render2DMesh(meshList[GEO_OPTIONS], false, 1);
}
/********************************************************************************
 Render this scene
 ********************************************************************************/
void CSceneManager2D::Render()
{
	glDisable (GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	// Render the background image
	//RenderBackground();
	/*
	// Render the rear tile map
	RenderRearTileMap();
	// Render the tile map
	RenderTileMap();
	// Render the goodies
	RenderGoodies();

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "theEnemy: " << theEnemy->GetPos_x() << ", " << theEnemy->GetPos_y();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);
	std::ostringstream sss;
	sss.precision(5);
	sss << "mapOffset_x: "<<theHero->GetMapOffset_x();
	RenderTextOnScreen(meshList[GEO_TEXT], sss.str(), Color(0, 1, 0), 30, 0, 30);*/
}

/********************************************************************************
 Exit this scene
 ********************************************************************************/
void CSceneManager2D::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}