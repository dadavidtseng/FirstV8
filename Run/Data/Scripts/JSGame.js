// JSGame.js - Enhanced JavaScript Game Framework with System Registration
class JSGame {
    constructor(engine) {
        this.engine = engine;
        this.elapsedTime = 0;
        this.frameCount = 0;
        this.testCounter = 0;

        console.log('JSGame: Game instance created');
        
        // Register individual game systems
        this.registerGameSystems();
    }

    /**
     * Register all game systems with the engine
     */
    registerGameSystems() {
        if (!this.engine || !this.engine.registerSystem) {
            console.log('JSGame: Engine does not support system registration, using legacy mode');
            return;
        }

        // Register C++ Bridge System (highest priority - must run first)
        this.engine.registerSystem('cppBridge', {
            update: (deltaTime) => this.updateCppBridge(deltaTime),
            render: () => this.renderCppBridge(),
            priority: 0,
            data: { description: 'C++ engine bridge system' }
        });

        // Register Input System (demonstrable - can be enabled/disabled)
        this.engine.registerSystem('inputHandler', {
            update: (deltaTime) => this.updateInputHandler(deltaTime),
            priority: 10,
            enabled: true,
            data: {
                description: 'F1 key handler and other input',
                lastF1State: false
            }
        });

        // Register Cube Spawner System
        this.engine.registerSystem('cubeSpawner', {
            update: (deltaTime) => this.updateCubeSpawner(deltaTime),
            priority: 20,
            data: { 
                description: 'Spawns cubes every 4 seconds',
                lastSpawnFrame: 0,
                interval: 240
            }
        });

        // Register Prop Mover System
        this.engine.registerSystem('propMover', {
            update: (deltaTime) => this.updatePropMover(deltaTime),
            priority: 30,
            data: {
                description: 'Moves props every 2 seconds',
                lastMoveFrame: 0,
                interval: 120
            }
        });

        // Register Camera Shaker System
        this.engine.registerSystem('cameraShaker', {
            update: (deltaTime) => this.updateCameraShaker(deltaTime),
            priority: 40,
            data: {
                description: 'Shakes camera every 6 seconds',
                lastShakeFrame: 0,
                interval: 360
            }
        });

        // Register Status Logger System
        this.engine.registerSystem('statusLogger', {
            update: (deltaTime) => this.updateStatusLogger(deltaTime),
            priority: 100,
            data: {
                description: 'Logs status every 10 seconds',
                lastLogFrame: 0,
                interval: 600
            }
        });

        console.log('JSGame: All systems registered with engine');
    }

    // ============================================================================
    // AI AGENT API - For runtime system control
    // ============================================================================

    /**
     * Enable input system - F1 key will work
     */
    enableInput() {
        return this.engine.setSystemEnabled('inputHandler', true);
    }

    /**
     * Disable input system - F1 key will NOT work
     */
    disableInput() {
        return this.engine.setSystemEnabled('inputHandler', false);
    }

    /**
     * Check if input system is enabled
     */
    isInputEnabled() {
        const system = this.engine.getSystem('inputHandler');
        return system ? system.enabled : false;
    }

    /**
     * Register new system at runtime (for AI agents)
     */
    registerSystem(id, config) {
        return this.engine.registerSystem(id, config);
    }

    /**
     * Unregister system at runtime (for AI agents)
     */
    unregisterSystem(id) {
        return this.engine.unregisterSystem(id);
    }

    // ============================================================================
    // SYSTEM IMPLEMENTATIONS
    // ============================================================================

    /**
     * C++ Bridge System - maintains original functionality
     */
    updateCppBridge(deltaTime) {
        this.frameCount++;
        this.elapsedTime += deltaTime / 1000.0;

        // Call C++ engine update through JSEngine (preserve original logic)
        if (this.engine) {
            const deltaSeconds = deltaTime / 1000.0;
            this.engine.updateCppEngine(deltaSeconds, deltaSeconds);
        }
    }

    renderCppBridge() {
        // Check shouldRender flag for F1 toggle functionality
        let shouldRenderValue = true;

        if (typeof shouldRender !== 'undefined') {
            shouldRenderValue = shouldRender;
        } else if (typeof globalThis.shouldRender !== 'undefined') {
            shouldRenderValue = globalThis.shouldRender;
        }

        // Call C++ engine render through JSEngine (preserve original logic)
        if (shouldRenderValue && this.engine) {
            this.engine.renderCppEngine(0.0, 0.0);
        }

        // Log render info occasionally
        if (this.frameCount % 600 === 0) {
            console.log('JSGame: Render called - frame ' + this.frameCount + ', shouldRender=' + shouldRenderValue);
        }

        if (!shouldRenderValue) {
            console.log('JSGame: Rendering skipped due to F1 toggle');
        }
    }

    /**
     * Input Handler System - can be enabled/disabled for demo
     */
    updateInputHandler(deltaTime) {
        const system = this.engine.getSystem('inputHandler');
        if (!system) return;

        // Handle F1 key for render toggle (preserve original logic)
        let currentF1State = false;
        if (typeof input !== 'undefined' && input.wasKeyJustPressed) {
            currentF1State = input.wasKeyJustPressed(112); // F1 key code
        }

        // Edge detection for F1 key
        if (currentF1State && !system.data.lastF1State) {
            if (typeof shouldRender !== 'undefined') {
                shouldRender = !shouldRender;
                console.log('JSGame: F1 pressed via INPUT SYSTEM, shouldRender =', shouldRender);
            } else if (typeof globalThis.shouldRender !== 'undefined') {
                globalThis.shouldRender = !globalThis.shouldRender;
                console.log('JSGame: F1 pressed via INPUT SYSTEM, globalThis.shouldRender =', globalThis.shouldRender);
            } else {
                console.log('JSGame: F1 pressed but shouldRender variable not found!');
            }
        }
        system.data.lastF1State = currentF1State;

        // Log input system status occasionally
        if (this.frameCount % 300 === 0) {
            console.log('JSGame: Input system active, F1 detection enabled');
        }
    }

    /**
     * Cube Spawner System
     */
    updateCubeSpawner(deltaTime) {
        const system = this.engine.getSystem('cubeSpawner');
        if (!system) return;

        if (this.frameCount - system.data.lastSpawnFrame >= system.data.interval) {
            this.testCreateCube();
            system.data.lastSpawnFrame = this.frameCount;
        }
    }

    /**
     * Prop Mover System
     */
    updatePropMover(deltaTime) {
        const system = this.engine.getSystem('propMover');
        if (!system) return;

        if (this.frameCount - system.data.lastMoveFrame >= system.data.interval && this.frameCount > 240) {
            this.testMoveProp();
            system.data.lastMoveFrame = this.frameCount;
        }
    }

    /**
     * Camera Shaker System
     */
    updateCameraShaker(deltaTime) {
        const system = this.engine.getSystem('cameraShaker');
        if (!system) return;

        if (this.frameCount - system.data.lastShakeFrame >= system.data.interval) {
            this.testCameraShake();
            system.data.lastShakeFrame = this.frameCount;
        }
    }

    /**
     * Status Logger System
     */
    updateStatusLogger(deltaTime) {
        const system = this.engine.getSystem('statusLogger');
        if (!system) return;

        if (this.frameCount - system.data.lastLogFrame >= system.data.interval) {
            this.logStatus();
            system.data.lastLogFrame = this.frameCount;
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
        
        // Show registered systems info
        if (this.engine && this.engine.listSystems) {
            const systems = this.engine.listSystems();
            console.log('Registered Systems:', systems.length);
            systems.forEach(sys => {
                console.log(`  - ${sys.id}: ${sys.enabled ? 'enabled' : 'DISABLED'} (priority: ${sys.priority})`);
            });
        }
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

// ============================================================================
// AI AGENT GLOBAL API
// ============================================================================

// Create a clean global API for AI agents and developers
globalThis.GameAPI = {
    // System management
    registerSystem: (id, config) => jsEngineInstance.registerSystem(id, config),
    unregisterSystem: (id) => jsEngineInstance.unregisterSystem(id),
    setSystemEnabled: (id, enabled) => jsEngineInstance.setSystemEnabled(id, enabled),
    getSystem: (id) => jsEngineInstance.getSystem(id),
    listSystems: () => jsEngineInstance.listSystems(),
    
    // Input system demo
    enableInput: () => jsGameInstance.enableInput(),
    disableInput: () => jsGameInstance.disableInput(),
    isInputEnabled: () => jsGameInstance.isInputEnabled(),
    
    // Engine status
    getStatus: () => jsEngineInstance.getStatus(),
    
    // Direct access for advanced use
    engine: jsEngineInstance,
    game: jsGameInstance
};

// Initialize shouldRender flag for F1 toggle functionality
if (typeof globalThis.shouldRender === 'undefined') {
    globalThis.shouldRender = true;
}

console.log('JSGame: Enhanced system registration framework initialized');
console.log('Available API: globalThis.GameAPI');
console.log('  - GameAPI.enableInput() / disableInput()');
console.log('  - GameAPI.registerSystem(id, config)');
console.log('  - GameAPI.unregisterSystem(id)');
console.log('  - GameAPI.setSystemEnabled(id, enabled)');
console.log('  - GameAPI.listSystems()');
console.log('');
console.log('=== INPUT SYSTEM DEMO ===');
console.log('Input system is currently:', jsGameInstance.isInputEnabled() ? 'ENABLED' : 'DISABLED');
console.log('To disable input: GameAPI.disableInput()');
console.log('To enable input: GameAPI.enableInput()');
console.log('==========================');