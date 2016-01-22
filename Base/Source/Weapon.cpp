#include "Weapon.h"


CWeapon::CWeapon(void)
{
	iRounds = 0;
	iTotalRounds = 0;
	iClipSize = 0;

	fReload = 0.f;

	bActive = false;
	bReload = false;
	bSwap = true;
	bEmpty = false;
	bIsFiring = false;
}

CWeapon::~CWeapon(void)
{
}
void CWeapon::Init(bool bActive, int iRounds, int iClipSize, int iTotalRounds, bool bSwap, bool bReload, float fReload, bool bEmpty, float fFiring, bool bIsFiring)
{
	this->bActive = bActive;
	this->iRounds = iRounds;
	this->iClipSize = iClipSize;
	this->iTotalRounds = iTotalRounds;
	this->bSwap = bSwap;
	this->bReload = bReload;
	this->fReload = fReload;
	this->bEmpty = bEmpty;
	this->fFiring = fFiring;
	this->bIsFiring = bIsFiring;
}
void CWeapon::setActiveWeapon(bool bActive)
{
	this->bActive = bActive;
}
bool CWeapon::getActiveWeapon(void)
{
	return bActive;
}
//number of rounds available in the clip of weapon
void CWeapon::setActiveRounds(int iRounds)
{
	this->iRounds = iRounds;
}
//get the remaining number of rounds
int CWeapon::getActiveRounds(void)
{
	return iRounds;
}
//number of rounds available all the clips of weapon
void CWeapon::setTotalRounds(int iTotalRounds)
{
	this->iTotalRounds = iTotalRounds;
}
//get the remaining number of rounds
int CWeapon::getTotalRounds(void)
{
	return iTotalRounds;
}
void CWeapon::setNeedReload(bool bReload)
{
	this->bReload = bReload;
}
bool CWeapon::getNeedReload()
{
	return bReload;
}
void CWeapon::setIsEmpty(bool bEmpty)
{
	this->bEmpty = bEmpty;
}
bool CWeapon::getIsEmpty(void)
{
	return bEmpty;
}
//is weapon swapable?
void CWeapon::setSwap(bool bSwap)
{
	this->bSwap = bSwap;
}
//get status of weapon swapability
bool CWeapon::getSwap(void)
{
	return bSwap;
}
//set the reload time
void CWeapon::setReload(float fReload)
{
	this->fReload = fReload;
}
//get the reload time, for display if needed
float CWeapon::getReload(void)
{
	return fReload;
}
void CWeapon::setFiringRate(float fFiring)
{
	this->fFiring = fFiring;
}//applies only to machine gun. pistol doesnt have this
float CWeapon::getFiringRate(void)
{
	return fFiring;
}
void CWeapon::setIsFiring(bool bIsFiring)
{
	this->bIsFiring = bIsFiring;
}
float CWeapon::getIsFiring(void)
{
	return bIsFiring;
}
void CWeapon::setClipSize(int iClipSize)
{
	this->iClipSize = iClipSize;
}
int CWeapon::getClipSize(void)
{
	return iClipSize;
}