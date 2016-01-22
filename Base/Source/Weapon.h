#pragma once
class CWeapon
{
private:
	int iRounds;
	int iTotalRounds;
	int iClipSize;

	float fFiring;
	float fReload;

	bool bActive;
	bool bReload;
	bool bSwap;
	bool bIsFiring;
	bool bEmpty;
public:
	CWeapon(void);
	~CWeapon(void);
	
	void Init(bool bActive, int iRounds, int iClipSize, int iTotalRounds, bool bSwap, bool bReload, float fReload, bool bEmpty, float fFiring, bool bIsFiring);

	void setActiveWeapon(bool bActive);	
	bool getActiveWeapon(void);					//is it active?

	void setActiveRounds(int iRounds);			//number of rounds available in the clip of weapon
	int getActiveRounds(void);					//get the remaining number of rounds

	void setTotalRounds(int iTotalRounds);	//number of rounds available all the clips of weapon
	int getTotalRounds(void);				//get the remaining number of rounds

	void setSwap(bool bSwap);				//is weapon swapable?
	bool getSwap(void);						//get status of weapon swapability

	void setNeedReload(bool bReload);			//need to reload?
	bool getNeedReload(void);					//get the status of reload

	void setReload(float fReload);			//set the reload time
	float getReload(void);					//get the reload time, for display if needed

	void setIsEmpty(bool bEmpty);			//is the weapon empty?
	bool getIsEmpty(void);					//if true, the weapon is empty

	void setFiringRate(float fFiring);		//applies only to machine gun. pistol doesnt have this
	float getFiringRate(void);

	void setIsFiring(bool bFiring);			//control the firing rate
	float getIsFiring(void);				

	void setClipSize(int iClipSize);
	int getClipSize(void);
};

