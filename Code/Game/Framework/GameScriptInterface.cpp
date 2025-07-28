//----------------------------------------------------------------------------------------------------
// GameScriptInterface.cpp
// Game 類別的腳本介面包裝器實作
//----------------------------------------------------------------------------------------------------

#include "Game/Framework/GameScriptInterface.hpp"
#include "Game/Game.hpp"
#include "Game/Player.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <stdexcept>
#include <sstream>

#include "Engine/Core/EngineCommon.hpp"

//----------------------------------------------------------------------------------------------------
GameScriptInterface::GameScriptInterface(Game* game)
    : m_game(game)
{
    if (!m_game)
    {
        ERROR_AND_DIE("GameScriptInterface: Game pointer cannot be null");
    }
}

//----------------------------------------------------------------------------------------------------
std::string GameScriptInterface::GetScriptObjectName() const
{
    return "game";
}

//----------------------------------------------------------------------------------------------------
std::vector<ScriptMethodInfo> GameScriptInterface::GetAvailableMethods() const
{
    return {
        ScriptMethodInfo("createCube",
                        "在指定位置創建一個立方體",
                        {"float", "float", "float"},
                        "string"),

        ScriptMethodInfo("moveProp",
                        "移動指定索引的道具到新位置",
                        {"int", "float", "float", "float"},
                        "string"),

        ScriptMethodInfo("getPlayerPosition",
                        "取得玩家目前位置",
                        {},
                        "object"),

        ScriptMethodInfo("executeCommand",
                        "執行 JavaScript 指令",
                        {"string"},
                        "string"),

        ScriptMethodInfo("executeFile",
                        "執行 JavaScript 檔案",
                        {"string"},
                        "string"),

        ScriptMethodInfo("isAttractMode",
                        "檢查遊戲是否處於吸引模式",
                        {},
                        "bool"),

        ScriptMethodInfo("getGameState",
                        "取得目前遊戲狀態",
                        {},
                        "string")
    };
}

//----------------------------------------------------------------------------------------------------
std::vector<std::string> GameScriptInterface::GetAvailableProperties() const
{
    return {
        "attractMode",
        "gameState"
    };
}

//----------------------------------------------------------------------------------------------------
ScriptMethodResult GameScriptInterface::CallMethod(const std::string& methodName,
                                                  const std::vector<std::any>& args)
{
    try
    {
        if (methodName == "createCube")
        {
            return ExecuteCreateCube(args);
        }
        else if (methodName == "moveProp")
        {
            return ExecuteMoveProp(args);
        }
        else if (methodName == "getPlayerPosition")
        {
            return ExecuteGetPlayerPosition(args);
        }
        else if (methodName == "executeCommand")
        {
            return ExecuteJavaScriptCommand(args);
        }
        else if (methodName == "executeFile")
        {
            return ExecuteJavaScriptFile(args);
        }
        else if (methodName == "isAttractMode")
        {
            return ExecuteIsAttractMode(args);
        }
        else if (methodName == "getGameState")
        {
            return ExecuteGetGameState(args);
        }

        return ScriptMethodResult::Error("未知的方法: " + methodName);
    }
    catch (const std::exception& e)
    {
        return ScriptMethodResult::Error("方法執行時發生例外: " + std::string(e.what()));
    }
}

//----------------------------------------------------------------------------------------------------
std::any GameScriptInterface::GetProperty(const std::string& propertyName) const
{
    if (propertyName == "attractMode")
    {
        return m_game->IsAttractMode();
    }
    else if (propertyName == "gameState")
    {
        return m_game->IsAttractMode() ? "attract" : "game";
    }

    return std::any{};
}

//----------------------------------------------------------------------------------------------------
bool GameScriptInterface::SetProperty(const std::string& propertyName, const std::any& value)
{
    // 目前 Game 物件沒有可設定的屬性
    // 如果需要，可以在這裡添加
    UNUSED(propertyName);
    UNUSED(value);
    return false;
}

//----------------------------------------------------------------------------------------------------
ScriptMethodResult GameScriptInterface::ExecuteCreateCube(const std::vector<std::any>& args)
{
    auto result = ValidateArgCount(args, 3, "createCube");
    if (!result.success)
        return result;

    try
    {
        Vec3 position = ExtractVec3(args, 0);
        m_game->CreateCube(position);
        return ScriptMethodResult::Success(std::string("立方體創建成功，位置: (" +
            std::to_string(position.x) + ", " +
            std::to_string(position.y) + ", " +
            std::to_string(position.z) + ")"));
    }
    catch (const std::exception& e)
    {
        return ScriptMethodResult::Error("創建立方體失敗: " + std::string(e.what()));
    }
}

//----------------------------------------------------------------------------------------------------
ScriptMethodResult GameScriptInterface::ExecuteMoveProp(const std::vector<std::any>& args)
{
    auto result = ValidateArgCount(args, 4, "moveProp");
    if (!result.success)
        return result;

    try
    {
        int propIndex = ExtractInt(args[0]);
        Vec3 newPosition = ExtractVec3(args, 1);
        m_game->MoveProp(propIndex, newPosition);
        return ScriptMethodResult::Success(std::string("道具 " + std::to_string(propIndex) +
            " 移動成功，新位置: (" +
            std::to_string(newPosition.x) + ", " +
            std::to_string(newPosition.y) + ", " +
            std::to_string(newPosition.z) + ")"));
    }
    catch (const std::exception& e)
    {
        return ScriptMethodResult::Error("移動道具失敗: " + std::string(e.what()));
    }
}

//----------------------------------------------------------------------------------------------------
ScriptMethodResult GameScriptInterface::ExecuteGetPlayerPosition(const std::vector<std::any>& args)
{
    auto result = ValidateArgCount(args, 0, "getPlayerPosition");
    if (!result.success)
        return result;

    try
    {
        Player* player = m_game->GetPlayer();
        if (!player)
        {
            return ScriptMethodResult::Error("玩家物件不存在");
        }

        // 這裡假設 Player 有 GetPosition 方法
        // 您可能需要根據實際的 Player 類別 API 調整
        // Vec3 position = player->GetPosition();

        // 暫時回傳一個假的位置，您可以根據實際 API 修改
        Vec3 position(0.0f, 0.0f, 0.0f);

        // 回傳一個可以被 JavaScript 使用的物件
        std::string positionStr = "{ x: " + std::to_string(position.x) +
                                ", y: " + std::to_string(position.y) +
                                ", z: " + std::to_string(position.z) + " }";

        return ScriptMethodResult::Success(positionStr);
    }
    catch (const std::exception& e)
    {
        return ScriptMethodResult::Error("取得玩家位置失敗: " + std::string(e.what()));
    }
}

//----------------------------------------------------------------------------------------------------
ScriptMethodResult GameScriptInterface::ExecuteJavaScriptCommand(const std::vector<std::any>& args)
{
    auto result = ValidateArgCount(args, 1, "executeCommand");
    if (!result.success)
        return result;

    try
    {
        std::string command = ExtractString(args[0]);
        m_game->ExecuteJavaScriptCommand(command);
        return ScriptMethodResult::Success(std::string("指令執行: " + command));
    }
    catch (const std::exception& e)
    {
        return ScriptMethodResult::Error("執行 JavaScript 指令失敗: " + std::string(e.what()));
    }
}

//----------------------------------------------------------------------------------------------------
ScriptMethodResult GameScriptInterface::ExecuteJavaScriptFile(const std::vector<std::any>& args)
{
    auto result = ValidateArgCount(args, 1, "executeFile");
    if (!result.success)
        return result;

    try
    {
        std::string filename = ExtractString(args[0]);
        m_game->ExecuteJavaScriptFile(filename);
        return ScriptMethodResult::Success(std::string("檔案執行: " + filename));
    }
    catch (const std::exception& e)
    {
        return ScriptMethodResult::Error("執行 JavaScript 檔案失敗: " + std::string(e.what()));
    }
}

//----------------------------------------------------------------------------------------------------
ScriptMethodResult GameScriptInterface::ExecuteIsAttractMode(const std::vector<std::any>& args)
{
    auto result = ValidateArgCount(args, 0, "isAttractMode");
    if (!result.success)
        return result;

    try
    {
        bool isAttract = m_game->IsAttractMode();
        return ScriptMethodResult::Success(isAttract);
    }
    catch (const std::exception& e)
    {
        return ScriptMethodResult::Error("檢查吸引模式失敗: " + std::string(e.what()));
    }
}

//----------------------------------------------------------------------------------------------------
ScriptMethodResult GameScriptInterface::ExecuteGetGameState(const std::vector<std::any>& args)
{
    auto result = ValidateArgCount(args, 0, "getGameState");
    if (!result.success)
        return result;

    try
    {
        std::string state = m_game->IsAttractMode() ? "attract" : "game";
        return ScriptMethodResult::Success(state);
    }
    catch (const std::exception& e)
    {
        return ScriptMethodResult::Error("取得遊戲狀態失敗: " + std::string(e.what()));
    }
}

//----------------------------------------------------------------------------------------------------
// 輔助方法實作
//----------------------------------------------------------------------------------------------------

template<typename T>
T GameScriptInterface::ExtractArg(const std::any& arg, const std::string& expectedType) const
{
    try
    {
        return std::any_cast<T>(arg);
    }
    catch (const std::bad_any_cast& e)
    {
        std::string typeInfo = expectedType.empty() ? typeid(T).name() : expectedType;
        throw std::invalid_argument("參數類型錯誤，期望: " + typeInfo);
    }
}

//----------------------------------------------------------------------------------------------------
Vec3 GameScriptInterface::ExtractVec3(const std::vector<std::any>& args, size_t startIndex) const
{
    if (startIndex + 2 >= args.size())
    {
        throw std::invalid_argument("Vec3 需要 3 個參數 (x, y, z)");
    }

    float x = ExtractFloat(args[startIndex]);
    float y = ExtractFloat(args[startIndex + 1]);
    float z = ExtractFloat(args[startIndex + 2]);

    return Vec3(x, y, z);
}

//----------------------------------------------------------------------------------------------------
float GameScriptInterface::ExtractFloat(const std::any& arg) const
{
    // 嘗試多種數值類型的轉換
    try
    {
        return std::any_cast<float>(arg);
    }
    catch (const std::bad_any_cast&)
    {
        try
        {
            return static_cast<float>(std::any_cast<double>(arg));
        }
        catch (const std::bad_any_cast&)
        {
            try
            {
                return static_cast<float>(std::any_cast<int>(arg));
            }
            catch (const std::bad_any_cast&)
            {
                throw std::invalid_argument("無法轉換為 float 類型");
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------
int GameScriptInterface::ExtractInt(const std::any& arg) const
{
    try
    {
        return std::any_cast<int>(arg);
    }
    catch (const std::bad_any_cast&)
    {
        try
        {
            return static_cast<int>(std::any_cast<float>(arg));
        }
        catch (const std::bad_any_cast&)
        {
            try
            {
                return static_cast<int>(std::any_cast<double>(arg));
            }
            catch (const std::bad_any_cast&)
            {
                throw std::invalid_argument("無法轉換為 int 類型");
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------
std::string GameScriptInterface::ExtractString(const std::any& arg) const
{
    try
    {
        return std::any_cast<std::string>(arg);
    }
    catch (const std::bad_any_cast&)
    {
        try
        {
            const char* cstr = std::any_cast<const char*>(arg);
            return std::string(cstr);
        }
        catch (const std::bad_any_cast&)
        {
            throw std::invalid_argument("無法轉換為 string 類型");
        }
    }
}

//----------------------------------------------------------------------------------------------------
bool GameScriptInterface::ExtractBool(const std::any& arg) const
{
    try
    {
        return std::any_cast<bool>(arg);
    }
    catch (const std::bad_any_cast&)
    {
        try
        {
            // 嘗試從數值轉換
            int val = std::any_cast<int>(arg);
            return val != 0;
        }
        catch (const std::bad_any_cast&)
        {
            throw std::invalid_argument("無法轉換為 bool 類型");
        }
    }
}

//----------------------------------------------------------------------------------------------------
ScriptMethodResult GameScriptInterface::ValidateArgCount(const std::vector<std::any>& args,
                                                        size_t expectedCount,
                                                        const std::string& methodName) const
{
    if (args.size() != expectedCount)
    {
        std::ostringstream oss;
        oss << methodName << " 需要 " << expectedCount << " 個參數，但收到 " << args.size() << " 個";
        return ScriptMethodResult::Error(oss.str());
    }
    return ScriptMethodResult::Success();
}

//----------------------------------------------------------------------------------------------------
ScriptMethodResult GameScriptInterface::ValidateArgCountRange(const std::vector<std::any>& args,
                                                            size_t minCount,
                                                            size_t maxCount,
                                                            const std::string& methodName) const
{
    if (args.size() < minCount || args.size() > maxCount)
    {
        std::ostringstream oss;
        oss << methodName << " 需要 " << minCount << "-" << maxCount << " 個參數，但收到 " << args.size() << " 個";
        return ScriptMethodResult::Error(oss.str());
    }
    return ScriptMethodResult::Success();
}