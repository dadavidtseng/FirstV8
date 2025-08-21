//----------------------------------------------------------------------------------------------------
// GameScriptInterface.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Scripting/IScriptableObject.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class Game;
class Player;
struct Vec3;

//----------------------------------------------------------------------------------------------------
// Game 類別的腳本介面包裝器
// 這個類別作為 Game 物件與 V8Subsystem 之間的橋樑
//----------------------------------------------------------------------------------------------------
class GameScriptInterface : public IScriptableObject
{
public:
    explicit GameScriptInterface(Game* game);

    // 實作 IScriptableObject 介面
    std::string                   GetScriptObjectName() const override;
    std::vector<ScriptMethodInfo> GetAvailableMethods() const override;
    ScriptMethodResult            CallMethod(std::string const & methodName,  std::vector<std::any> const& args) override;

    // 實作屬性存取
    std::any                 GetProperty(const std::string& propertyName) const override;
    bool                     SetProperty(const std::string& propertyName, const std::any& value) override;
    std::vector<std::string> GetAvailableProperties() const override;

private:
    Game* m_game; // 不擁有，只是參考

    // 輔助方法來處理類型轉換和錯誤檢查
    template <typename T>
    T ExtractArg(const std::any& arg, const std::string& expectedType = "") const;

    // 專門的類型提取方法
    Vec3        ExtractVec3(const std::vector<std::any>& args, size_t startIndex) const;
    float       ExtractFloat(const std::any& arg) const;
    int         ExtractInt(const std::any& arg) const;
    std::string ExtractString(const std::any& arg) const;
    bool        ExtractBool(const std::any& arg) const;

    // 參數驗證輔助方法
    ScriptMethodResult ValidateArgCount(const std::vector<std::any>& args,
                                        size_t                       expectedCount,
                                        const std::string&           methodName) const;

    ScriptMethodResult ValidateArgCountRange(const std::vector<std::any>& args,
                                             size_t                       minCount,
                                             size_t                       maxCount,
                                             const std::string&           methodName) const;

    // 方法實作
    ScriptMethodResult ExecuteCreateCube(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteMoveProp(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteGetPlayerPosition(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteMovePlayerCamera(const std::vector<std::any>& args);  // 新增：移動玩家相機
    ScriptMethodResult ExecuteJavaScriptCommand(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteJavaScriptFile(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteIsAttractMode(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteGetGameState(const std::vector<std::any>& args);
};
