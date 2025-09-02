//---------------------------------------------------------------------------------------------------- 
// shakeCamera.js 
//----------------------------------------------------------------------------------------------------

(function() {
    console.log("(shakeCamera.js)(start)");

    //----------------------------------------------------------------------------------------------------
    const shake = {
        time: 0.0,
        intensity: 0.05,
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
            console.log("(shakeCamera.js)(failed)", e);
        }
    }

    console.log("(shakeCamera.js)(intensity)"+ shake.intensity);

    /// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/globalThis#search_for_the_global_across_environments
    // If you need to expose functions globally for external access
    globalThis.updateShake = updateShake;

    // Or if you need to expose the shake object for external modification
    globalThis.shake = shake;

})();