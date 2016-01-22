#include "SceneManager.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
CSceneManager::CSceneManager(void)
	: m_window_width(800)
	, m_window_height(600)
	, m_cAvatar(NULL)
	, m_cSceneGraph(NULL)
	, m_cSpatialPartition(NULL)
	, m_cProjectileManager(NULL)
	, m_cPistol(NULL)
	, m_cMG(NULL)
	, m_cLG(NULL)
	, timer(0.f)
	, score(0)
{

}

CSceneManager::CSceneManager(const int m_window_width, const int m_window_height)
	: m_cAvatar(NULL)
	, m_cSceneGraph(NULL)
	, m_cSpatialPartition(NULL)
	, m_cProjectileManager(NULL)
	, m_cPistol(NULL)
	, m_cMG(NULL)
	, m_cLG(NULL)
	, timer(0.f)
	, score(0)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
}

CSceneManager::~CSceneManager(void)
{
	if (m_cProjectileManager)
	{
		delete m_cProjectileManager;
		m_cProjectileManager = NULL;
	}
	if (m_cSpatialPartition)
	{
		delete m_cSpatialPartition;
		m_cSpatialPartition = NULL;
	}
	if (m_cSceneGraph)
	{
		delete m_cSceneGraph;
		m_cSceneGraph = NULL;
	}
	if (m_cAvatar)
	{
		delete m_cAvatar;
		m_cAvatar = NULL;
	}
	if (m_cPistol)
	{
		delete m_cPistol;
		m_cPistol = NULL;
	}
	if (m_cMG)
	{
		delete m_cMG;
		m_cMG = NULL;
	}
	if (m_cLG)
	{
		delete m_cLG;
		m_cLG = NULL;
	}
	//drop when completed
	//TheSound->drop();
}
void CSceneManager::InitPistol()
{
	m_cPistol = new CWeapon();

	//the parameters are as such:
	//active weapon?, active number of rounds, clip size, max rounds possible, swappable?, reload active?, reload timer, empty?, firing rate(not applicaple to pistol)
	m_cPistol->Init(true, 12, 12, 72, true, false, 0.f, false, 0.f, false);
}
void CSceneManager::InitMG()
{
	m_cMG = new CWeapon();

	//the parameters are as such:
	//active weapon?, active number of rounds, clip size, max rounds possible, swappable?, reload active?, reload timer, empty?, firing rate, is it firing?
	m_cMG->Init(false, 35, 35, 135, true, false, 0.f, false, 0.1f, false);
}
void CSceneManager::InitLG()
{
	m_cLG = new CWeapon();

	//the parameters are as such:
	//active weapon?, active number of rounds, clip size, max rounds possible, swappable?, reload active?, reload timer, empty?, firing rate, is it firing?
	m_cLG->Init(false, 70, 70, 140, true, false, 0.f, false, 0.05f, false);
}
static int childNodeID;
void CSceneManager::Init()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	camera.Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);

	meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("GEO_SKYPLANE",
		Color(1, 1, 1), 128, 200.0f, 2000.0f, 1.0f, 1.0f);
	meshList[GEO_SKYPLANE]->textureID = LoadTGA("Image//top.tga");
	// Load the ground mesh and texture
	meshList[GEO_TILE] = MeshBuilder::GenerateQuad("tiles", Color(1, 1, 1), 1.f);
	meshList[GEO_TILE]->textureID = LoadTGA("Image//tile_floor.tga");

	meshList[GEO_BULLET] = MeshBuilder::GenerateOBJ("bullet", "OBJ/bullet.obj");
	meshList[GEO_BULLET]->textureID = LoadTGA("Image//bullet.tga");

	meshList[GEO_RAY] = MeshBuilder::GenerateOBJ("ray of batons", "OBJ/baton.obj");
	meshList[GEO_RAY]->textureID = LoadTGA("Image//baton.tga");

	meshList[GEO_AMMO] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_AMMO]->textureID = LoadTGA("Image//ammo.tga");

	meshList[GEO_PISTOL] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_PISTOL]->textureID = LoadTGA("Image//pistol.tga");

	meshList[GEO_MG] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_MG]->textureID = LoadTGA("Image//MG.tga");

	meshList[GEO_LG] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_LG]->textureID = LoadTGA("Image//laser.tga");

	meshList[GEO_AMMO_BOX] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_AMMO_BOX]->textureID = LoadTGA("Image//ammo_box.tga");

	meshList[GEO_SPATIAL_GRID] = MeshBuilder::GenerateCube("grid", Color(1, 0, 0), 1.f, true);
	meshList[GEO_SPATIAL_FILLED] = MeshBuilder::GenerateCube("grid filled", Color(0, 1, 0), 1.f);

	meshList[GEO_SPATIAL_GRID_ICON] = MeshBuilder::GenerateCube("grid", Color(1, 0, 0), 1.f, true);
	meshList[GEO_SPATIAL_GRID_ICON_FILLED] = MeshBuilder::GenerateCube("grid filled", Color(0, 1, 0), 1.f);

	meshList[GEO_RELOAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_RELOAD]->textureID = LoadTGA("Image//reload.tga");

	meshList[GEO_EMPTY] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_EMPTY]->textureID = LoadTGA("Image//empty.tga");

	meshList[GEO_SCORE] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_SCORE]->textureID = LoadTGA("Image//score.tga");

	meshList[GEO_PISTOL_XHAIR] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_PISTOL_XHAIR]->textureID = LoadTGA("Image//pistol_xhair.tga");

	meshList[GEO_MG_XHAIR] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_MG_XHAIR]->textureID = LoadTGA("Image//mg_xhair.tga");

	// Initialise and load a model into it
	m_cAvatar = new CPlayInfo3PV();
	//position. direction, up, topleft, bottomright
	m_cAvatar->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0), Vector3(15, 1, 10), Vector3(0, 1,0));
	m_cAvatar->SetModel(MeshBuilder::GenerateOBJ("OBJ1", "OBJ//stool.obj"));

	//create a scene graph
		m_cSceneGraph = new CSceneNode();
		CModel* newModel = new CModel();
		//newModel->Init();
		newModel->SetMesh(MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 5.f));
		newModel->SetActive(true);
		newModel->SetTopLeft(Vector3(5, 5, 5));
		newModel->SetBottomRight(Vector3(-5, -5, -5));
		m_cSceneGraph->SetNode(new CTransform(0, 0, 0), newModel);
	
	//head
	newModel = new CModel();
	newModel->SetMesh(MeshBuilder::GenerateCube("cube", Color(1, 0, 0), 2.f));
	childNodeID = m_cSceneGraph->AddChild(new CTransform(0, 3, 0), newModel);

	//ball bottom
	newModel = new CModel();
	newModel->SetMesh(MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 2.f));
	childNodeID = m_cSceneGraph->AddChild(new CTransform(0, -5, 0), newModel);

	//m_cSceneGraph->ApplyTranslate(100, 0, 100);

	// Create a spatial partition
	m_cSpatialPartition = new CSpatialPartition();
	m_cSpatialPartition->Init(100, 100, 5, 5);
	for (int i = 0; i < m_cSpatialPartition->GetxNumOfGrid(); i++)
	{
		for (int j = 0; j < m_cSpatialPartition->GetyNumOfGrid(); j++)
		{
			m_cSpatialPartition->SetGridMesh(i, j, NULL);
		}
	}

	//m_cSpatialPartition->PrintSelf();

	// Add the pointers to the scene graph to the spatial partition
	m_cSpatialPartition->AddObject(m_cSceneGraph);
	m_cSpatialPartition->AddObject(m_cAvatar->node);
	// Create the projectile manager
	m_cProjectileManager = new CProjectileManager();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	rotateAngle = 0;

	bLightEnabled = true;

	InitPistol();
	InitMG();
	InitLG();

}
void CSceneManager::SwapWeapon(double dt)
{
	//activate pistol mode
	if (Application::IsKeyPressed('1')  && m_cMG->getSwap() && m_cLG->getSwap())
	{
		if (!m_cPistol->getActiveWeapon())
		{
			m_cPistol->setActiveWeapon(true);
			m_cMG->setActiveWeapon(false);
			m_cLG->setActiveWeapon(false);
		}

	}
	//activate machine gun mode
	if (Application::IsKeyPressed('2') && m_cPistol->getSwap() && m_cLG->getSwap())
	{
		if (!m_cMG->getActiveWeapon())
		{
			m_cPistol->setActiveWeapon(false);
			m_cMG->setActiveWeapon(true);
			m_cLG->setActiveWeapon(false);
		}
	}
	//activate laser gun mode
	if (Application::IsKeyPressed('3') && m_cPistol->getSwap() && m_cMG->getSwap())
	{
		if (!m_cMG->getActiveWeapon())
		{
			m_cPistol->setActiveWeapon(false);
			m_cMG->setActiveWeapon(false);
			m_cLG->setActiveWeapon(true);
		}
	}
}
void CSceneManager::UpdateParticles(double dt)
{
	// Check for collisions fpr Projectiles
	// Render the projectiles
	Vector3 ProjectilePosition;
	Vector3 ProjectilePosition_End;
	for (int i = 0; i < m_cProjectileManager->GetMaxNumberOfProjectiles(); i++)
	{
		if (m_cProjectileManager->IsActive(i))
		{
			ProjectilePosition = m_cProjectileManager->theListOfProjectiles[i]->GetPosition();

			if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_DISCRETE)
			{
				//Destroy the projectile after collision
				if (m_cSpatialPartition->CheckForCollision(ProjectilePosition) == true)
				{
					m_cProjectileManager->RemoveProjectile(i);
					
					score += 2;
					cout << "HIT!" << endl;
				}
			}
			else if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_RAY)
			{
				ProjectilePosition_End = ProjectilePosition + m_cProjectileManager->theListOfProjectiles[i]->GetDirection() * m_cProjectileManager->theListOfProjectiles[i]->GetLength();

				// Destroy the projectile after collision
				if (m_cSpatialPartition->CheckForCollision(ProjectilePosition, ProjectilePosition_End) == true)
				{
					m_cProjectileManager->RemoveProjectile(i);

					score += 1;
					cout << "HIT!" << endl;
				}
					
			}
		}
	}
}
void CSceneManager::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if (Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	if (Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if (Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if (Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if (Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);

	rotateAngle -= Application::camera_yaw;// += (float)(10 * dt);

	m_cAvatar->Update(camera, dt);

	camera.UpdatePosition(m_cAvatar->GetPosition(), m_cAvatar->GetDirection());
	//camera.Update(dt);

	// Update the Projectile Manager
	m_cProjectileManager->Update(dt);

	SwapWeapon(dt);
	if (m_cPistol->getActiveWeapon())
	{
		UpdatePistol(dt);
	}
	if (m_cMG->getActiveWeapon())
	{
		UpdateMG(dt);
	}
	if (m_cLG->getActiveWeapon())
	{
		UpdateLG(dt);
	}

	UpdateParticles(dt);

	//clear the grid, update the spatial partition
	m_cSpatialPartition->Update(camera.position, (camera.target - camera.position).Normalize());
	m_cSpatialPartition->AddObject(m_cAvatar->node);
	m_cSpatialPartition->AddObject(m_cSceneGraph);
	timer += dt;
	//std::cout << m_cAvatar->node->GetTopLeft() << "," << m_cAvatar->node->GetBottomRight() << std::endl;

	fps = (float)(1.f / dt);
}

/********************************************************************************
 Update Camera position
 ********************************************************************************/
void CSceneManager::UpdateCameraStatus(const unsigned char key, const bool status)
{
	camera.UpdateStatus(key, status);
}

/********************************************************************************
 Update Avatar position
 ********************************************************************************/
void CSceneManager::UpdateAvatarStatus(const unsigned char key, const bool status)
{
	m_cAvatar->UpdateMovement(key, status);
	m_cAvatar->SetPos_x(camera.target.x);
	m_cAvatar->SetPos_z(camera.target.z);
}
static bool bLButtonState = false;
static bool bLButtonPressed = false;
/********************************************************************************
 Update Weapon status
 ********************************************************************************/
void CSceneManager::UpdateWeaponStatus(const unsigned char key)
{
}
void CSceneManager::UpdateMG(double dt)
{
	Vector3 SpawnLoc = m_cAvatar->GetPosition() - camera.getRight() * m_cAvatar->avataroffset;
	Vector3 Direction = Vector3(camera.target.x - camera.position.x, 0, camera.target.z - camera.position.z) - camera.getRight() * m_cAvatar->avataroffset;
	Direction.Normalize();

	//Mouse Section
	if (!bLButtonState && Application::IsMousePressed(0) && m_cMG->getNeedReload() == false && m_cMG->getActiveWeapon() == true && m_cMG->getIsEmpty() == false)
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		m_cMG->setIsFiring(true);
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
		//camera.target.y -= fRecoilPistol * (float) dt;
	}

	if (bLButtonState && m_cMG->getActiveRounds() != 0)
	{
		m_cMG->setFiringRate(m_cMG->getFiringRate() - dt);

		if (m_cMG->getIsFiring())
		{
			// Add a bullet object which starts at the camera position and moves in the camera's direction
			m_cProjectileManager->AddProjectile(SpawnLoc, Direction, 300.0f);
			m_cMG->setActiveRounds(m_cMG->getActiveRounds() - 1);
			m_cMG->setIsFiring(false);
		}
		if (m_cMG->getFiringRate() < 0 && m_cMG->getIsFiring() == false)
		{
			m_cMG->setFiringRate(0.1f);
			m_cMG->setIsFiring(true);
		}
	}

	//guard against double reload
	//voluntary reload
	if ((Application::IsKeyPressed('R') && m_cMG->getActiveRounds() < m_cMG->getClipSize()) || m_cMG->getActiveRounds() == 0)
	{
		m_cMG->setSwap(false);
		m_cMG->setNeedReload(true);
	}
	//call the need reload
	if (m_cMG->getNeedReload())
	{
		m_cMG->setReload(m_cMG->getReload() + dt);

		if (m_cMG->getReload() > 1.3f)
		{
			if (m_cMG->getClipSize() > m_cMG->getTotalRounds())
			{
				m_cMG->setActiveRounds(m_cMG->getTotalRounds());
				m_cMG->setTotalRounds(m_cMG->getTotalRounds() - m_cMG->getTotalRounds());
			}
			else
			{
				m_cMG->setTotalRounds(m_cMG->getTotalRounds() - (m_cMG->getClipSize() - m_cMG->getActiveRounds()));	//remove the required amount
				m_cMG->setActiveRounds(m_cMG->getClipSize());
			}

			m_cMG->setReload(0.f);
			m_cMG->setSwap(true);			//able to swap now
			m_cMG->setNeedReload(false);
		}
	}
	if (m_cMG->getActiveRounds() == 0 && m_cMG->getTotalRounds() == 0)
	{
		m_cMG->setSwap(true);
		m_cMG->setIsEmpty(true);
	}
}
void CSceneManager::UpdatePistol(double dt)
{
	Vector3 SpawnLoc = m_cAvatar->GetPosition() - camera.getRight() * m_cAvatar->avataroffset;
	Vector3 Direction = Vector3(camera.target.x - camera.position.x, -0.25, camera.target.z - camera.position.z) - camera.getRight() * m_cAvatar->avataroffset;
	Direction = Direction.Normalize();

	//Mouse Section
	if (!bLButtonState && Application::IsMousePressed(0) && !m_cPistol->getNeedReload() && m_cPistol->getActiveWeapon() && !m_cPistol->getIsEmpty())
	{
		bLButtonState = true;
		bLButtonPressed = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		// Add a bullet object which starts at the camera position and moves in the camera's direction
		m_cProjectileManager->AddProjectile(SpawnLoc, Direction, 300.0f);
		m_cPistol->setActiveRounds(m_cPistol->getActiveRounds() - 1);
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}

	//guard against double reload
	//voluntary reload
	if ((Application::IsKeyPressed('R') && m_cPistol->getActiveRounds() < m_cPistol->getClipSize()) || m_cPistol->getActiveRounds() == 0)
	{

		m_cPistol->setSwap(false);
		m_cPistol->setNeedReload(true);

	}
	//call the need reload, play sound only when pistol isnt empty
	if (m_cPistol->getNeedReload() && !m_cPistol->getIsEmpty())
	{
		m_cPistol->setReload(m_cPistol->getReload() + dt);

		if (m_cPistol->getReload() > 1.3f)
		{
			if (m_cPistol->getClipSize() > m_cPistol->getTotalRounds())
			{
				m_cPistol->setActiveRounds(m_cPistol->getTotalRounds());
				m_cPistol->setTotalRounds(m_cPistol->getTotalRounds() - m_cPistol->getTotalRounds());
			}
			else
			{
				m_cPistol->setTotalRounds(m_cPistol->getTotalRounds() - (m_cPistol->getClipSize() - m_cPistol->getActiveRounds()));	//remove the required amount
				m_cPistol->setActiveRounds(m_cPistol->getClipSize());
			}

			m_cPistol->setReload(0.f);
			m_cPistol->setSwap(true);			//able to swap now
			m_cPistol->setNeedReload(false);
		}
	}

	if (m_cPistol->getActiveRounds() == 0 && m_cPistol->getTotalRounds() == 0)
	{
		m_cPistol->setSwap(true);
		m_cPistol->setIsEmpty(true);
	}
}
void CSceneManager::UpdateLG(double dt)
{
	Vector3 SpawnLoc = m_cAvatar->GetPosition() - camera.getRight() * m_cAvatar->avataroffset;
	Vector3 Direction = Vector3(camera.target.x - camera.position.x, m_cAvatar->GetPos_y(), camera.target.z - camera.position.z) - camera.getRight() * m_cAvatar->avataroffset * 0.02;
	Direction.Normalize();
	//Mouse Section
	if (!bLButtonState && Application::IsMousePressed(0) && m_cLG->getNeedReload() == false && m_cLG->getActiveWeapon() == true && m_cLG->getIsEmpty() == false)
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		m_cLG->setIsFiring(true);
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
		//camera.target.y -= fRecoilPistol * (float) dt;
	}

	if (bLButtonState && m_cLG->getActiveRounds() != 0)
	{
		m_cLG->setFiringRate(m_cLG->getFiringRate() - dt);

		if (m_cLG->getIsFiring())
		{
			// Add a bullet object which starts at the camera position and moves in the camera's direction
			m_cProjectileManager->AddRayProjectile(SpawnLoc, Direction, 300.0f);
			m_cLG->setActiveRounds(m_cLG->getActiveRounds() - 1);
			m_cLG->setIsFiring(false);
		}
		if (m_cLG->getFiringRate() < 0 && m_cLG->getIsFiring() == false)
		{
			m_cLG->setFiringRate(0.05f);
			m_cLG->setIsFiring(true);
		}
	}

	//guard against double reload
	//voluntary reload
	if ((Application::IsKeyPressed('R') && m_cLG->getActiveRounds() < m_cLG->getClipSize()) || m_cLG->getActiveRounds() == 0)
	{
		m_cLG->setSwap(false);
		m_cLG->setNeedReload(true);
	}
	//call the need reload
	if (m_cLG->getNeedReload())
	{
		m_cLG->setReload(m_cLG->getReload() + dt);

		if (m_cLG->getReload() > 2.f)
		{
			if (m_cLG->getClipSize() > m_cLG->getTotalRounds())
			{
				m_cLG->setActiveRounds(m_cLG->getTotalRounds());
				m_cLG->setTotalRounds(m_cLG->getTotalRounds() - m_cLG->getTotalRounds());
			}
			else
			{
				m_cLG->setTotalRounds(m_cLG->getTotalRounds() - (m_cLG->getClipSize() - m_cLG->getActiveRounds()));	//remove the required amount
				m_cLG->setActiveRounds(m_cLG->getClipSize());
			}

			m_cLG->setReload(0.f);
			m_cLG->setSwap(true);			//able to swap now
			m_cLG->setNeedReload(false);
		}
	}
	if (m_cLG->getActiveRounds() == 0 && m_cLG->getTotalRounds() == 0)
	{
		m_cLG->setSwap(true);
		m_cLG->setIsEmpty(true);
	}
}
/********************************************************************************
 Start the rendering process by calling PreRendering()
 ********************************************************************************/
void CSceneManager::PreRendering(Vector3 translate, bool enableLight)
{
	modelStack.PushMatrix();
	modelStack.Translate(translate.x, translate.y, translate.z);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
}
void CSceneManager::PreRendering(Mtx44 rotate, bool enableLight)
{
	modelStack.PushMatrix();
	modelStack.MultMatrix(rotate);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	modelStack.PopMatrix();
}
/********************************************************************************
 Complete the rendering process by calling the PostRendering()
 ********************************************************************************/
void CSceneManager::PostRendering(void)
{
	//if(m_cModelMesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/********************************************************************************
 Render text onto the screen with reference position in the middle of the image
 ********************************************************************************/
void CSceneManager::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.8f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

/********************************************************************************
 Render text onto the screen
 ********************************************************************************/
void CSceneManager::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	//ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
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
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
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
	glEnable(GL_DEPTH_TEST);
}

/********************************************************************************
 Render a mesh in 2D
 ********************************************************************************/
void CSceneManager::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	if (rotate)
		modelStack.Rotate(rotateAngle, 0, 0, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
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
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

/********************************************************************************
 Render a mesh
 ********************************************************************************/
void CSceneManager::RenderMesh(Mesh *mesh, bool enableLight)
{
	if (mesh == NULL)
		return;

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
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
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/********************************************************************************
 Render mobile objects
 ********************************************************************************/
void CSceneManager::RenderGUI()
{
	// Render the crosshair
	//RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f);

	//On screen text
	std::ostringstream ss;
	ss.precision(3);

	//use this when there are available clips
	if ((m_cPistol->getNeedReload() && !m_cPistol->getIsEmpty()) || (m_cMG->getNeedReload() && !m_cMG->getIsEmpty()) || (m_cLG->getNeedReload() && !m_cLG->getIsEmpty()))
	{
		RenderMeshIn2D(meshList[GEO_RELOAD], false, 100.0f, 0, 0);
	}
	if (m_cPistol->getActiveWeapon())
	{
		//use this when there are no more available clips
		if (m_cPistol->getIsEmpty())
		{
			RenderMeshIn2D(meshList[GEO_EMPTY], false, 100.0f, 0, 0);
		}

		RenderMeshIn2D(meshList[GEO_PISTOL_XHAIR], false, 10.0f);
		for (int i = 0; i < m_cPistol->getActiveRounds(); i++)
		{
			RenderMeshIn2D(meshList[GEO_AMMO], false, 5.0f, -68 + (i * 5), 53);
		}
		for (int j = 0; j < (m_cPistol->getTotalRounds() / m_cPistol->getClipSize()); j++)
		{
			RenderMeshIn2D(meshList[GEO_AMMO_BOX], false, 9.0f, -68 + (j * 4), 43);
		}
		RenderMeshIn2D(meshList[GEO_PISTOL], false, 10.0f, -73, 53);
		/*ss << "Bullet Count: " << m_cPistol->getActiveRounds();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);*/
	}
	else if (m_cMG->getActiveWeapon())
	{
		//use this when there are no more available clips
		if (m_cLG->getIsEmpty())
		{
			RenderMeshIn2D(meshList[GEO_EMPTY], false, 100.0f, 0, 0);
		}
		//use this when there are no more available clips
		if (m_cMG->getIsEmpty())
		{
			RenderMeshIn2D(meshList[GEO_EMPTY], false, 100.0f, 0, 0);
		}
		RenderMeshIn2D(meshList[GEO_MG_XHAIR], false, 10.0f);
		RenderMeshIn2D(meshList[GEO_MG], false, 8.0f, -73, 53);
		for (int i = 0; i < m_cMG->getActiveRounds(); i++)
		{
			RenderMeshIn2D(meshList[GEO_AMMO], false, 4.0f, -68 + (i * 4), 53);
		}
		for (int j = 0; j < (m_cMG->getTotalRounds() / m_cMG->getClipSize()); j++)
		{
			RenderMeshIn2D(meshList[GEO_AMMO_BOX], false, 9.0f, -68 + (j * 4), 43);
		}
		/*ss << "Bullet Count: " << m_cMG->getActiveRounds();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);*/
	}
	else if (m_cLG->getActiveWeapon())
	{
		//use this when there are no more available clips
		if (m_cLG->getIsEmpty())
		{
			RenderMeshIn2D(meshList[GEO_EMPTY], false, 100.0f, 0, 0);
		}
		RenderMeshIn2D(meshList[GEO_PISTOL_XHAIR], false, 10.0f);
		RenderMeshIn2D(meshList[GEO_LG], false, 8.0f, -73, 53);
		for (int i = 0; i < m_cLG->getActiveRounds(); i++)
		{
			RenderMeshIn2D(meshList[GEO_AMMO], false, 2.0f, -68 + (i * 2), 53);
		}
		for (int j = 0; j < (m_cLG->getTotalRounds() / m_cLG->getClipSize()); j++)
		{
			RenderMeshIn2D(meshList[GEO_AMMO_BOX], false, 9.0f, -68 + (j * 4), 43);
		}
		/*ss << "Bullet Count: " << m_cLG->getActiveRounds();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);*/
	}
	RenderMeshIn2D(meshList[GEO_SCORE], false, 20.0f, -63, 28);
	//On screen text
	std::ostringstream ss3;
	ss3 << score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(1, 1, 1), 50, 150, 418);
	//On screen text
	std::ostringstream ss2;
	ss2.precision(3);

	ss2 << "ProjCount: " << m_cProjectileManager->GetNumberOfActiveProjectiles();
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 30, 0, 3);



	//
	//RenderTextOnScreen(meshList[GEO_TEXT], "Hello Screen", Color(0, 1, 0), 3, 0, 0);

	//Render the spatial partition
	for (int i = 0; i < m_cSpatialPartition->GetxNumOfGrid(); i++)
	{
		for (int j = 0; j < m_cSpatialPartition->GetyNumOfGrid(); j++)
		{
			modelStack.PushMatrix();
			glEnable(GL_CULL_FACE);
			modelStack.Translate(m_cSpatialPartition->xGridSize * i, 0, m_cSpatialPartition->yGridSize * j);
			modelStack.Scale(m_cSpatialPartition->xGridSize, m_cSpatialPartition->xGridSize, m_cSpatialPartition->xGridSize);
			if (m_cSpatialPartition->GetGrid(i, j).GetListOfObject().size() > 0) //if have something make grid green
			{
				RenderMeshIn2D(meshList[GEO_SPATIAL_GRID_ICON_FILLED], false, 2.f, 50 + 3 * i, 30 + 3 * j, false);
			}
			else
			{
				RenderMeshIn2D(meshList[GEO_SPATIAL_GRID_ICON], false, 2.f, 50 + 3 * i, 30 + 3 * j, false);
			}
			modelStack.PopMatrix();
		}
	}
}

/********************************************************************************
 Render mobile objects
 ********************************************************************************/
void CSceneManager::RenderMobileObjects()
{
	// Render the projectiles
	Vector3 ProjectilePosition;
	for (int i = 0; i < m_cProjectileManager->GetMaxNumberOfProjectiles(); i++)
	{
		if (m_cProjectileManager->IsActive(i))
		{
			ProjectilePosition = m_cProjectileManager->theListOfProjectiles[i]->GetPosition();
			modelStack.PushMatrix();
			modelStack.Translate(ProjectilePosition.x, ProjectilePosition.y - 2, ProjectilePosition.z);
			if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_DISCRETE)
			{
				RenderMesh(meshList[GEO_BULLET], false);
			}
			else if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_RAY)
			{
				glLineWidth(5.0f);
				RenderMesh(meshList[GEO_RAY], false);
				glLineWidth(1.0f);
			}

			modelStack.PopMatrix();
		}
	}

	// Render the Avatar
	modelStack.PushMatrix();
	modelStack.Translate(m_cAvatar->GetPos_x() - camera.getRight().x * m_cAvatar->avataroffset, m_cAvatar->GetPos_y() - 10, m_cAvatar->GetPos_z() - camera.getRight().z * m_cAvatar->avataroffset);
	float theta = Math::RadianToDegree(atan2(camera.getDirection().x, camera.getDirection().z));
	modelStack.Rotate(theta, 0, 1, 0);
	RenderMesh(m_cAvatar->theAvatarMesh, false);
	modelStack.PopMatrix();
	////Render the top left and bottom right
	//modelStack.PushMatrix();
	//modelStack.Translate(m_cAvatar->node->GetTopLeft().x, 0, m_cAvatar->node->GetTopLeft().z);
	//RenderMesh(meshList[GEO_SPHERE], false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(m_cAvatar->node->GetBottomRight().x, 0, m_cAvatar->node->GetBottomRight().z);
	//RenderMesh(meshList[GEO_SPHERE], false);
	//modelStack.PopMatrix();

	m_cSceneGraph->ApplyTranslate(0.01, 0, 0.01);
	m_cSceneGraph->GetNode(2);
	// Draw the scene graph
	m_cSceneGraph->Draw(this);
}

/********************************************************************************
 Render the lights in this scene
 ********************************************************************************/
void CSceneManager::RenderFixedObjects()
{
	RenderMesh(meshList[GEO_AXES], false);
}

/********************************************************************************
 Render the lights in this scene
 ********************************************************************************/
void CSceneManager::RenderLights()
{
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if (lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
}
void CSceneManager::RenderSkyPlane(Mesh* mesh, Color color, int slices, float
	PlanetRadius, float AtmosphereRadius, float hTile, float vTile)
{
	modelStack.PushMatrix();
	modelStack.Translate(500, 1800, -500);
	RenderMesh(meshList[GEO_SKYPLANE], false);
	modelStack.PopMatrix();
}

/********************************************************************************
 Render the ground in this scene
 ********************************************************************************/
void CSceneManager::RenderGround()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -10);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);

	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x - 5.0f, z - 5.0f, 0.0f);
			if (((x * 9 + z) % 2) == 0)
				RenderMesh(meshList[GEO_TILE], false);
			else
				RenderMesh(meshList[GEO_TILE], false);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();

	//Render the spatial partition
	for (int i = 0; i < m_cSpatialPartition->GetxNumOfGrid(); i++)
	{
		for (int j = 0; j < m_cSpatialPartition->GetyNumOfGrid(); j++)
		{
			modelStack.PushMatrix();
			glEnable(GL_CULL_FACE);
			modelStack.Translate(m_cSpatialPartition->xGridSize * i, -m_cSpatialPartition->yGridSize+42, m_cSpatialPartition->yGridSize * j);
			modelStack.Scale(m_cSpatialPartition->xGridSize, m_cSpatialPartition->xGridSize, m_cSpatialPartition->xGridSize);
			if (m_cSpatialPartition->GetGrid(i, j).GetListOfObject().size() > 0) //if have something make grid green
			{
				RenderMesh(meshList[GEO_SPATIAL_FILLED], false);
			}
			else
			{
				RenderMesh(meshList[GEO_SPATIAL_GRID], false);
			}
			modelStack.PopMatrix();
		}
	}
}
/********************************************************************************
 Render this scene
 ********************************************************************************/
void CSceneManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Set up the view
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderLights();
	RenderGround();
	RenderSkyPlane(meshList[GEO_SKYPLANE], Color(1, 1, 1), 128, 300.0f, 2000.0f, 1.0f, 1.0f);
	RenderFixedObjects();
	RenderMobileObjects();

	RenderGUI();
}
/********************************************************************************
 Exit process for this scene
 ********************************************************************************/
void CSceneManager::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
