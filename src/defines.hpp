#ifndef ____ISICG_ISIR_DEFINES__
#define ____ISICG_ISIR_DEFINES__

#include "glm/glm.hpp"

namespace ISICG_ISIR
{
	using uint = unsigned int;
	using uchar = unsigned char;

	// glm::vec attributes can be accessed with .x/y/z/w or .r/g/b/a
	// Use the second method for colours :-)
	using Vec3f = glm::vec3;
	using Vec2f = glm::vec2;
	using Vec4f = glm::vec4;
	using Vec3u = glm::uvec3;

	const Vec3f VEC3F_ZERO = Vec3f(0.f, 0.f, 0.f);
	const Vec3f VEC3F_ONE = Vec3f(1.0f,1.0f,1.0f);
	 
} // namespace ISICG_ISIR

#endif // ____ISICG_ISIR_DEFINES__
