//----------------------------------------------------------------------------------------------------
// Game.hpp - 整合 V8Subsystem 後的版本
//----------------------------------------------------------------------------------------------------

#pragma once
#include "Engine/Renderer/VertexUtils.hpp"
#include "Engine/Renderer/Vertex_PCUTBN.hpp"
#include "Engine/Resource/ResourceHandle.hpp"
#include <vector>
#include <string>

struct Vertex_PCUTBN;
class ModelResource;

//----------------------------------------------------------------------------------------------------
class Camera;
class Clock;
class Player;
class Prop;

//----------------------------------------------------------------------------------------------------
enum class eGameState : uint8_t
{
    ATTRACT,
    GAME
};

//----------------------------------------------------------------------------------------------------
class Game
{
public:
    Game();
    ~Game();

    void Update();
    void Render() const;
    bool IsAttractMode() const;

    // 新增：JavaScript 相關功能
    void ExecuteJavaScriptCommand(const std::string& command);
    void ExecuteJavaScriptFile(const std::string& filename);
    void HandleJavaScriptCommands();
    
    // 新增：JavaScript 回呼函數需要的遊戲功能
    void    CreateCube(const Vec3& position);
    void    MoveProp(int propIndex, const Vec3& newPosition);
    void    MovePlayerCamera(const Vec3& offset);  // 新增：移動玩家相機
    Player* GetPlayer();

    // 新增：控制台命令處理
    void HandleConsoleCommands();

private:
    void UpdateFromKeyBoard();
    void UpdateFromController();
    void UpdateEntities(float gameDeltaSeconds, float systemDeltaSeconds) const;
    void RenderAttractMode() const;
    void RenderEntities() const;

    void SpawnPlayer();
    void SpawnProp();

    // 新增：JavaScript 測試和除錯
    void RunJavaScriptTests();
    void SetupJavaScriptBindings();

    Camera*    m_screenCamera = nullptr;
    Player*    m_player       = nullptr;
    Prop*      m_firstCube    = nullptr;
    Prop*      m_secondCube   = nullptr;
    Prop*      m_sphere       = nullptr;
    Prop*      m_grid         = nullptr;
    Clock*     m_gameClock    = nullptr;
    eGameState m_gameState    = eGameState::ATTRACT;

    // 新增：物件管理
    std::vector<Prop*> m_props;  // 用於 JavaScript 管理的物件清單

    // 新增：JavaScript 狀態
    bool m_hasInitializedJS = false;
    bool m_hasRunJSTests    = false;

    Vec3 m_originalPlayerPosition = Vec3(-2.f, 0.f, 1.f);  // 儲存原始位置
    bool m_cameraShakeActive = false;                        // 追蹤震動狀態
};
