#ifndef MATH_H
#define MATH_H
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Real-Engine/Core/Core.h"
#include "glm/ext/matrix_clip_space.hpp"
namespace Real
{
    typedef glm::vec2 Vec2;
    typedef glm::vec3 Vec3;
    typedef glm::vec4 Vec4;
    typedef glm::mat2 Mat2;
    typedef glm::mat3 Mat3;
    typedef glm::mat4 Mat4;

    bool decompose(const Mat4& transform, Vec3& translation, Vec3& rotation, Vec3& scale);
}
#endif //MATH_H
