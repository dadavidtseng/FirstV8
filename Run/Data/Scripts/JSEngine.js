// JSEngine.js - Simple JavaScript Engine Framework
class JSEngine {
    constructor() {
        this.game = null;
        this.isInitialized = false;
        this.frameCount = 0;
        
        console.log('JSEngine: Created');
    }

    /**
     * Initialize the engine
     */
    initialize() {
        console.log('JSEngine: Initializing...');
        this.isInitialized = true;
        this.frameCount = 0;
        return this;
    }

    /**
     * Set the game instance
     */
    setGame(gameInstance) {
        this.game = gameInstance;
        console.log('JSEngine: Game instance set');
    }

    /**
     * Update method - called by C++ engine
     * This calls the game's update method
     */
    update(deltaTime) {
        if (!this.isInitialized || !this.game) {
            return;
        }
        
        this.frameCount++;
        
        // Call the game's update method first
        if (this.game.update) {
            this.game.update(deltaTime);
        }
    }

    /**
     * Render method - called by C++ engine
     * This calls the game's render method
     */
    render() {
        if (!this.isInitialized || !this.game) {
            return;
        }
        
        // Call the game's render method first
        if (this.game.render) {
            this.game.render();
        }
    }

    /**
     * C++ Engine interface methods - called by JSGame
     * These bridge to the actual C++ engine functions
     */
    updateCppEngine(gameDeltaSeconds, systemDeltaSeconds) {
        if (typeof game !== 'undefined' && game.update) {
            game.update(gameDeltaSeconds || 0.0, systemDeltaSeconds || 0.0);
            return true;
        }
        console.warn('JSEngine: C++ game.update not available');
        return false;
    }

    renderCppEngine(gameDeltaSeconds, systemDeltaSeconds) {
        if (typeof game !== 'undefined' && game.render) {
            game.render(gameDeltaSeconds || 0.0, systemDeltaSeconds || 0.0);
            return true;
        }
        console.warn('JSEngine: C++ game.render not available');
        return false;
    }

    /**
     * Helper methods for game to use C++ engine functions
     */
    createCube(x, y, z) {
        if (typeof game !== 'undefined' && game.createCube) {
            game.createCube(x, y, z);
            console.log(`JSEngine: Created cube at (${x}, ${y}, ${z})`);
            return true;
        }
        console.warn('JSEngine: createCube not available');
        return false;
    }

    moveProp(index, x, y, z) {
        if (typeof game !== 'undefined' && game.moveProp) {
            game.moveProp(index, x, y, z);
            console.log(`JSEngine: Moved prop ${index} to (${x}, ${y}, ${z})`);
            return true;
        }
        console.warn('JSEngine: moveProp not available');
        return false;
    }

    getPlayerPosition() {
        if (typeof game !== 'undefined' && game.getPlayerPos) {
            return game.getPlayerPos();
        }
        console.warn('JSEngine: getPlayerPos not available');
        return { x: 0, y: 0, z: 0 };
    }

    moveCamera(x, y, z) {
        if (typeof game !== 'undefined' && game.movePlayerCamera) {
            game.movePlayerCamera(x, y, z);
            return true;
        }
        console.warn('JSEngine: movePlayerCamera not available');
        return false;
    }

    /**
     * Get engine status
     */
    getStatus() {
        return {
            isInitialized: this.isInitialized,
            hasGame: this.game !== null,
            frameCount: this.frameCount
        };
    }
}

// Make the class globally available
if (typeof globalThis !== 'undefined') {
    globalThis.JSEngine = JSEngine;
} else if (typeof window !== 'undefined') {
    window.JSEngine = JSEngine;
}
