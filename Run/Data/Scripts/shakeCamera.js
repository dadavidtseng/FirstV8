//---------------------------------------------------------------------------------------------------- 
// shakeCamera.js 
//----------------------------------------------------------------------------------------------------

console.log("=== 相機晃動系統 ===");

//----------------------------------------------------------------------------------------------------
const shake = {
    time: 0.0,
    intensity: 0.5,
    frequency: 8.0
};

//----------------------------------------------------------------------------------------------------
function updateShake() {
    shake.time += 0.5;

    const offsetX = Math.sin(shake.time * shake.frequency) * shake.intensity;
    const offsetY = Math.cos(shake.time * shake.frequency * 0.7) * shake.intensity;
    const offsetZ = Math.sin(shake.time * shake.frequency * 1.3) * shake.intensity * 0.3;

    try {
        game.movePlayerCamera(offsetX, offsetY, offsetZ);
    } catch (e) {
        console.log("相機移動失敗:", e);
    }
}

console.log("相機晃動已啟動 - 強度: " + shake.intensity);
console.log("使用 movePlayerCamera API 進行真正的相機晃動");
console.log("基本相機晃動系統準備就緒！");
