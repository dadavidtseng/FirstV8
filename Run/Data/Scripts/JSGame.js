// JSGame.js - Simple JavaScript Game Framework
class JSGame {
    constructor(engine) {
        this.engine = engine;
        this.elapsedTime = 0;
        this.frameCount = 0;
        this.testCounter = 0;

        console.log('JSGame: Game instance created');
    }

    /**
     * Update method - called by JSEngine every frame
     */
    update(deltaTime) {
        this.frameCount++;
        this.elapsedTime += deltaTime / 1000.0; // Convert ms to seconds

        // FIRST: Call C++ engine update through JSEngine
        if (this.engine) {
            // Convert deltaTime back to seconds for C++ (it expects seconds)
            const deltaSeconds = deltaTime / 1000.0;
            this.engine.updateCppEngine(deltaSeconds, deltaSeconds);
        }

        // THEN: Do JavaScript-specific game logic (framework features)
        // Framework test logic - these are ADDITIONAL to the main C++ game logic
        // Simple test: every 240 frames (4 seconds at 60fps), create a cube
        if (this.frameCount % 240 === 0) {
            this.testCreateCube();
        }

        // Simple test: every 120 frames (2 seconds at 60fps), move existing props
        if (this.frameCount % 120 === 0 && this.frameCount > 240) {
            this.testMoveProp();
        }

        // Simple test: every 360 frames (6 seconds at 60fps), shake camera
        if (this.frameCount % 360 === 0) {
            this.testCameraShake();
        }

        // Log status every 10 seconds
        if (this.frameCount % 600 === 0) {
            this.logStatus();
        }

        // Handle F1 key for render toggle
        if (typeof input !== 'undefined' && input.wasKeyJustPressed && input.wasKeyJustPressed(112)) {
            if (typeof shouldRender !== 'undefined') {
                shouldRender = !shouldRender;
                console.log('JSGame: F1 pressed, shouldRender =', shouldRender);
            } else {
                globalThis.shouldRender = !globalThis.shouldRender;
                console.log('JSGame: F1 pressed, shouldRender =', globalThis.shouldRender);
            }
        }
    }

    /**
     * Render method - called by JSEngine every frame
     */
    render() {
        // Check shouldRender flag for F1 toggle functionality
        let shouldRenderValue = true; // default to true

        if (typeof shouldRender !== 'undefined') {
            shouldRenderValue = shouldRender;
        } else if (typeof globalThis.shouldRender !== 'undefined') {
            shouldRenderValue = globalThis.shouldRender;
        }

        // FIRST: Call C++ engine render through JSEngine (respecting shouldRender flag)
        if (shouldRenderValue && this.engine) {
            // Use 0.0 for both delta times as render doesn't need them
            this.engine.renderCppEngine(0.0, 0.0);
        }

        // THEN: Do JavaScript-specific rendering logic
        // Log render info occasionally
        if (this.frameCount % 600 === 0) { // Every 10 seconds
            console.log('JSGame: Render called - frame ' + this.frameCount + ', shouldRender=' + shouldRenderValue);
        }

        // Additional JavaScript-only rendering logic could go here
        if (!shouldRenderValue) {
            console.log('JSGame: Rendering skipped due to F1 toggle');
        }
    }

    /**
     * Test methods to demonstrate the framework
     */
    testCreateCube() {
        if (this.engine) {
            const x = (Math.random() - 0.5) * 10;
            const y = (Math.random() - 0.5) * 10;
            const z = Math.random() * 3;

            this.engine.createCube(x, y, z);
            console.log('JSGame: Test - Created random cube');
        }
    }

    testMoveProp() {
        if (this.engine) {
            const propIndex = 0; // Move the first prop
            const x = (Math.random() - 0.5) * 8;
            const y = (Math.random() - 0.5) * 8;
            const z = Math.random() * 2;

            this.engine.moveProp(propIndex, x, y, z);
            console.log('JSGame: Test - Moved prop');
        }
    }

    testCameraShake() {
        if (this.engine) {
            const shakeX = (Math.random() - 0.5) * 0.2;
            const shakeY = (Math.random() - 0.5) * 0.2;
            const shakeZ = (Math.random() - 0.5) * 0.1;

            this.engine.moveCamera(shakeX, shakeY, shakeZ);
            console.log('JSGame: Test - Camera shake');
        }
    }

    logStatus() {
        console.log('=== JSGame Status ===');
        console.log('Frame Count: ' + this.frameCount);
        console.log('Elapsed Time: ' + this.elapsedTime.toFixed(2) + 's');

        if (this.engine) {
            const playerPos = this.engine.getPlayerPosition();
            console.log('Player Position: (' + playerPos.x + ', ' + playerPos.y + ', ' + playerPos.z + ')');
        }

        console.log('Engine Status:', this.engine ? this.engine.getStatus() : 'No engine');
    }
}

// Make it globally available
if (typeof globalThis !== 'undefined') {
    globalThis.JSGame = JSGame;
} else if (typeof window !== 'undefined') {
    window.JSGame = JSGame;
}

const jsEngineInstance = new JSEngine();
const jsGameInstance = new JSGame(jsEngineInstance);
jsEngineInstance.initialize();
jsEngineInstance.setGame(jsGameInstance);
globalThis.JSEngine = jsEngineInstance;