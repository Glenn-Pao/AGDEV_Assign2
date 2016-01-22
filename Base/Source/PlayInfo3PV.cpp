#include "PlayInfo3PV.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "Camera3.h"
#include "SceneGraph\Model.h"


static float CAMERA_SPEED = 200.f;
Camera3 camera;
CPlayInfo3PV::CPlayInfo3PV(void)
	: theAvatarMesh(NULL)
	, jumpspeed(0)
{
}


CPlayInfo3PV::~CPlayInfo3PV(void)
{
	if (theAvatarMesh)
	{
		delete theAvatarMesh;
		theAvatarMesh = NULL;
	}
}

// Initialise this class instance
void CPlayInfo3PV::Init(const Vector3& pos, const Vector3& direction, const Vector3& up, Vector3 vTopLeft, Vector3 vBottomRight)
{
	this->curPosition = defaultPos = pos;
	this->curDirection = defaultDir = direction;
	Vector3 view = (direction - pos).Normalized();
	right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	//pitchLimit = 0;
	avataroffset = 0;

	node = new CSceneNode();
	CModel* newModel = new CModel();
	newModel->Init();
	newModel->SetTopLeft(vTopLeft);
	newModel->SetBottomRight(vBottomRight);
	//newModel->SetTopLeft(pos + Vector3(1, 1, 1));	//set the top left of model
	//newModel->SetBottomRight(pos + Vector3(-1, -1, -1));
	node->SetNode(new CTransform(curPosition.x, 0, curPosition.z), newModel);

	// Initialise the Avatar's movement flags
	for(int i=0; i<255; i++){
		myKeys[i] = false;
	}
}

// Set Model
bool CPlayInfo3PV::SetModel(Mesh* theAvatarMesh)
{
	this->theAvatarMesh = theAvatarMesh;

	if (this->theAvatarMesh == NULL)
		return false;

	return true;
}

// Returns true if the player is on ground
bool CPlayInfo3PV::isOnGround(void)
{
	return true;
}

// Returns true if the player is jumping upwards
bool CPlayInfo3PV::isJumpUpwards(void)
{
	return true;
}

// Returns true if the player is on freefall
bool CPlayInfo3PV::isFreeFall(void)
{
	return true;
}

// Set the player's status to free fall mode
void CPlayInfo3PV::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		jumpspeed = 0;
	}
}

// Set the player to jumping upwards
void CPlayInfo3PV::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		jumpspeed = 15;
	}
}

// Set position x of the player
void CPlayInfo3PV::SetPos_x(float pos_x)
{
	curPosition.x = pos_x;
}

// Set position y of the player
void CPlayInfo3PV::SetPos_y(float pos_y)
{
	curPosition.y = pos_y;
}

// Set position y of the player
void CPlayInfo3PV::SetPos_z(float pos_z)
{
	curPosition.z = pos_z;
}

// Set Jumpspeed of the player
void CPlayInfo3PV::SetJumpspeed(int jumpspeed)
{
	this->jumpspeed = jumpspeed;
}

// Stop the player's movement
void CPlayInfo3PV::SetToStop(void)
{
	jumpspeed = 0;
}
void CPlayInfo3PV::MoveLeft(const double dt)
{
	curPosition.x = curPosition.x - (int)(50.f * dt) * Vector3(curDirection.x, 0, curDirection.z).Cross(Vector3(0, 1, 0)).x;
	curPosition.z = curPosition.x - (int)(50.f * dt) * Vector3(curDirection.x, 0, curDirection.z).Cross(Vector3(0, 1, 0)).z;
}
void CPlayInfo3PV::MoveRight(const double dt)
{
	curPosition.x = curPosition.x + (int)(50.f * dt) * Vector3(curDirection.x, 0, curDirection.z).Cross(Vector3(0, 1, 0)).x;
	curPosition.z = curPosition.x + (int)(50.f * dt) * Vector3(curDirection.x, 0, curDirection.z).Cross(Vector3(0, 1, 0)).z;
}

void CPlayInfo3PV::TurnLeft(const double dt)
{
	float yaw = (float)(Application::camera_yaw * -50) *(float)dt;
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	curDirection = rotation * curDirection;
	right = curDirection.Cross(up);
	right.y = 0;
	right.Normalize();
	//up = right.Cross(curDirection).Normalized();
}
void CPlayInfo3PV::TurnRight(const double dt)
{
	float yaw = (float)(Application::camera_yaw * -50) * (float)dt;
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	curDirection = rotation * curDirection;
	right = curDirection.Cross(up);
	right.y = 0;
	right.Normalize();
	//up = right.Cross(curDirection).Normalized();
}
void CPlayInfo3PV::Strafe(const double dt)
{
	if (dt > 0)
		MoveRight(dt);
	else if (dt < 0)
		MoveLeft(abs(dt));
}
void CPlayInfo3PV::Pitch(const double dt)
{
	if (Application::camera_pitch > 0.0 /*&& pitchLimit < 10*/)
	{	
		float pitch = (float)(Application::camera_pitch * -100) * (float)dt;
		curDirection.Normalized();
		right = curDirection.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(curDirection).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		curDirection = rotation * curDirection;
		//curDirection.y += -Application::camera_pitch * dt;

		//pitchLimit += Application::camera_pitch;
	}
	else if (Application::camera_pitch < 0.0 /*&& pitchLimit > -5*/)
	{
		float pitch = (float)(Application::camera_pitch * -100) * (float)dt;
		curDirection.Normalized();
		right = curDirection.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(curDirection).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		curDirection = rotation * curDirection;

		//pitchLimit += Application::camera_pitch;
	}
}
void CPlayInfo3PV::Yaw(const double dt)
{
	if (Application::camera_yaw > 0.0)
		TurnRight(dt);
	else if (Application::camera_yaw < 0.0)
		TurnLeft(dt);
}
/********************************************************************************
 Hero Move Up Down
 ********************************************************************************/
void CPlayInfo3PV::MoveFrontBack(const bool mode, const float timeDiff)
{
	if (mode)
	{
		curPosition.z = curPosition.z - (50.0f * timeDiff) * curDirection.z;
		curPosition.x = curPosition.x - (50.0f * timeDiff) * curDirection.x;
	}
	else
	{
		curPosition.z = curPosition.z + (50.0f * timeDiff) * curDirection.z;
		curPosition.x = curPosition.x + (50.0f * timeDiff) * curDirection.x;
	}
}

/********************************************************************************
 Hero Move Left Right
 ********************************************************************************/
void CPlayInfo3PV::MoveLeftRight(const bool mode, const float timeDiff)
{
	if (mode)
	{
		curPosition.x = curPosition.x - (50.f * timeDiff) * Vector3(curDirection.x, 0, curDirection.z).Cross(Vector3(0, 1, 0)).x;
		curPosition.z = curPosition.z - (50.f * timeDiff) * Vector3(curDirection.x, 0, curDirection.z).Cross(Vector3(0, 1, 0)).z;
	}
	else
	{
		curPosition.x = curPosition.x + (50.f * timeDiff) * Vector3(curDirection.x, 0, curDirection.z).Cross(Vector3(0, 1, 0)).x;
		curPosition.z = curPosition.z + (50.f * timeDiff) * Vector3(curDirection.x, 0, curDirection.z).Cross(Vector3(0, 1, 0)).z;
	}
}


// Get position x of the player
float CPlayInfo3PV::GetPos_x(void)
{
	return curPosition.x;
}

// Get position y of the player
float CPlayInfo3PV::GetPos_y(void)
{
	return curPosition.y;
}

// Get position y of the player
float CPlayInfo3PV::GetPos_z(void)
{
	return curPosition.z;
}

// Get position of the player
Vector3 CPlayInfo3PV::GetPosition()
{
	return curPosition;
}

// Get direction of the player
Vector3 CPlayInfo3PV::GetDirection()
{
	return curDirection;
}

// Get Jumpspeed of the player
int CPlayInfo3PV::GetJumpspeed(void)
{
	return jumpspeed;
}

// Update Jump Upwards
void CPlayInfo3PV::UpdateJumpUpwards()
{
	curPosition.y -= jumpspeed;
	jumpspeed -= 1;
	if (jumpspeed == 0)
	{
	}
}

// Update FreeFall
void CPlayInfo3PV::UpdateFreeFall()
{
	curPosition.y += jumpspeed;
	jumpspeed += 1;
}

// Constrain the position of the Hero to within the border
void CPlayInfo3PV::ConstrainHero(const float leftBorder, const float rightBorder,
	const float topBorder, const float bottomBorder,
								  float timeDiff)
{
	if (curPosition.x < leftBorder)
	{
		curPosition.x = leftBorder;
	}
	else if (curPosition.x > rightBorder)
	{
		curPosition.x = rightBorder;
	}

	if (curPosition.y < topBorder)
		curPosition.y = topBorder;
	else if (curPosition.y > bottomBorder)
		curPosition.y = bottomBorder;
}


/********************************************************************************
 Update Movement
 ********************************************************************************/
void CPlayInfo3PV::UpdateMovement(const unsigned char key, const bool status)
{
	myKeys[key] = status;
}

/********************************************************************************
 Update
 ********************************************************************************/
void CPlayInfo3PV::Update(Camera3 &camera, double dt)
{
	// WASD movement
	if ( myKeys['w'] == true)
	{
		MoveFrontBack( false, dt );
		node->ApplyTranslate(curPosition.x - node->GetTranslate().x, 0, curPosition.z - node->GetTranslate().z);
	}
	if (myKeys['s'] == true)
	{
		MoveFrontBack( true, dt );
		node->ApplyTranslate(curPosition.x - node->GetTranslate().x, 0, curPosition.z - node->GetTranslate().z);
	}
	if (myKeys['a'] == true)
	{
		MoveLeftRight( true, dt );
		if (avataroffset > -5)
		{
			avataroffset -= 20 * dt;
		}
		node->ApplyTranslate(curPosition.x - node->GetTranslate().x, 0, curPosition.z - node->GetTranslate().z);
	}
	if (myKeys['d'] == true)
	{
		//Strafe(dt);
		MoveLeftRight( false, dt );
		if (avataroffset < 5)
		{
			avataroffset += 20 * dt;
		}
		node->ApplyTranslate(curPosition.x - node->GetTranslate().x, 0, curPosition.z - node->GetTranslate().z);
	}
	else
	{
		if (avataroffset > 10 * dt)
			avataroffset -= 10 * dt;
		else if (avataroffset < -10 * dt)
			avataroffset += 10 * dt; 
	}


	// Rotation
	/*
	if ( myKeys[VK_UP] == true)
	{
		LookUp( dt );
	}
	if (myKeys[VK_DOWN] == true)
	{
		LookUp( -dt );
	}
	if (myKeys[VK_LEFT] == true)
	{
		TurnLeft( -dt );
	}
	if (myKeys[VK_RIGHT] == true)
	{
		TurnRight( dt );
	}

	// Jump
	if (myKeys[32] == true)
	{
		Jump( dt );
		myKeys[32]	= false;
	}
	UpdateJump(dt);
	*/
	//Update the camera direction based on mouse move
	// left-right rotate
	if ( Application::camera_yaw != 0 )
		Yaw( dt );
	if ( Application::camera_pitch != 0 )
		Pitch( dt );

	/*if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
	*/
}