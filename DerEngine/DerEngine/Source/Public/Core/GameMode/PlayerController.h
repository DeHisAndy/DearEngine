#pragma once
#include "../Object/Actor/Actor.h"
class APlayerController :public AActor
{
public:
	typedef AActor Super;
	virtual void Init() override;
	virtual void BeginPaly() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw(float DeltaSeconds) override;
	virtual void Destroy() override;
public:
	APlayerController();
	virtual~APlayerController();
};
