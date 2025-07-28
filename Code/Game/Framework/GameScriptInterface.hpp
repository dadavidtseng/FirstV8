//----------------------------------------------------------------------------------------------------
// GameScriptInterface.hpp
// Game 類別的腳本介面包裝器 - 讓 JavaScript 可以與 Game 物件互動
//----------------------------------------------------------------------------------------------------

#pragma once
#include "Engine/Scripting/IScriptableObject.hpp"
#include <memory>

//----------------------------------------------------------------------------------------------------
// 前向宣告
//----------------------------------------------------------------------------------------------------
class Game;
class Player;
class Vec3;

//----------------------------------------------------------------------------------------------------
// Game 類別的腳本介面包裝器
// 這個類別作為 Game 物件與 V8Subsystem 之間的橋樑
//----------------------------------------------------------------------------------------------------
class GameScriptInterface : public IScriptableObject
{
public:
    explicit GameScriptInterface(Game* game);
    virtual ~GameScriptInterface() = default;

    // 實作 IScriptableObject 介面
    virtual std::string GetScriptObjectName() const override;
    virtual std::vector<ScriptMethodInfo> GetAvailableMethods() const override;
    virtual ScriptMethodResult CallMethod(const std::string& methodName,
                                        const std::vector<std::any>& args) override;

    // 實作屬性存取
    virtual std::any GetProperty(const std::string& propertyName) const override;
    virtual bool SetProperty(const std::string& propertyName, const std::any& value) override;
    virtual std::vector<std::string> GetAvailableProperties() const override;

private:
    Game* m_game; // 不擁有，只是參考

    // 輔助方法來處理類型轉換和錯誤檢查
    template<typename T>
    T ExtractArg(const std::any& arg, const std::string& expectedType = "") const;

    // 專門的類型提取方法
    Vec3 ExtractVec3(const std::vector<std::any>& args, size_t startIndex) const;
    float ExtractFloat(const std::any& arg) const;
    int ExtractInt(const std::any& arg) const;
    std::string ExtractString(const std::any& arg) const;
    bool ExtractBool(const std::any& arg) const;

    // 參數驗證輔助方法
    ScriptMethodResult ValidateArgCount(const std::vector<std::any>& args,
                                      size_t expectedCount,
                                      const std::string& methodName) const;

    ScriptMethodResult ValidateArgCountRange(const std::vector<std::any>& args,
                                           size_t minCount,
                                           size_t maxCount,
                                           const std::string& methodName) const;

    // 方法實作
    ScriptMethodResult ExecuteCreateCube(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteMoveProp(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteGetPlayerPosition(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteJavaScriptCommand(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteJavaScriptFile(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteIsAttractMode(const std::vector<std::any>& args);
    ScriptMethodResult ExecuteGetGameState(const std::vector<std::any>& args);
};