#pragma once
#include "../Object/Actor/Actor.h"
class APawn :public AActor
{
public:
	typedef AActor Super;
	virtual void Init() override;
	virtual void BeginPaly() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw(float DeltaSeconds) override;
	virtual void Destroy() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent);
public:
	APawn();
	virtual	~APawn();
public:
	void AddActorMovementInput(const FVector& direction, float magnitude);
	// 上下观察
// 正rad值向上观察
// 负rad值向下观察
	void AddControllerYawInput(float Val);
	// 左右观察
	// 正rad值向右观察
	// 负rad值向左观察
	void AddControllerPitchInput(float Val);

	void AddControllerRollInput(float Val);
private:
	class UInputComponent* inputComponent;
	FRotator RotationInput;
	FRotator ControlInputVector;
};
