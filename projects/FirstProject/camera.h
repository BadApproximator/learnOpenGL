#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    CAM_FORWARD = (1 << 0),
    CAM_BACKWARD = (1 << 1),
    CAM_LEFT = (1 << 2),
    CAM_RIGHT = (1 << 3),
    CAM_UP = (1 << 4),
    CAM_DOWN = (1 << 5),
};

// Default camera values
const float YAW = 90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f; // чувствительность
const float ZOOM = 45.0f;

const float FOV = 45.0f;
const float ZNEAR = 0.1f; // клипинг
const float ZFAR = 1000.0f;
const float ASPECTRATIO = 16.f/9.f; // соотношение сторон


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float Fov;
    float zNear;
    float zFar;
    float AspectRatio;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW, float pitch = PITCH) : 
        Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
        Fov(FOV), zNear(ZNEAR), 
        MovementSpeed(SPEED), zFar(ZFAR), AspectRatio(ASPECTRATIO),
        MouseSensitivity(SENSITIVITY), 
        Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, 
        float upX, float upY, float upZ, 
        float yaw, float pitch) : 
        Fov(FOV), zNear(ZNEAR),
        MovementSpeed(SPEED), zFar(ZFAR), AspectRatio(ASPECTRATIO),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetProjectionMatrix()
    {
        return glm::perspective(glm::radians(Fov), AspectRatio, zNear, zFar);
    }


    void Move(int32_t dirs, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        glm::vec3 direction = glm::vec3(0.f, 0.f, 0.f);
        direction.z = (dirs & CAM_FORWARD) / CAM_FORWARD - (dirs & CAM_BACKWARD) / CAM_BACKWARD;
        direction.x = (dirs & CAM_RIGHT) / CAM_RIGHT - (dirs & CAM_LEFT) / CAM_LEFT;
        direction.y = (dirs & CAM_UP) / CAM_UP - (dirs & CAM_DOWN) / CAM_DOWN;
        glm::normalize(direction);

        Position += Front * velocity * direction.z;
        Position += Right * velocity * direction.x;
        Position += Up * velocity * direction.y;
        updateCameraVectors();
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == CAM_FORWARD)
            Position += Front * velocity;
        if (direction == CAM_BACKWARD)
            Position -= Front * velocity;
        if (direction == CAM_LEFT)
            Position -= Right * velocity;
        if (direction == CAM_RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void Rotate(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void ChangeFov(float value)
    {
        Fov -= (float)value;
        if (Fov < 1.0f)
            Fov = 1.0f;
        if (Fov > 120.0f)
            Fov = 120.0f;
        updateCameraVectors();
    }


    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif