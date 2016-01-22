#pragma once
#include "Vector3.h"
#include "Mesh.h"
#include "Camera3.h"
#include "SceneGraph\SceneNode.h"

class CPlayInfo3PV : public  Base_SCENENODE
{
public:
	CPlayInfo3PV(void);
	virtual ~CPlayInfo3PV(void);

	enum GEOMETRY_TYPE
	{
		GEO_AVATAR,
		NUM_GEOMETRY,
	};

	// Initialise this class instance
	void Init(const Vector3& pos, const Vector3& direction, const Vector3& up, Vector3 vTopLeft, Vector3 vBottomRight);

	// Set Model
	bool SetModel( Mesh* theAvatarMesh);

	// Returns true if the player is on ground
	bool isOnGround(void);
	// Returns true if the player is jumping upwards
	bool isJumpUpwards(void);
	// Returns true if the player is on freefall
	bool isFreeFall(void);
	// Set the player's status to free fall mode
	void SetOnFreeFall(bool isOnFreeFall);
	// Set the player to jumping upwards
	void SetToJumpUpwards(bool isOnJumpUpwards);
	// Stop the player's movement
	void SetToStop(void);
	// Set position x of the player
	void SetPos_x(float pos_x);
	// Set position y of the player
	void SetPos_y(float pos_y);
	// Set position z of the player
	void SetPos_z(float pos_z);	
	// Set Jumpspeed of the player
	void SetJumpspeed(int jumpspeed);

	// Update Movements
	void MoveFrontBack(const bool mode, const float timeDiff);
	void MoveLeftRight(const bool mode, const float timeDiff);

	// Get position x of the player
	float GetPos_x(void);
	// Get position y of the player
	float GetPos_y(void);
	// Get position z of the player
	float GetPos_z(void);
	// Get position of the player
	Vector3 GetPosition();
	// Get direction of the player
	Vector3 GetDirection();
	// Get Jumpspeed of the player
	int GetJumpspeed(void);

	// Update Jump Upwards
	void UpdateJumpUpwards();
	// Update FreeFall
	void UpdateFreeFall();
	// Update
	void UpdateMovement(const unsigned char key, const bool status = true);
	// Update
	void Update(Camera3 &camera, double dt);

	// Reset the avatar to default settings
	void Reset();

	// Constrain the position of the Hero to within the border
	void ConstrainHero(const float leftBorder, const float rightBorder,
		const float topBorder, const float bottomBorder,
					   float timeDiff);

	// Avatar's Mesh
	Mesh*	theAvatarMesh;


	//Camera controls
	virtual void MoveLeft(const double dt);
	virtual void MoveRight(const double dt);
	virtual void TurnLeft(const double dt);
	virtual void TurnRight(const double dt);

	//applied methods
	virtual void Pitch(const double dt);
	virtual void Yaw(const double dt);
	virtual void Strafe(const double dt);

	float avataroffset;
private:
	// Hero's information
	Vector3 curPosition;
	Vector3 curDirection;
	Vector3 up;
	Vector3 right;

	//default positions
	Vector3 defaultPos;
	Vector3 defaultDir;
	Vector3 defaultUp;

	int jumpspeed;

	bool myKeys[255];

	//float pitchLimit;		//stop the camera from moving up a certain point
};

