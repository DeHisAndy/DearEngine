#pragma once

class UGame 
{
public:
	virtual	~UGame();
	static UGame* Get();
	static UGame* Instance;
public:
	virtual void RenderThread();
	virtual void Init();
	virtual void BeginPaly();
	virtual void OnResize();
	virtual void UpdateScene(float DeltaSeconds);
	virtual void Draw(float DeltaSeconds);
	virtual void Destroy();
public:
	void SetGameInstance(class UGameInstance* instance);
	class UGameInstance* GetGameInstance();
	class FRender* GetRenderThread();
	class UGameProjectSetting* GetGameProjectSetting();
	void SetGameSetting(class UGameProjectSetting* set);

private:
	UGame();
private:
	class UGameInstance* U_GameInstance;
	class FRender* F_Render;
	class UGameProjectSetting* U_GameSetting;
};
