#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Object/Object/GameInstance/GameProjectSetting.h"
class UGameSetting :public UGameProjectSetting
{
public:
	typedef UGameProjectSetting Super;
	UGameSetting();
	virtual~UGameSetting();
public:
	virtual void initializationProjectSetting();
};