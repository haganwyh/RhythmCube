# Rhythm Cube v0.3.1a

A 3D music rhythm game built in **Unreal Engine 5** with C++, featuring a 3D note highway, real-time audio visualization, and custom beatmap support.

> **Status:** Playable Alpha/Beta — core gameplay is functional and actively evolving.

---

## About

Rhythm Cube is an open-source 3D rhythm game that challenges players to hit notes as they travel down a 3D highway in sync with the music. Built from the ground up in Unreal Engine 5, the project explores real-time audio visualization, dynamic gameplay mechanics, and level encoding techniques.

## GamePlay

<img width="1379" height="775" alt="圖片1" src="https://github.com/user-attachments/assets/441f3352-b05b-4890-8dc6-1aaf94b43f98" />
<img width="1919" height="1079" alt="Screenshot 2026-08-29 150251" src="https://github.com/user-attachments/assets/6e76d637-9cdf-4347-b643-1a6dc9773688" />
<img width="1379" height="775" alt="圖片5" src="https://github.com/user-attachments/assets/a4fbac61-9a8b-4560-95e7-0743173c51ff" />
<img width="1379" height="774" alt="圖片3" src="https://github.com/user-attachments/assets/53240545-ec82-45e1-b856-905ab41e90ea" />
<img width="1379" height="775" alt="圖片2" src="https://github.com/user-attachments/assets/6b70c01d-ff24-4577-af98-0fd9c453c7a0" />

## Features

- **3D Note Highway** — Notes travel along a fully 3D lane system, giving depth and perspective to the gameplay experience.
- **Real-Time Audio Visualization** — The game environment reacts dynamically to the music, creating an immersive audiovisual experience.
- **Custom Beatmap Support** — Create and play your own levels with a flexible level encoding system.
- **Dynamic Gameplay Mechanics** — Responsive input handling and scoring tuned for satisfying rhythm gameplay.

## Tech Stack

| Component | Details |
|-----------|---------|
| Engine | Unreal Engine 5 |
| Language | C++, with supporting C# and config files |
| Platform | Windows (primary) |

## Getting Started

### Prerequisites

- [Unreal Engine 5](https://www.unrealengine.com/) (installed via the Epic Games Launcher)
- [Visual Studio 2022](https://visualstudio.microsoft.com/) with the **Game development with C++** workload
- Windows 10/11

### Building from Source

1. Clone the repository:
   ```bash
   git clone https://github.com/haganwyh/RhythmCube.git
   ```
2. Open `RhythmCube.uproject` — this will launch the project in Unreal Editor.  
   If prompted, allow the engine to rebuild modules.
3. To generate Visual Studio project files manually, right-click `RhythmCube.uproject` and select **Generate Visual Studio project files**.
4. Open the generated `.sln` file in Visual Studio, set the build configuration to **Development Editor**, and build.
5. Press **Play** in the Unreal Editor to run the game.

## Project Structure

```
RhythmCube/
├── Config/             # Engine and project configuration files
├── Content/            # Game assets (meshes, materials, blueprints, audio, maps)
├── Plugins/            # Third-party and engine plugins
│   └── VisualStudioTools/
├── Source/             # C++ source code (gameplay, audio, level systems)
├── RhythmCube.uproject # Unreal project file
└── shadertoolsconfig.json
```

## How to Play

1. Launch the game from the Unreal Editor or a packaged build.
2. Select a track/beatmap.
3. Hit the corresponding input keys as notes reach the hit zone on the 3D highway.
4. Aim for accuracy — timing determines your score.

## Custom Beatmaps

Rhythm Cube supports custom beatmaps through its level encoding system. See the `Content/` directory for examples of existing maps to use as a reference when creating your own.

## Contributing

Contributions are welcome! If you'd like to help improve Rhythm Cube:

1. Fork the repository.
2. Create a feature branch: `git checkout -b feature/your-feature`
3. Commit your changes: `git commit -m "Add your feature"`
4. Push to the branch: `git push origin feature/your-feature`
5. Open a Pull Request.

Please make sure your code compiles cleanly in UE5 before submitting.

## License

This project is open source. Please check the repository for license details.

## Acknowledgements

- Built with [Unreal Engine 5](https://www.unrealengine.com/) by Epic Games.
- Inspired by rhythm games like Beat Saber, Guitar Hero, and Muse Dash.

---

**Questions or feedback?** Open an [issue](https://github.com/haganwyh/RhythmCube/issues) on GitHub.
