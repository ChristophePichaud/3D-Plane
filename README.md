# 3D-Plane

Simple C++17 3D plane demo using **raylib** (OpenGL wrapper), compatible with Linux and Windows.

## Features

- Rear camera view of a plane.
- Plane controls:
  - **Left / Right arrows**: horizontal move.
  - **Up / Down arrows**: altitude control.
- Constant forward flight simulated by moving terrain on the Z axis.
- Procedural basic 3D mountain-like shapes.
- Simple physics engine for each control axis (position, speed, acceleration).

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/plane_3d
```

On Windows (Visual Studio generator), run the produced `plane_3d.exe`.

## Physics engine (short technical note)

The plane uses two independent physics axes:

- **X axis** for left/right movement.
- **Y axis** for up/down movement.

For each frame:

1. Arrow keys set acceleration.
2. Velocity is integrated with `velocity += acceleration * dt`.
3. Velocity is damped (friction-like) and clamped to a max speed.
4. Position is integrated with `position += velocity * dt`.
5. Position is clamped to safe flight bounds.

This keeps movement smooth and frame-rate independent.
