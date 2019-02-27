#include <vector>

struct Point2d {
    float x, y;
};

const float FPS = 60.f;

const float HORIZONTAL_VELOCITY = 0.6 / FPS;
const float VERTICAL_ACCELERATION = 9.8 / FPS;
const float JUMP_ACCELERATION = VERTICAL_ACCELERATION * 2.f;

struct LevelDescription {
    /// Total length of the level
    float length;
    /// Total height of the level
    float height;
    struct Pylon {
        /// Center is between [0, 0] and [width, height]
        /// Refers to the point in the middle of the gap between top and bottom part of the pylon
        Point2d center;
        /// Width of the pylon
        float width;
        /// The hight of the gap between top and bottom part of the pylon
        float gapHeight;
    };

    /// Each pylon in the level
    std::vector<Pylon> pylons;
};

/// Expected number of decisions is (level.width / (HORIZONTAL_VELOCITY * FPS))
std::vector<bool> getAgentDecisions(const LevelDescription &level);
