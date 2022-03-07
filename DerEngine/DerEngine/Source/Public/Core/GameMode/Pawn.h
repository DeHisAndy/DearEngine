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
	// ���¹۲�
// ��radֵ���Ϲ۲�
// ��radֵ���¹۲�
	void AddControllerYawInput(float Val);
	// ���ҹ۲�
	// ��radֵ���ҹ۲�
	// ��radֵ����۲�
	void AddControllerPitchInput(float Val);

	void AddControllerRollInput(float Val);
private:
	class UInputComponent* inputComponent;
	FRotator RotationInput;
	FRotator ControlInputVector;
};
