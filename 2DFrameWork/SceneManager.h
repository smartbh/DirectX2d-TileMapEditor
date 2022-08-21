#pragma once
class SceneManager :public Singleton<SceneManager>
{
private:
    bool                    isChanging = false;
    Scene *                 nextScene = nullptr; //다음 씬
    Scene *                 currentScene = nullptr; //현재 씬
    map<string, Scene*>     scenes; //저장시킬 장면들
    float                   changingTime;

public:
    ~SceneManager();

    //씬 추가
    bool    AddScene(string key, Scene * value);
    //씬 삭제
    bool    DeleteScene(string key);
    //씬 변경
    Scene * ChangeScene(string key, float changingTime = 0.0f);
    //씬 가져오기
    Scene * GetScene(string key);
    //현재 씬 가져오기
    Scene * GetCurrentScene();

    void    Release();
    void    Update();
    void    LateUpdate();
    void    Render();
    void    ResizeScreen();
};

