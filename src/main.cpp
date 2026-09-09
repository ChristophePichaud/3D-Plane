#include "raylib.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

struct PhysicsAxisState {
    float position{};
    float velocity{};
    float acceleration{};
};

struct PlanePhysics {
    PhysicsAxisState x{};
    PhysicsAxisState y{};
};

struct TerrainPeak {
    float x{};
    float z{};
    float radius{};
    float height{};
};

static float DampenVelocity(float velocity, float factor) {
    if (std::fabs(velocity) < 0.0001f) {
        return 0.0f;
    }
    return velocity * factor;
}

int main() {
    constexpr int kScreenWidth = 1280;
    constexpr int kScreenHeight = 720;
    constexpr float kForwardSpeed = 22.0f;
    constexpr float kInputAcceleration = 45.0f;
    constexpr float kFrictionFactor = 0.92f;
    constexpr float kMaxSpeed = 15.0f;
    constexpr float kXLimit = 20.0f;
    constexpr float kYMin = 2.5f;
    constexpr float kYMax = 18.0f;

    InitWindow(kScreenWidth, kScreenHeight, "3D Plane");
    SetTargetFPS(60);

    Camera3D camera{};
    camera.position = {0.0f, 8.0f, 20.0f};
    camera.target = {0.0f, 6.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    PlanePhysics plane{};
    plane.y.position = 6.0f;

    std::vector<TerrainPeak> peaks;
    peaks.reserve(60);
    for (int i = 0; i < 60; ++i) {
        const float x = GetRandomValue(-28, 28);
        const float z = -GetRandomValue(20, 620);
        const float radius = static_cast<float>(GetRandomValue(3, 9));
        const float height = static_cast<float>(GetRandomValue(5, 20));
        peaks.push_back({x, z, radius, height});
    }

    float terrainOffset = 0.0f;

    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();

        plane.x.acceleration = 0.0f;
        plane.y.acceleration = 0.0f;

        if (IsKeyDown(KEY_LEFT)) plane.x.acceleration -= kInputAcceleration;
        if (IsKeyDown(KEY_RIGHT)) plane.x.acceleration += kInputAcceleration;
        if (IsKeyDown(KEY_UP)) plane.y.acceleration += kInputAcceleration;
        if (IsKeyDown(KEY_DOWN)) plane.y.acceleration -= kInputAcceleration;

        plane.x.velocity += plane.x.acceleration * dt;
        plane.y.velocity += plane.y.acceleration * dt;

        plane.x.velocity = std::clamp(DampenVelocity(plane.x.velocity, kFrictionFactor), -kMaxSpeed, kMaxSpeed);
        plane.y.velocity = std::clamp(DampenVelocity(plane.y.velocity, kFrictionFactor), -kMaxSpeed, kMaxSpeed);

        plane.x.position = std::clamp(plane.x.position + plane.x.velocity * dt, -kXLimit, kXLimit);
        plane.y.position = std::clamp(plane.y.position + plane.y.velocity * dt, kYMin, kYMax);

        terrainOffset += kForwardSpeed * dt;

        const Vector3 planePos{plane.x.position, plane.y.position, 0.0f};
        camera.target = {planePos.x, planePos.y + 0.6f, planePos.z - 10.0f};
        camera.position = {planePos.x, planePos.y + 4.0f, planePos.z + 20.0f};

        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);

        DrawPlane({0.0f, 0.0f, -300.0f}, {220.0f, 820.0f}, DARKGREEN);

        for (const TerrainPeak& peak : peaks) {
            float z = peak.z + std::fmod(terrainOffset, 640.0f);
            if (z > 40.0f) z -= 640.0f;

            const Vector3 baseCenter{peak.x, 0.0f, z};
            const Vector3 topCenter{peak.x, peak.height, z};
            DrawCylinderEx(baseCenter, topCenter, peak.radius, 0.5f, 8, BROWN);
        }

        DrawCube({planePos.x, planePos.y, planePos.z}, 1.2f, 0.5f, 3.6f, RED);
        DrawCube({planePos.x, planePos.y + 0.35f, planePos.z + 1.0f}, 0.7f, 0.45f, 1.2f, MAROON);
        DrawTriangle3D(
            {planePos.x, planePos.y + 0.2f, planePos.z - 2.0f},
            {planePos.x - 3.0f, planePos.y + 0.1f, planePos.z - 0.3f},
            {planePos.x + 3.0f, planePos.y + 0.1f, planePos.z - 0.3f},
            RED
        );

        EndMode3D();

        DrawText("LEFT/RIGHT: move plane sideways", 20, 20, 20, BLACK);
        DrawText("UP/DOWN: control altitude", 20, 45, 20, BLACK);
        DrawFPS(kScreenWidth - 110, 15);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
