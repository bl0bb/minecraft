#include "ecs.h"

struct Position { float x, y; };
struct Velocity { float dx, dy; };

class MovementSystem : public System {
public:
    void Init() override {
        std::cout << "[MovementSystem] Initialized\n";
    }

    void Destroy() override {
        std::cout << "[MovementSystem] Cleaned up\n";
    }

    void Tick(ComponentManager& components) override {
        for (Entity e : entities) {
            if (components.HasComponent<Position>(e) && components.HasComponent<Velocity>(e)) {
                auto& pos = components.GetComponent<Position>(e);
                auto& vel = components.GetComponent<Velocity>(e);
                pos.x += vel.dx;
                pos.y += vel.dy;
            }
        }
    }

    void Render(ComponentManager& components) override {
        for (Entity e : entities) {
            if (components.HasComponent<Position>(e)) {
                auto& pos = components.GetComponent<Position>(e);
                std::cout << "Entity " << e << " at (" << pos.x << ", " << pos.y << ")\n";
            }
        }
    }
};

int main() {
    ECS ecs;

    auto movementSystem = std::make_shared<MovementSystem>();
    ecs.systemManager.AddSystem(movementSystem);

    Entity e1 = ecs.CreateEntity();
    ecs.AddComponent(e1, Position{0, 0});
    ecs.AddComponent(e1, Velocity{1, 1});
    movementSystem->entities.insert(e1);

    Entity e2 = ecs.CreateEntity();
    ecs.AddComponent(e2, Position{5, 5});
    ecs.AddComponent(e2, Velocity{-0.5f, 0.25f});
    movementSystem->entities.insert(e2);

    for (int frame = 0; frame < 3; ++frame) {
        std::cout << "--- Frame " << frame << " ---\n";
        ecs.Tick();
        ecs.Render();
    }

    ecs.Shutdown();
    return 0;
}