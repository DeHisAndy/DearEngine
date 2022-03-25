#pragma once
#include "../Object/Actor/Actor.h"
class AGameMode :public AActor
{
public:
	typedef AActor Super;
public:
	AGameMode();
	AGameMode(class APlayerController* playController, class APawn* pawn);
	virtual~AGameMode();
	virtual void Init() override;
	virtual void BeginPaly() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw( ) override;
	virtual void Destroy() override;
public:
	void SetPawn(class APawn* pawn);
	void SetPlayerController(class APlayerController* playController);
public:
	class APawn* GetPawn();
	class APlayerController* GetPlayerController();
private:
	class APawn* pawn;
	class APlayerController* playController;
};
