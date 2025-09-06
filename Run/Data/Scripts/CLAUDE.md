# JavaScript Runtime - JSEngine & JSGame Architecture

**Navigation**: [🏠 Project Root](../../../CLAUDE.md) → **JavaScript Runtime**

## Module Overview

The JavaScript Runtime module implements a sophisticated dual-layer JavaScript architecture that provides both engine-level abstractions and game-specific logic. This module demonstrates modern JavaScript integration patterns with C++ game engines, featuring complete API coverage, comprehensive testing, and clear separation between framework and application concerns.

## Architecture: Dual-Layer Design

### Layer 1: JSEngine (Framework Layer)
**File**: `JSEngine.js`  
**Purpose**: Engine abstraction and C++ integration wrapper

```javascript
// JSEngine provides the foundation framework
class JSEngine {
    static update(deltaTime) {
        // Framework-level update logic
        // Coordinates with C++ App::Update()
    }
    
    static render() {
        // Framework-level rendering coordination
        // Manages render state and C++ integration
    }
    
    // Engine-level utilities and abstractions
}
```

### Layer 2: JSGame (Game Logic Layer)
**File**: `JSGame.js`  
**Purpose**: Game-specific logic and entity management

```javascript
// JSGame handles actual gameplay
class JSGame {
    static update(deltaTime) {
        // Game logic: entities, AI, physics
        // Called by JSEngine framework layer
    }
    
    static render() {
        // Game rendering: UI, effects, debug visuals
        // Called by JSEngine framework layer
    }
    
    // Game-specific systems and logic
}
```

## Lifecycle Integration

### C++ to JavaScript Flow
```
C++ App::RunMainLoop()
├── App::Update()
│   └── V8::Execute("JSEngine.update(deltaTime)")
│       └── JSEngine.update() → JSGame.update()
├── App::Render()
│   └── V8::Execute("JSEngine.render()")
│       └── JSEngine.render() → JSGame.render()
```

### JavaScript to C++ Callbacks
```javascript
// JavaScript can call back to C++ through GameScriptInterface
JSGame.createEntity("Player", {x: 0, y: 0, z: 0});
JSGame.destroyEntity(entityId);
JSGame.handleInput(inputEvent);
// All mapped to C++ GameScriptInterface methods
```

## API Reference

### JSEngine Framework APIs
- **Lifecycle Management**: `update()`, `render()`, `initialize()`
- **C++ Integration**: Wrapper methods for GameScriptInterface calls
- **Error Handling**: JavaScript exception management and reporting
- **Utility Functions**: Math, vector operations, debugging tools

### JSGame Application APIs
- **Entity Management**: Create, destroy, update entities
- **Game Logic**: Player control, AI behaviors, game rules
- **Rendering**: Game-specific visual effects and UI
- **Input Handling**: Keyboard, mouse, controller input processing

### C++ GameScriptInterface Binding
The JavaScript layer can access these C++ methods:

```javascript
// Entity System
game.createEntity(type, position);
game.destroyEntity(entityId);
game.moveEntity(entityId, newPosition);

// Rendering
game.setRenderMode(mode);
game.drawDebugLine(start, end, color);

// Input
game.getInput(inputType);
game.handleInputEvent(event);

// System
game.getFrameTime();
game.getDeltaTime();
game.exit();
```

## Testing Framework

### Test Suite: `test_scripts.js`
**Coverage**: 5 comprehensive test modules

#### Module 1: Basic JavaScript Functionality
```javascript
// Tests core JavaScript execution in V8 context
testBasicSyntax();
testVariableTypes();
testFunctionCalls();
testObjectManipulation();
```

#### Module 2: C++ API Bindings
```javascript
// Tests all GameScriptInterface methods
testCreateEntity();
testDestroyEntity(); 
testRenderMethods();
testInputHandling();
```

#### Module 3: Math and Utility Functions
```javascript
// Tests mathematical operations and utilities
testVectorMath();
testMatrixOperations();
testUtilityFunctions();
testPerformanceMeasurements();
```

#### Module 4: Pattern Generation
```javascript
// Tests complex algorithmic patterns
testPatternGeneration();
testProceduralGeneration();
testAlgorithmicArt();
```

#### Module 5: Game Logic Integration
```javascript
// Tests complete game systems
testPlayerMovement();
testEntityInteractions();
testGameStateManagement();
testRenderingPipeline();
```

## Error Handling & Debugging

### JavaScript Error Management
- **Exception Catching**: All JS errors caught and logged
- **Stack Traces**: Full JavaScript stack trace reporting
- **Error Recovery**: Non-fatal errors don't crash C++ engine
- **Debug Modes**: Enhanced debugging in Debug builds

### Development Tools
- **Console Logging**: `console.log()` support with C++ output
- **Runtime Inspection**: Live variable inspection during execution
- **Performance Profiling**: JavaScript execution timing
- **Mixed Debugging**: C++ and JavaScript breakpoints simultaneously

## Performance Characteristics

### Execution Model
- **Just-In-Time Compilation**: V8 JIT optimization active
- **Memory Management**: V8 garbage collection with C++ coordination
- **Call Overhead**: Minimal C++/JS boundary crossing cost
- **Hot Paths**: Frequently called methods optimized by V8

### Optimization Patterns
- **Batching**: Group multiple C++ calls to reduce boundary crossings
- **Caching**: Cache frequently accessed C++ data in JavaScript
- **Lazy Loading**: Initialize heavy systems only when needed
- **Pooling**: Reuse JavaScript objects to reduce GC pressure

## Development Workflow

### Adding New Game Features
1. **Design in JavaScript**: Prototype in JSGame layer
2. **Test with Suite**: Add tests to `test_scripts.js`
3. **Integrate C++ APIs**: Add GameScriptInterface methods if needed
4. **Performance Profile**: Measure and optimize hot paths

### Debugging JavaScript Issues
1. **Console Output**: Use console.log() for immediate feedback
2. **Test Isolation**: Run specific test modules to isolate issues
3. **Mixed Debugging**: Set breakpoints in both C++ and JavaScript
4. **Error Analysis**: Check JavaScript stack traces in log output

### Hot Reloading Workflow
1. **Edit JavaScript**: Modify .js files in Run/Data/Scripts/
2. **Press F1**: Toggle rendering to reload scripts
3. **Test Changes**: Immediate feedback without C++ recompilation
4. **Iterate Rapidly**: Full game logic iteration in seconds

## Integration Points

### With Framework Module
- **App Lifecycle**: Framework calls JavaScript at precise timing points
- **GameScriptInterface**: All C++ APIs exposed through this interface
- **V8Subsystem**: Core V8 integration handled by external Engine

### With Game Assets
- **Configuration**: JavaScript can load and parse XML game config
- **Resource Loading**: Scripts can trigger asset loading requests
- **Dynamic Content**: JavaScript can generate procedural content

### With Game Core
- **Entity System**: JavaScript controls C++ entity creation/destruction
- **Player Control**: JavaScript processes input and updates player state
- **Game Rules**: All game logic and rules implemented in JavaScript

---

**Module Status**: ✅ Complete - 100% Coverage  
**Files**: JSEngine.js (framework), JSGame.js (logic), test_scripts.js (testing)  
**Test Coverage**: 5 comprehensive test modules  
**API Methods**: 14+ C++ bindings available  
**Last Updated**: 2025-09-06