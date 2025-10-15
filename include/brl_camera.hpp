#ifndef BRL_CAMERA
#define BRL_CAMERA

#include "smath.hpp"

#include "brl_window.hpp"

namespace brl {
    struct Camera {
        smath::vector3 focus;

        float distance;
        float pitch, yaw;
        float aspect, fov, near, far;
    };

    void updateCamera(Camera* camera, const Window* window);

    Camera createCamera(smath::vector3 focus, float distance, float fov, float near, float far, float pitch = 0.0f, float yaw = 0.0f);

    smath::quaternion calculateCameraOrientation(const Camera &camera);

    smath::vector3 calculateCameraPosition(const Camera &camera);

    smath::matrix4x4 calculateCameraView(const Camera &camera);

    smath::matrix4x4 calculateCameraProjection(const Camera &camera);
}

#endif