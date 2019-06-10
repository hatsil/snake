#include <cmath>

#include "bbox.hpp"

bool snake::BoxesCollide(const BBox& a, const BBox& b)
{
	glm::vec3 t = b.position - a.position;
	float rxx = glm::dot(a.xAxis, b.xAxis), rxy = glm::dot(a.xAxis, b.yAxis), rxz = glm::dot(a.xAxis, b.zAxis),
		ryx = glm::dot(a.yAxis, b.xAxis), ryy = glm::dot(a.yAxis, b.yAxis), ryz = glm::dot(a.yAxis, b.zAxis),
		rzx = glm::dot(a.zAxis, b.xAxis), rzy = glm::dot(a.zAxis, b.yAxis), rzz = glm::dot(a.zAxis, b.zAxis);
	float tAx = glm::dot(t, a.xAxis), tAy = glm::dot(t, a.yAxis), tAz = glm::dot(t, a.zAxis),
		tBx = glm::dot(t, b.xAxis), tBy = glm::dot(t, b.yAxis), tBz = glm::dot(t, b.zAxis);
	
	// L = Ax
	return !((std::abs(tAx) > a.width + std::abs(b.width * rxx) + std::abs(b.height * rxy) + std::abs(b.depth * rxz)) ||

		// L = Ay
		(std::abs(tAy) > a.height + std::abs(b.width * ryx) + std::abs(b.height * ryy) + std::abs(b.depth * ryz)) ||

		// L = Az
		(std::abs(tAz) > a.depth + std::abs(b.width * rzx) + std::abs(b.height * rzy) + std::abs(b.depth * rzz)) ||

		// L = Bx
		(std::abs(tBx) > b.width + std::abs(a.width * rxx) + std::abs(a.height * ryx) + std::abs(a.depth * rzx)) ||

		// L = By
		(std::abs(tBy) > b.height + std::abs(a.width * rxy) + std::abs(a.height * ryy) + std::abs(a.depth * rzy)) ||

		// L = Bz
		(std::abs(tBz) > b.depth + std::abs(a.width * rxz) + std::abs(a.height * ryz) + std::abs(a.depth * rzz)) ||

		// L = Ax x Bx
		(std::abs(tAz * ryx - tAy * rzx) > std::abs(a.height * rzx) + std::abs(a.depth * ryx) + std::abs(b.height * rxz) + std::abs(b.depth * rxy)) ||

		// L = Ax x By
		(std::abs(tAz * ryy - tAy * rzy) > std::abs(a.height * rzy) + std::abs(a.depth * ryy) + std::abs(b.width * rxz) + std::abs(b.depth * rxx)) ||

		// L = Ax x Bz
		(std::abs(tAz * ryz - tAy * rzz) > std::abs(a.height * rzz) + std::abs(a.depth * ryz) + std::abs(b.width * rxy) + std::abs(b.height * rxx)) ||

		// L = Ay x Bx
		(std::abs(tAx * rzx - tAz * rxx) > std::abs(a.width * rzx) + std::abs(a.depth * rxx) + std::abs(b.height * ryz) + std::abs(b.depth * ryy)) ||

		// L = Ay x By
		(std::abs(tAx * rzy - tAz * rxy) > std::abs(a.width * rzy) + std::abs(a.depth * rxy) + std::abs(b.width * ryz) + std::abs(b.depth * ryx)) ||

		// L = Ay x Bz
		(std::abs(tAx * rzz - tAz * rxz) > std::abs(a.width * rzz) + std::abs(a.depth * rxz) + std::abs(b.width * ryy) + std::abs(b.height * ryx)) ||

		// L = Az x Bx
		(std::abs(tAy * rxx - tAx * ryx) > std::abs(a.width * ryx) + std::abs(a.height * rxx) + std::abs(b.height * rzz) + std::abs(b.depth * rzy)) ||

		// L = Az x By
		(std::abs(tAy * rxy - tAx * ryy) > std::abs(a.width * ryy) + std::abs(a.height * rxy) + std::abs(b.width * rzz) + std::abs(b.depth * rzx)) ||

		// L = Az x Bz
		(std::abs(tAy * rxz - tAx * ryz) > std::abs(a.width * ryz) + std::abs(a.height * rxz) + std::abs(b.width * rzy) + std::abs(b.height * rzx)));
}

bool snake::VertexTriangleIntersect(const glm::vec3& vertex, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	// Compute vectors        
	glm::vec3 v0 = c - a, v1 = b - a, v2 = vertex - a;

	// Compute dot products
	float dot00 = glm::dot(v0, v0),
		dot01 = glm::dot(v0, v1),
		dot02 = glm::dot(v0, v2),
		dot11 = glm::dot(v1, v1),
		dot12 = glm::dot(v1, v2);

	// Compute barycentric coordinates
	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01),
		u = (dot11 * dot02 - dot01 * dot12) * invDenom,
		v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return (u >= 0) && (v >= 0) && (u + v < 1);
}
