#include "brl_camera.hpp"
#include "brl_viewport.hpp"

namespace brl {
    void updateCamera(Camera *camera, const Window *window) {
        bool middleMouse = 
        glfwGetMouseButton(window->glfwWindow, GLFW_MOUSE_BUTTON_MIDDLE) || 
        (glfwGetMouseButton(window->glfwWindow, GLFW_MOUSE_BUTTON_LEFT) && glfwGetKey(window->glfwWindow, GLFW_KEY_LEFT_ALT));

        if (middleMouse && !glfwGetKey(window->glfwWindow, GLFW_KEY_LEFT_SHIFT)) {
            camera->yaw -= window->deltaMousePos.x*0.0075f;
            camera->pitch -= window->deltaMousePos.y*0.0075f;
        }   

        if (middleMouse && glfwGetKey(window->glfwWindow, GLFW_KEY_LEFT_SHIFT)) {
            smath::vector3 movement = smath::vector3{-window->deltaMousePos.x,window->deltaMousePos.y,0.0f};
            smath::quaternion cameraOrientation = calculateCameraOrientation(*camera);

            movement = smath::quaternion_transform_vector(cameraOrientation, movement);
            movement *= 0.00125f;

            camera->focus -= movement*camera->distance;
        }   

        camera->distance -= window->scrollInput*camera->distance*0.075f;
    }

    Camera createCamera(smath::vector3 focus, float distance, float fov, float near, float far, float pitch, float yaw) {
        Camera camera;

        camera.focus = focus;
        camera.distance = distance;

        camera.fov = fov;
        camera.near = near;
        camera.far = far;

        camera.aspect = 1.0f;
        camera.pitch = pitch;
        camera.yaw = yaw;

        return camera;
    }

    smath::quaternion calculateCameraOrientation(const Camera &camera) {
        return smath::quaternion_from_euler_angles_ZYX(0.0f, camera.yaw, camera.pitch);
    }

    smath::vector3 calculateCameraPosition(const Camera &camera) {
        smath::quaternion orientation = calculateCameraOrientation(camera);

        smath::vector3 forwardVector = smath::quaternion_transform_vector(orientation, smath::vector3{0.0f, 0.0f, 1.0f});
        smath::vector3 position = camera.focus - (forwardVector * camera.distance);

        return position;
    }

    smath::matrix4x4 calculateCameraView(const Camera &camera) {
        
        using namespace smath;

        quaternion orientation = calculateCameraOrientation(camera);

        vector3 forwardVector = quaternion_transform_vector(orientation, vector3{0.0f, 0.0f, 1.0f});
        vector3 position = camera.focus - (forwardVector * camera.distance);
        matrix3x3 rotationMatrix = matrix3x3_from_quaternion(orientation);

        matrix4x4 transformationMatrix = matrix4x4_from_matrix3x3(transpose(rotationMatrix));

        position = matrix3x3_transform_vector3(transpose(rotationMatrix), position);

        transformationMatrix[0][3] = position.x;
        transformationMatrix[1][3] = position.y;
        transformationMatrix[2][3] = position.z;

        return transformationMatrix;
    }

    smath::matrix4x4 calculateCameraProjection(const Camera &camera) {
        return smath::matrix4x4_from_perspective(camera.fov, camera.aspect, camera.near, camera.far);
    }
}