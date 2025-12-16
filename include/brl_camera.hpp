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

    void update_camera(Camera* camera, const Window* window);

    Camera create_camera(smath::vector3 focus, float distance, float fov, float near, float far, float pitch = 0.0f, float yaw = 0.0f);

    smath::quaternion calculate_camera_orientation(const Camera &camera);

    smath::vector3 calculate_camera_position(const Camera &camera);

    smath::matrix4x4 calculate_camera_view(const Camera &camera);

    smath::matrix4x4 calculate_camera_projection(const Camera &camera);
}

#endif