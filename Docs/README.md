# FirstV8

## Make it First, V8 JavaScript Engine and Chrome DevTools integration into DaemonEngine

![C++](https://img.shields.io/badge/C++20-grey?style=for-the-badge&logo=cplusplus)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey?style=for-the-badge&logo=windows)
![Graphics API](https://img.shields.io/badge/Graphics%20API-DirectX%2011-green?style=for-the-badge&logo=microsoft)
![License](https://img.shields.io/badge/License-Apache%202.0-blue?style=for-the-badge&logo=apache)

A sophisticated C++ game engine demonstrating dual-language architecture through seamless integration of **V8 JavaScript
Engine** and **Chrome DevTools** with the **DaemonEngine** foundation. This project represents the next evolution in
modern game development methodologies, enabling unprecedented flexibility between performance-critical C++ systems and
rapid JavaScript prototyping.

## 🚀 Project Overview

FirstV8 is a groundbreaking research project that bridges the gap between traditional C++ game engine performance and
modern JavaScript development workflows. As part of the "First" series of experimental game development frameworks, this
project demonstrates how to achieve enterprise-grade dual-language architecture while maintaining the performance
characteristics required for production game development.

### 🎯 Core Innovation

- **Dual-Language Architecture**: C++ for engine performance, JavaScript for game logic flexibility
- **Real-Time Hot Reloading**: JavaScript changes without C++ recompilation
- **Chrome DevTools Integration**: Full debugging support for JavaScript game logic
- **DaemonEngine Foundation**: Built upon proven engine architecture patterns
- **Academic Research Quality**: Suitable for computer science research and education

## ✨ Key Features

### 🔥 V8 JavaScript Integration

- **Google V8 Engine v13.0.245.25**: Latest JavaScript runtime with optimal performance
- **Bidirectional Communication**: Seamless C++ ↔ JavaScript interoperability
- **Chrome DevTools Support**: Professional debugging environment through ChromeDevToolsServer
- **Memory Management**: RAII patterns with automatic JavaScript garbage collection
- **Error Isolation**: JavaScript errors don't crash the C++ engine

### 🏗️ Engine Architecture

- **Modular Subsystem Design**: Core, Math, Renderer, Audio, Input, Resource, Network, Scripting
- **Entity-Component System**: Flexible game object architecture with dual-language support
- **Hot-Reload Development**: FileWatcher and ScriptReloader for rapid iteration
- **Production-Ready Build System**: Enterprise-grade MSBuild configuration
- **Cross-Platform Support**: Windows x64 with comprehensive compatibility

### 🛠️ Development Experience

- **Visual Studio 2022 Integration**: Complete debugging support for C++ and JavaScript
- **Academic Documentation**: Research-grade documentation and architectural specifications
- **Professional Build Pipeline**: Automated V8 runtime deployment and asset management
- **Industry Standards**: SOLID principles, modern C++20, and professional coding practices

## 🏭 Architecture Overview

### Dual-Language Integration Flow

```
Windows Application Entry
├── DaemonEngine Foundation
│   ├── Core Subsystems (C++)
│   ├── Rendering Pipeline (C++)
│   └── Resource Management (C++)
├── V8 JavaScript Engine
│   ├── Game Logic Layer (JS)
│   ├── Chrome DevTools (Debug)
│   └── Hot-Reload System (JS)
└── GameScriptInterface
    ├── C++ → JavaScript Bindings
    └── JavaScript → C++ Callbacks
```

### Runtime Execution Model

```
C++ Main Loop:
├── BeginFrame()
├── Update() ──→ V8::Execute(JSEngine.update()) ──→ JSGame.update()
├── Render() ──→ V8::Execute(JSEngine.render()) ──→ JSGame.render()
└── EndFrame()
```

## 📁 Project Structure

```
FirstV8/
├── Code/
│   └── Game/                          # Game Application (.exe)
│       ├── Game.cpp/hpp               # Main game class and state management
│       ├── Entity.cpp/hpp             # Base entity system
│       ├── Player.cpp/hpp             # Player entity with input handling
│       ├── Prop.cpp/hpp               # Interactive game objects
│       ├── Framework/                 # Application infrastructure
│       │   ├── App.cpp/hpp            # Application lifecycle and main loop
│       │   ├── GameScriptInterface.*  # C++ ↔ JavaScript bindings
│       │   ├── FileWatcher.*          # Hot-reload file monitoring
│       │   ├── ScriptReloader.*       # JavaScript hot-reload system
│       │   └── GameCommon.hpp         # Shared definitions and globals
│       ├── Subsystem/                 # Game-specific subsystems
│       │   └── Light/                 # Lighting subsystem example
│       └── EngineBuildPreferences.hpp # Engine compilation configuration
├── Run/                               # Execution Environment
│   ├── Data/                          # Game Assets
│   │   ├── Scripts/                   # JavaScript game logic
│   │   │   ├── JSEngine.js            # JavaScript engine framework
│   │   │   ├── JSGame.js              # Game logic implementation
│   │   │   └── test_scripts.js        # Development and testing scripts
│   │   ├── Shaders/                   # HLSL rendering shaders
│   │   ├── Models/                    # 3D assets (.obj, .fbx)
│   │   ├── Textures/                  # Image assets and materials
│   │   ├── Audio/                     # FMOD audio assets
│   │   └── GameConfig.xml             # Runtime configuration
│   ├── FirstV8_Debug_x64.exe          # Debug application build
│   ├── FirstV8_Release_x64.exe        # Release application build
│   └── *.dll                          # V8 and FMOD runtime libraries
├── Engine/                            # DaemonEngine Integration (External)
│   └── Code/Engine/                   # Engine static library
│       ├── Core/                      # Engine foundation systems
│       ├── Scripting/                 # V8Subsystem and Chrome DevTools
│       ├── Renderer/                  # DirectX graphics pipeline
│       ├── Audio/                     # FMOD audio integration
│       └── [Additional Subsystems]    # Math, Input, Resource, Network
├── Docs/                              # Project Documentation
│   ├── README.md                      # This file
│   └── [Academic Papers]              # Research documentation
└── FirstV8.sln                       # Visual Studio 2022 Solution
```

## 🚀 Getting Started

### Prerequisites

- **Visual Studio 2022** with C++ development workload
- **Windows 10/11 (x64)** - Primary development platform
- **Git** with submodule support
- **NuGet Package Manager** (included with Visual Studio)

### Quick Start

1. **Clone the Repository:**
   ```bash
   git clone --recursive https://github.com/yourusername/FirstV8.git
   cd FirstV8
   ```

2. **Initialize DaemonEngine Submodule:**
   ```bash
   git submodule update --init --recursive
   ```

3. **Open Visual Studio Solution:**
   ```bash
   start FirstV8.sln
   ```

4. **Restore NuGet Packages:**
    - Visual Studio will automatically restore V8 packages
    - Manual restore: `Build → Restore NuGet Packages`

5. **Build the Solution:**
    - Select `Debug|x64` or `Release|x64` configuration
    - `Build → Build Solution` (Ctrl+Shift+B)

6. **Run the Application:**
   ```bash
   cd Run
   FirstV8_Debug_x64.exe
   ```

### Development Workflow

1. **C++ Engine Development**: Modify files in `Code/Game/` and `Engine/`
2. **JavaScript Game Logic**: Edit files in `Run/Data/Scripts/`
3. **Hot Reloading**: JavaScript changes apply automatically without rebuild
4. **Debugging**: Use Visual Studio for C++ and Chrome DevTools for JavaScript
5. **Asset Management**: Add resources to `Run/Data/` subdirectories

## 🔧 Configuration

### Game Configuration (`Run/Data/GameConfig.xml`)

```xml

<GameConfig>
    <WindowClose>false</WindowClose>
    <screenSizeX>1600</screenSizeX>
    <screenSizeY>900</screenSizeY>
    <screenCenterX>800</screenCenterX>
    <screenCenterY>450</screenCenterY>
    <enableVSync>true</enableVSync>
    <debugMode>true</debugMode>
</GameConfig>
```

### V8 Engine Configuration

- **Chrome DevTools Port**: 9222 (configurable)
- **JavaScript Runtime**: V8 v13.0.245.25
- **Memory Management**: Automatic garbage collection with RAII cleanup
- **Error Handling**: Non-fatal JavaScript error reporting

## 🎮 JavaScript Game Development

### Core JavaScript APIs

```javascript
// Game lifecycle callbacks
function update(deltaTime) {
    // Game logic implementation
    player.update(deltaTime);
    entities.forEach(entity => entity.update(deltaTime));
}

function render() {
    // Rendering commands
    renderer.clear();
    scene.render();
    ui.render();
}

// C++ binding examples
createEntity("Player", {x: 0, y: 0, z: 0});
playSound("footstep.wav");
setLightColor(255, 255, 255);
```

### Hot Reload Development

1. Edit JavaScript files in `Run/Data/Scripts/`
2. Save changes (Ctrl+S)
3. FileWatcher automatically detects modifications
4. ScriptReloader recompiles and reloads JavaScript
5. Changes take effect immediately without restart

## 📦 Dependencies

### Core Technologies

- **Google V8 JavaScript Engine**: v13.0.245.25 (Apache 2.0 License)
- **DaemonEngine**: Custom game engine foundation (External submodule)
- **FMOD Audio Engine**: Professional audio system (Commercial license)
- **DirectX Graphics API**: Windows graphics pipeline
- **Microsoft Visual C++ 2022**: C++20 compiler and runtime

### NuGet Packages

- `v8-v143-x64`: V8 engine headers and libraries
- `v8.redist-v143-x64`: V8 runtime redistribution files

## 🔗 Related Projects

- **DaemonEngine**: [Custom Game Engine Foundation](https://github.com/dadavidtseng/Engine)
- **Google V8**: [JavaScript Engine](https://v8.dev/)
- **Chrome DevTools**: [Developer Tools](https://developer.chrome.com/docs/devtools/)
- **FMOD**: [Audio Engine](https://www.fmod.com/)

---

## 📊 Project Status

- **Version**: 1.0.0-alpha
- **Development Status**: Archived
- **Platforms**: Windows x64
- **Build Status**: ✅ Passing (Debug/Release)