// shakeCamera.js - 相機晃動（使用真正的相機移動）

console.log("=== 相機晃動系統 ===");

// 簡單的晃動狀態
var shake = {
    time: 0.0,
    intensity: 0.05,
    frequency: 8.0
};

// 更新晃動效果
function updateShake() {
    shake.time += 0.05; // 較慢的時間增量
    
    // 計算晃動偏移
    var offsetX = Math.sin(shake.time * shake.frequency) * shake.intensity;
    var offsetY = Math.cos(shake.time * shake.frequency * 0.7) * shake.intensity;
    var offsetZ = Math.sin(shake.time * shake.frequency * 1.3) * shake.intensity * 0.3;
    
    // 使用新的 movePlayerCamera API 直接移動相機
    try {
        game.movePlayerCamera(offsetX, offsetY, offsetZ);
    } catch (e) {
        console.log("相機移動失敗:", e);
    }
}

console.log("相機晃動已啟動 - 強度: " + shake.intensity);
console.log("使用 movePlayerCamera API 進行真正的相機晃動");
console.log("基本相機晃動系統準備就緒！");
