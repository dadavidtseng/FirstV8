//----------------------------------------------------------------------------------------------------
// Game.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Game.hpp"

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Resource/ResourceSubsystem.hpp"
#include "Engine/Resource/Resource/ModelResource.hpp"
#include "Engine/Resource/ResourceLoader/ObjModelLoader.hpp"
#include "Engine/Scripting/V8Subsystem.hpp"
#include "Game/Framework/App.hpp"
#include "Game/Framework/GameCommon.hpp"
#include "Game/Player.hpp"
#include "Game/Prop.hpp"

//----------------------------------------------------------------------------------------------------
Game::Game()
{
    SpawnPlayer();
    SpawnProp();

    m_screenCamera = new Camera();

    Vec2 const bottomLeft = Vec2::ZERO;
    // Vec2 const screenTopRight = Vec2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
    Vec2 clientDimensions = Window::s_mainWindow->GetClientDimensions();

    m_screenCamera->SetOrthoGraphicView(bottomLeft, clientDimensions);
    m_screenCamera->SetNormalizedViewport(AABB2::ZERO_TO_ONE);
    m_gameClock = new Clock(Clock::GetSystemClock());

    m_player->m_position     = Vec3(-2.f, 0.f, 1.f);
    m_firstCube->m_position  = Vec3(2.f, 2.f, 0.f);
    m_secondCube->m_position = Vec3(-2.f, -2.f, 0.f);
    m_sphere->m_position     = Vec3(10, -5, 1);
    m_grid->m_position       = Vec3::ZERO;

    DebugAddWorldBasis(Mat44(), -1.f);

    Mat44 transform;

    transform.SetIJKT3D(-Vec3::Y_BASIS, Vec3::X_BASIS, Vec3::Z_BASIS, Vec3(0.25f, 0.f, 0.25f));
    DebugAddWorldText("X-Forward", transform, 0.25f, Vec2::ONE, -1.f, Rgba8::RED);

    transform.SetIJKT3D(-Vec3::X_BASIS, -Vec3::Y_BASIS, Vec3::Z_BASIS, Vec3(0.f, 0.25f, 0.5f));
    DebugAddWorldText("Y-Left", transform, 0.25f, Vec2::ZERO, -1.f, Rgba8::GREEN);

    transform.SetIJKT3D(-Vec3::X_BASIS, Vec3::Z_BASIS, Vec3::Y_BASIS, Vec3(0.f, -0.25f, 0.25f));
    DebugAddWorldText("Z-Up", transform, 0.25f, Vec2(1.f, 0.f), -1.f, Rgba8::BLUE);


    // // 執行一些測試腳本
    // RunJavaScriptTests();
}

//----------------------------------------------------------------------------------------------------
Game::~Game()
{
    DebuggerPrintf("遊戲關閉中...\n");

    // 清理物件（保持原本的清理邏輯）
    for (Prop* prop : m_props)
    {
        delete prop;
    }
    m_props.clear();

    delete m_gameClock;
    m_gameClock = nullptr;

    delete m_grid;
    m_grid = nullptr;

    delete m_sphere;
    m_sphere = nullptr;

    delete m_secondCube;
    m_secondCube = nullptr;

    delete m_firstCube;
    m_firstCube = nullptr;

    delete m_player;
    m_player = nullptr;

    delete m_screenCamera;
    m_screenCamera = nullptr;

    delete m_worldCamera;
    m_worldCamera = nullptr;

    DebuggerPrintf("遊戲關閉完成。\n");
}

//----------------------------------------------------------------------------------------------------
void Game::Startup()
{
    DebuggerPrintf("遊戲啟動中...\n");

    // 建立遊戲物件
    m_gameClock = new Clock();

    // 建立相機
    m_screenCamera = new Camera();
    m_worldCamera  = new Camera();

    // 生成遊戲實體
    SpawnPlayer();
    SpawnProp();

    // 初始化 props 向量
    if (m_firstCube) m_props.push_back(m_firstCube);
    if (m_secondCube) m_props.push_back(m_secondCube);
    if (m_sphere) m_props.push_back(m_sphere);
    if (m_grid) m_props.push_back(m_grid);

    m_gameState = eGameState::GAME;

    DebuggerPrintf("遊戲啟動完成！\n");
}

void Game::Shutdown()
{
}

//----------------------------------------------------------------------------------------------------
void Game::Update()
{
    // 原本的更新邏輯
    float const gameDeltaSeconds   = static_cast<float>(m_gameClock->GetDeltaSeconds());
    float const systemDeltaSeconds = static_cast<float>(Clock::GetSystemClock().GetDeltaSeconds());

    UpdateEntities(gameDeltaSeconds, systemDeltaSeconds);
    UpdateFromKeyBoard();
    UpdateFromController();

    // 新增：JavaScript 相關更新
    HandleJavaScriptCommands();
    HandleConsoleCommands();

    // 新增：一次性 JavaScript 測試
    if (!m_hasRunJSTests && g_theV8Subsystem && g_theV8Subsystem->IsInitialized())
    {
        RunJavaScriptTests();
        m_hasRunJSTests = true;
    }
}

//----------------------------------------------------------------------------------------------------
void Game::Render() const
{
    //-Start-of-Game-Camera---------------------------------------------------------------------------

    g_theRenderer->BeginCamera(*m_player->GetCamera());

    if (m_gameState == eGameState::GAME)
    {
        RenderEntities();
        Vec2 screenDimensions = Window::s_mainWindow->GetScreenDimensions();
        Vec2 windowDimensions = Window::s_mainWindow->GetWindowDimensions();
        Vec2 clientDimensions = Window::s_mainWindow->GetClientDimensions();
        Vec2 windowPosition   = Window::s_mainWindow->GetWindowPosition();
        Vec2 clientPosition   = Window::s_mainWindow->GetClientPosition();
        DebugAddScreenText(Stringf("ScreenDimensions=(%.1f,%.1f)", screenDimensions.x, screenDimensions.y), Vec2(0, 0), 20.f, Vec2::ZERO, 0.f);
        DebugAddScreenText(Stringf("WindowDimensions=(%.1f,%.1f)", windowDimensions.x, windowDimensions.y), Vec2(0, 20), 20.f, Vec2::ZERO, 0.f);
        DebugAddScreenText(Stringf("ClientDimensions=(%.1f,%.1f)", clientDimensions.x, clientDimensions.y), Vec2(0, 40), 20.f, Vec2::ZERO, 0.f);
        DebugAddScreenText(Stringf("WindowPosition=(%.1f,%.1f)", windowPosition.x, windowPosition.y), Vec2(0, 60), 20.f, Vec2::ZERO, 0.f);
        DebugAddScreenText(Stringf("ClientPosition=(%.1f,%.1f)", clientPosition.x, clientPosition.y), Vec2(0, 80), 20.f, Vec2::ZERO, 0.f);
        // 新增：JavaScript 狀態顯示
        if (g_theV8Subsystem)
        {
            std::string jsStatus = g_theV8Subsystem->IsInitialized() ? "JS: 已啟用" : "JS: 未啟用";
            DebugAddScreenText(jsStatus, Vec2(0, 100), 20.f, Vec2::ZERO, 0.f);

            if (g_theV8Subsystem->HasError())
            {
                DebugAddScreenText("JS錯誤: " + g_theV8Subsystem->GetLastError(), Vec2(0, 120), 15.f, Vec2::ZERO, 0.f, Rgba8::RED);
            }
        }
    }

    g_theRenderer->EndCamera(*m_player->GetCamera());

    //-End-of-Game-Camera-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    if (m_gameState == eGameState::GAME)
    {
        DebugRenderWorld(*m_player->GetCamera());
    }
    //------------------------------------------------------------------------------------------------
    //-Start-of-Screen-Camera-------------------------------------------------------------------------

    g_theRenderer->BeginCamera(*m_screenCamera);

    if (m_gameState == eGameState::ATTRACT)
    {
        RenderAttractMode();
    }

    g_theRenderer->EndCamera(*m_screenCamera);

    //-End-of-Screen-Camera---------------------------------------------------------------------------
    if (m_gameState == eGameState::GAME)
    {
        DebugRenderScreen(*m_screenCamera);
    }
}

//----------------------------------------------------------------------------------------------------
bool Game::IsAttractMode() const
{
    return m_gameState == eGameState::ATTRACT;
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromKeyBoard()
{
    if (m_gameState == eGameState::ATTRACT)
    {
        if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
        {
            App::RequestQuit();
        }

        if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE))
        {
            m_gameState = eGameState::GAME;
        }
    }

    if (m_gameState == eGameState::GAME)
    {
        if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
        {
            m_gameState = eGameState::ATTRACT;
        }

        if (g_theInput->WasKeyJustPressed(KEYCODE_P))
        {
            m_gameClock->TogglePause();
        }

        if (g_theInput->WasKeyJustPressed(KEYCODE_O))
        {
            m_gameClock->StepSingleFrame();
        }

        if (g_theInput->IsKeyDown(KEYCODE_T))
        {
            m_gameClock->SetTimeScale(0.1f);
        }

        if (g_theInput->WasKeyJustReleased(KEYCODE_T))
        {
            m_gameClock->SetTimeScale(1.f);
        }

        if (g_theInput->WasKeyJustPressed(NUMCODE_1))
        {
            Vec3 forward;
            Vec3 right;
            Vec3 up;
            m_player->m_orientation.GetAsVectors_IFwd_JLeft_KUp(forward, right, up);

            DebugAddWorldLine(m_player->m_position, m_player->m_position + forward * 20.f, 0.01f, 10.f, Rgba8(255, 255, 0), Rgba8(255, 255, 0), eDebugRenderMode::X_RAY);
        }

        if (g_theInput->IsKeyDown(NUMCODE_2))
        {
            DebugAddWorldPoint(Vec3(m_player->m_position.x, m_player->m_position.y, 0.f), 0.25f, 60.f, Rgba8(150, 75, 0), Rgba8(150, 75, 0));
        }

        if (g_theInput->WasKeyJustPressed(NUMCODE_3))
        {
            Vec3 forward;
            Vec3 right;
            Vec3 up;
            m_player->m_orientation.GetAsVectors_IFwd_JLeft_KUp(forward, right, up);

            DebugAddWorldWireSphere(m_player->m_position + forward * 2.f, 1.f, 5.f, Rgba8::GREEN, Rgba8::RED);
        }

        if (g_theInput->WasKeyJustPressed(NUMCODE_4))
        {
            DebugAddWorldBasis(m_player->GetModelToWorldTransform(), 20.f);
        }

        if (g_theInput->WasKeyJustReleased(NUMCODE_5))
        {
            float const  positionX    = m_player->m_position.x;
            float const  positionY    = m_player->m_position.y;
            float const  positionZ    = m_player->m_position.z;
            float const  orientationX = m_player->m_orientation.m_yawDegrees;
            float const  orientationY = m_player->m_orientation.m_pitchDegrees;
            float const  orientationZ = m_player->m_orientation.m_rollDegrees;
            String const text         = Stringf("Position: (%.2f, %.2f, %.2f)\nOrientation: (%.2f, %.2f, %.2f)", positionX, positionY, positionZ, orientationX, orientationY, orientationZ);

            Vec3 forward;
            Vec3 right;
            Vec3 up;
            m_player->m_orientation.GetAsVectors_IFwd_JLeft_KUp(forward, right, up);

            DebugAddBillboardText(text, m_player->m_position + forward, 0.1f, Vec2::HALF, 10.f, Rgba8::WHITE, Rgba8::RED);
        }

        if (g_theInput->WasKeyJustPressed(NUMCODE_6))
        {
            DebugAddWorldCylinder(m_player->m_position, m_player->m_position + Vec3::Z_BASIS * 2, 1.f, 10.f, true, Rgba8::WHITE, Rgba8::RED);
        }


        if (g_theInput->WasKeyJustReleased(NUMCODE_7))
        {
            float const orientationX = m_player->GetCamera()->GetOrientation().m_yawDegrees;
            float const orientationY = m_player->GetCamera()->GetOrientation().m_pitchDegrees;
            float const orientationZ = m_player->GetCamera()->GetOrientation().m_rollDegrees;

            DebugAddMessage(Stringf("Camera Orientation: (%.2f, %.2f, %.2f)", orientationX, orientationY, orientationZ), 5.f);
        }

        DebugAddMessage(Stringf("Player Position: (%.2f, %.2f, %.2f)", m_player->m_position.x, m_player->m_position.y, m_player->m_position.z), 0.f);
    }
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromController()
{
    XboxController const& controller = g_theInput->GetController(0);

    if (m_gameState == eGameState::ATTRACT)
    {
        if (controller.WasButtonJustPressed(XBOX_BUTTON_BACK))
        {
            App::RequestQuit();
        }

        if (controller.WasButtonJustPressed(XBOX_BUTTON_START))
        {
            m_gameState = eGameState::GAME;
        }
    }

    if (m_gameState == eGameState::GAME)
    {
        if (controller.WasButtonJustPressed(XBOX_BUTTON_BACK))
        {
            m_gameState = eGameState::ATTRACT;
        }

        if (controller.WasButtonJustPressed(XBOX_BUTTON_B))
        {
            m_gameClock->TogglePause();
        }

        if (controller.WasButtonJustPressed(XBOX_BUTTON_Y))
        {
            m_gameClock->StepSingleFrame();
        }

        if (controller.WasButtonJustPressed(XBOX_BUTTON_X))
        {
            m_gameClock->SetTimeScale(0.1f);
        }

        if (controller.WasButtonJustReleased(XBOX_BUTTON_X))
        {
            m_gameClock->SetTimeScale(1.f);
        }
    }
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateEntities(float const gameDeltaSeconds, float const systemDeltaSeconds) const
{
    // 更新玩家
    if (m_player)
    {
        m_player->Update(gameDeltaSeconds);
    }

    // 更新所有物件
    for (Prop* prop : m_props)
    {
        if (prop)
        {
            prop->Update(gameDeltaSeconds);
        }
    }
    // m_player->Update(systemDeltaSeconds);
    // m_firstCube->Update(gameDeltaSeconds);
    // m_secondCube->Update(gameDeltaSeconds);
    // m_sphere->Update(gameDeltaSeconds);
    // m_grid->Update(gameDeltaSeconds);

    m_firstCube->m_orientation.m_pitchDegrees += 30.f * gameDeltaSeconds;
    m_firstCube->m_orientation.m_rollDegrees += 30.f * gameDeltaSeconds;

    float const time       = static_cast<float>(m_gameClock->GetTotalSeconds());
    float const colorValue = (sinf(time) + 1.0f) * 0.5f * 255.0f;

    m_secondCube->m_color.r = static_cast<unsigned char>(colorValue);
    m_secondCube->m_color.g = static_cast<unsigned char>(colorValue);
    m_secondCube->m_color.b = static_cast<unsigned char>(colorValue);

    m_sphere->m_orientation.m_yawDegrees += 45.f * gameDeltaSeconds;

    DebugAddScreenText(Stringf("Time: %.2f\nFPS: %.2f\nScale: %.1f", m_gameClock->GetTotalSeconds(), 1.f / m_gameClock->GetDeltaSeconds(), m_gameClock->GetTimeScale()), m_screenCamera->GetOrthographicTopRight() - Vec2(250.f, 60.f), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
}

//----------------------------------------------------------------------------------------------------
void Game::RenderAttractMode() const
{
    Vec2 clientDimensions = Window::s_mainWindow->GetClientDimensions();

    VertexList_PCU verts;
    AddVertsForDisc2D(verts, Vec2(clientDimensions.x * 0.5f, clientDimensions.y * 0.5f), 300.f, 10.f, Rgba8::YELLOW);
    g_theRenderer->SetModelConstants();
    g_theRenderer->SetBlendMode(eBlendMode::OPAQUE);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts);
}

//----------------------------------------------------------------------------------------------------
void Game::RenderEntities() const
{
    m_firstCube->Render();
    m_secondCube->Render();
    m_sphere->Render();
    m_grid->Render();

    g_theRenderer->SetModelConstants(m_player->GetModelToWorldTransform());
    m_player->Render();

    for (Prop* prop : m_props)
    {
        prop->Render();
    }
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnPlayer()
{
    m_player = new Player(this);
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnProp()
{
    Texture const* texture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/TestUV.png");

    m_firstCube  = new Prop(this);
    m_secondCube = new Prop(this);
    m_sphere     = new Prop(this, texture);
    m_grid       = new Prop(this);

    m_firstCube->InitializeLocalVertsForCube();
    m_secondCube->InitializeLocalVertsForCube();
    m_sphere->InitializeLocalVertsForSphere();
    m_grid->InitializeLocalVertsForGrid();
}


//----------------------------------------------------------------------------------------------------
// 新增的 JavaScript 相關方法
//----------------------------------------------------------------------------------------------------

void Game::ExecuteJavaScriptCommand(const std::string& command)
{
    if (g_theV8Subsystem && g_theV8Subsystem->IsInitialized())
    {
        DebuggerPrintf("執行 JS 指令: %s\n", command.c_str());
        bool success = g_theV8Subsystem->ExecuteScript(command);

        if (!success)
        {
            DebuggerPrintf("JavaScript 指令執行失敗！\n");
            if (g_theV8Subsystem->HasError())
            {
                DebuggerPrintf("錯誤: %s\n", g_theV8Subsystem->GetLastError().c_str());
            }
        }
        else
        {
            std::string result = g_theV8Subsystem->GetLastResult();
            if (!result.empty())
            {
                DebuggerPrintf("JS 結果: %s\n", result.c_str());
            }
        }
    }
    else
    {
        DebuggerPrintf("V8Subsystem 不可用，無法執行 JS 指令: %s\n", command.c_str());
    }
}

//----------------------------------------------------------------------------------------------------
void Game::ExecuteJavaScriptFile(const std::string& filename)
{
    if (g_theV8Subsystem && g_theV8Subsystem->IsInitialized())
    {
        DebuggerPrintf("執行 JS 檔案: %s\n", filename.c_str());
        bool success = g_theV8Subsystem->ExecuteScriptFile(filename);

        if (!success)
        {
            DebuggerPrintf("JavaScript 檔案執行失敗: %s\n", filename.c_str());
            if (g_theV8Subsystem->HasError())
            {
                DebuggerPrintf("錯誤: %s\n", g_theV8Subsystem->GetLastError().c_str());
            }
        }
    }
    else
    {
        DebuggerPrintf("V8Subsystem 不可用，無法執行 JS 檔案: %s\n", filename.c_str());
    }
}

//----------------------------------------------------------------------------------------------------
void Game::HandleJavaScriptCommands()
{
    // 處理動態 JavaScript 指令（例如從網路、檔案或其他來源）
    // 這裡可以加入定期檢查 JavaScript 指令的邏輯

    // 範例：檢查特定按鍵來執行預設腳本
    if (g_theInput->WasKeyJustPressed('J'))
    {
        // ExecuteJavaScriptCommand("console.log('J 鍵觸發的 JavaScript!');");
        ExecuteJavaScriptFile("Data/Scripts/test_scripts.js");
    }

    if (g_theInput->WasKeyJustPressed('K'))
    {
        ExecuteJavaScriptCommand("Game.createCube(Math.random() * 10 - 5, 0, Math.random() * 10 - 5);");
        // CreateCube(Vec3::ZERO);
    }

    if (g_theInput->WasKeyJustPressed('L'))
    {
        ExecuteJavaScriptCommand("var pos = Game.getPlayerPos(); console.log('玩家位置:', pos);");
    }
}

//----------------------------------------------------------------------------------------------------
void Game::CreateCube(Vec3 const& position)
{
    DebuggerPrintf("JavaScript 請求建立方塊在位置 (%.2f, %.2f, %.2f)\n", position.x, position.y, position.z);

    // 建立新的方塊物件
    Prop* newCube       = new Prop(this);
    newCube->m_position = position;
    newCube->m_color    = Rgba8(
        static_cast<unsigned char>(g_theRNG->RollRandomIntInRange(100, 255)),
        static_cast<unsigned char>(g_theRNG->RollRandomIntInRange(100, 255)),
        static_cast<unsigned char>(g_theRNG->RollRandomIntInRange(100, 255)),
        255
    );
    newCube->InitializeLocalVertsForCube();

    // 加入到物件清單
    m_props.push_back(newCube);

    DebuggerPrintf("方塊建立成功！目前共有 %zu 個物件\n", m_props.size());
}

//----------------------------------------------------------------------------------------------------
void Game::MoveProp(int propIndex, Vec3 const& newPosition)
{
    if (propIndex >= 0 && propIndex < static_cast<int>(m_props.size()))
    {
        m_props[propIndex]->m_position = newPosition;
        DebuggerPrintf("物件 %d 移動到位置 (%.2f, %.2f, %.2f)\n", propIndex, newPosition.x, newPosition.y, newPosition.z);
    }
    else
    {
        DebuggerPrintf("警告：JavaScript 請求移動無效的物件索引 %d（總共 %zu 個物件）\n", propIndex, m_props.size());
    }
}

//----------------------------------------------------------------------------------------------------
Player* Game::GetPlayer()
{
    return m_player;
}

//----------------------------------------------------------------------------------------------------
void Game::HandleConsoleCommands()
{
    // 處理開發者控制台的 JavaScript 指令
    // 這需要與 DevConsole 整合

    if (g_theDevConsole && g_theDevConsole->IsOpen())
    {
        // 檢查控制台輸入是否為 JavaScript 指令
        // 這裡需要實作具體的控制台輸入檢查邏輯

        // 範例實作（需要 DevConsole 支援）:
        /*
        std::string input = g_theConsole->GetLastInput();
        if (input.substr(0, 3) == "js:")
        {
            std::string jsCommand = input.substr(3);
            ExecuteJavaScriptCommand(jsCommand);
        }
        else if (input.substr(0, 7) == "jsfile:")
        {
            std::string filename = input.substr(7);
            ExecuteJavaScriptFile(filename);
        }
        */
    }
}

//----------------------------------------------------------------------------------------------------
void Game::RunJavaScriptTests()
{
    DebuggerPrintf("開始執行 JavaScript 測試...\n");

    // 基本功能測試
    ExecuteJavaScriptCommand("console.log('=== JavaScript 功能測試開始 ===');");

    // 數學運算測試
    ExecuteJavaScriptCommand("var result = 10 + 5 * 2; console.log('數學測試: 10 + 5 * 2 =', result);");

    // 遊戲物件互動測試
    ExecuteJavaScriptCommand("console.log('取得玩家位置:', Game.getPlayerPos());");

    // 建立物件測試
    ExecuteJavaScriptCommand("Game.createCube(3, 0, 3); console.log('已建立測試方塊');");

    // 移動物件測試（移動第一個物件）
    if (!m_props.empty())
    {
        ExecuteJavaScriptCommand("Game.moveProp(0, 2, 1, 2); console.log('已移動第一個物件');");
    }

    // 複雜腳本測試
    ExecuteJavaScriptCommand(R"(
        for(var i = 0; i < 3; i++) {
            Game.createCube(i * 2, 0, 5);
            console.log('建立方塊', i + 1);
        }
        console.log('=== JavaScript 功能測試完成 ===');
    )");

    DebuggerPrintf("JavaScript 測試執行完成！\n");
}
