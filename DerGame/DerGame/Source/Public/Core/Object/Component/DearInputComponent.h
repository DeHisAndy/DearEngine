#pragma once
#include "..\..\..\..\..\..\..\DerEngine\DerEngine\Source\Public\Core\Object\Object\Component\InputComponent.h"

class UDearInputComponent :public UInputComponent
{
public:
	typedef UInputComponent Super;
	UDearInputComponent();
	virtual ~UDearInputComponent();
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginDestroy() override;
public:
public:
	void Key_W();
	void Key_A();
	void Key_S();
	void Key_D();
	void Key_Q();
	void Key_E();
	void Key_ESC();
	void MousePre();
};