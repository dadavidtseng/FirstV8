# Framework Module - Core Application & V8 Integration

**Navigation**: [🏠 Project Root](../../../CLAUDE.md) → **Framework**

## Module Overview

The Framework module provides the core application infrastructure and V8 JavaScript integration layer. It serves as the bridge between the Windows platform, the external Engine dependency, and the game-specific logic, implementing the main application lifecycle and JavaScript scripting capabilities.

## Key Components

### Core Files

| File | Type | Purpose | Coverage |
|------|------|---------|----------|
| **Main_Windows.cpp** | Entry Point | Windows main() and application startup | 100% |
| **App.cpp/.hpp** | Core | Application lifecycle and main game loop | 100% |
| **GameScriptInterface.cpp/.hpp** | Integration | C++ to JavaScript API bindings | 100% |
| **GameCommon.hpp** | Headers | Shared definitions and includes | 100% |

### Architecture Details

#### Application Lifecycle (`App.cpp`)
```cpp
class App {
    void RunMainLoop() {
        BeginFrame();
        Update();    // → Calls V8 JavaScript execution
        Render();    // → Calls V8 JavaScript rendering
        EndFrame();
    }
};
```

#### V8 Integration Flow
```
Windows Main Entry
├── App::Initialize()
│   ├── Engine Subsystem Init
│   ├── V8Subsystem Init (../Engine)
│   └── GameScriptInterface Setup
├── App::RunMainLoop()
│   ├── BeginFrame()
│   ├── Update() → V8::Execute(JSEngine.update)
│   ├── Render() → V8::Execute(JSEngine.render) 
│   └── EndFrame()
└── App::Shutdown()
```

## Dependencies

### External Engine Integration
- **Location**: `../Engine/Code/Engine/`
- **Key Component**: `V8Subsystem` for JavaScript execution
- **Binding**: Through project references and headers
- **Status**: Fully functional in Debug/Release modes

### V8 JavaScript Engine
- **Version**: v13.0.245.25 (NuGet package)
- **Integration**: Via Engine's V8Subsystem wrapper
- **Memory Management**: RAII patterns with proper cleanup
- **Error Handling**: Exception catching and logging

### System Dependencies
- **Platform**: Windows SDK for window management
- **Graphics**: DirectX integration through Engine
- **Audio**: FMOD integration through Engine

## JavaScript API Bindings

### GameScriptInterface Methods
The `GameScriptInterface` class exposes C++ functionality to JavaScript:

```cpp
// Core game methods available to JavaScript
void update(float deltaTime);
void render();
void handleInput();
void createEntity(string type, Vector3 position);
void destroyEntity(int entityId);
// ... 14+ additional methods
```

### Lifecycle Synchronization
- **C++ Main Loop**: Drives the overall application timing
- **JavaScript Execution**: Invoked at specific lifecycle points
- **Bidirectional Calls**: C++ can call JS, JS can call back to C++
- **Error Isolation**: JavaScript errors don't crash the C++ engine

## Build Integration

### Project Configuration
- **Visual Studio Project**: Framework.vcxproj
- **C++ Standard**: C++20 with modern features
- **Platform**: x64 Windows
- **Dependencies**: Engine project reference + V8 NuGet

### Compilation Flow
1. **Header Dependencies**: GameCommon.hpp provides shared definitions
2. **Engine Linking**: Automatic linking to external Engine project
3. **V8 Integration**: NuGet package provides V8 libraries
4. **Output**: Framework.lib for linking with Game project

## Key Features

### Dual-Language Architecture
- **Separation of Concerns**: C++ for performance, JS for flexibility
- **Hot Reloading**: JavaScript changes without C++ recompilation
- **Type Safety**: Strict parameter validation at C++/JS boundary

### Error Handling
- **Graceful Degradation**: JavaScript errors logged, C++ continues
- **Debug Support**: Enhanced error reporting in Debug builds
- **Memory Safety**: RAII patterns prevent resource leaks

### Platform Abstraction
- **Windows Integration**: Native window management and input
- **Cross-Language Events**: Event system works across C++/JS
- **Resource Management**: Unified resource lifetime management

## Development Patterns

### Adding New JavaScript APIs
1. **Add method to GameScriptInterface**: Define C++ implementation
2. **Register with V8Subsystem**: Bind function name to implementation
3. **Test in JSEngine**: Verify callable from JavaScript
4. **Document in JSGame**: Add to game-level API

### Debugging JavaScript Issues
1. **Enable Debug Mode**: Additional V8 error reporting
2. **Console Logging**: JavaScript console.log() support
3. **Breakpoints**: Mixed-mode debugging C++ and JS
4. **Error Stack Traces**: Full JavaScript stack trace reporting

---

**Module Status**: ✅ Complete - 100% Coverage  
**External Dependencies**: Engine V8Subsystem, V8 v13.0.245.25  
**Last Updated**: 2025-09-06