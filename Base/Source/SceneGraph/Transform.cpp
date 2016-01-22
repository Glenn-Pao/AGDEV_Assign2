#include "Transform.h"

#include <iostream>
using namespace std;

CTransform::CTransform(void)
{
	Mtx.SetToZero();
	Mtx.SetToIdentity();
	Update_Mtx.SetToZero();
	Update_Mtx.SetToIdentity();
}

CTransform::CTransform( const float dx, const float dy, const float dz )
{
	Mtx.SetToTranslation( dx, dy, dz );
}

CTransform::~CTransform(void)
{
}

void CTransform::PreRendering()
{
}

void CTransform::Draw(void)
{
}

void CTransform::PostRendering()
{
}

void CTransform::SetTranslate( const float dx, const float dy, const float dz  )
{
	Mtx44 TempMtx;
	TempMtx.SetToTranslation( dx, dy, dz );
	Mtx = Mtx * TempMtx;
}

void CTransform::SetRotate( const float angle, const float rx, const float ry, const float rz  )
{
	Mtx44 TempMtx;
	TempMtx.SetToRotation( angle, rx, ry, rz );
	
	Mtx44 MtxBackToOrigin;

	MtxBackToOrigin.a[ 12 ] = -Mtx.a[ 12 ];
	MtxBackToOrigin.a[ 13 ] = -Mtx.a[ 13 ];
	MtxBackToOrigin.a[ 14 ] = -Mtx.a[ 14 ];

	Mtx44 MtxBackToPosition;
	MtxBackToPosition.a[ 12 ] = Mtx.a[ 12 ];
	MtxBackToPosition.a[ 13 ] = Mtx.a[ 13 ];
	MtxBackToPosition.a[ 14 ] = Mtx.a[ 14 ];

	Mtx = Mtx * MtxBackToOrigin;
	Mtx = Mtx * TempMtx;
	Mtx = Mtx * MtxBackToPosition;
}

void CTransform::SetScale( const float sx, const float sy, const float sz  )
{
	Mtx.SetToScale( sx, sy, sz );
}

void CTransform::GetOffset( float& x, float& y, float& z )
{
	x = Mtx.a[ 12 ];
	y = Mtx.a[ 13 ];
	z = Mtx.a[ 14 ];
}

// Get the transformation matrix
Mtx44 CTransform::GetTransform(void)
{
	return Mtx;
}

//void CTransform::PrintSelf(void)
//{
//	cout << "CTransform::PrintSelf(void)" << endl;
//	cout << "===========================" << endl;
//	Mtx.PrintSelf();
//}