#pragma once
#include "Pawn.h"

class ADearPawn :public APawn
{
public:
	typedef APawn Super;
	virtual void Init() override;
	virtual void BeginPaly() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw(float DeltaSeconds) override;
	virtual void Destroy() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent);
public:
	ADearPawn();
	virtual	~ADearPawn();
};
