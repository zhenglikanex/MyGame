#pragma once

#include <memory>
#include <limits>

#include "Primitive.hpp"

#include "glm/glm.hpp"
#include "Framework/Game/Math.hpp"

struct Contact
{
	void* body[2];
	glm::vec3 point;
};

// Given point p, return point q on (or in) OBB b, closest to p
void ClosestPtPointOBB(const vec3& p,const OBB& b, vec3& q)
{
    vec3 d = p - b.c;
    // Start result at center of box; make steps from there
    q = b.c;
    // For each OBB axis...
    for (int i = 0; i < 3; i++) {
        // ...project d onto that axis to get the distance
        // along the axis of d from the box center
        fixed16 dist = glm::dot(d, b.u[i]);
        // If distance farther than the box extents, clamp to the box
        if (dist > b.e[i]) dist = b.e[i];
        if (dist < -b.e[i]) dist = -b.e[i];
        // Step that distance along the axis to get world coordinate
        q += dist * b.u[i];
    }
}

// Computes the square distance between a point p and an AABB b
fixed16 SqDistPointAABB(const vec3& p, const AABB& b)
{
    fixed16 sqDist = fixed16(0.0f);
    for (int i = 0; i < 3; i++) {
        // For each axis count any excess distance outside box extents
        fixed16 v = p[i];
        fixed16 min = b.c[i] - b.r[i];
        fixed16 max = b.c[i] + b.r[i];
        if (v < min) sqDist += (min - v) * (min - v);
        if (v > max) sqDist += (v - max) * (v - max);
    }
    return sqDist;
}

// Given point p, return the point q on or in AABB b, that is closest to p
void ClosestPtPointAABB(vec3 p, AABB b, vec3& q)
{
    // For each coordinate axis, if the point coordinate value is
    // outside box, clamp it to the box, else keep it as is
    for (int i = 0; i < 3; i++) {
        fixed16 v = p[i];
        fixed16 min = b.c[i] - b.r[i];
        fixed16 max = b.c[i] + b.r[i];
        if (v < min) v = min; // v = max(v, b.min[i])
        if (v > max) v = max; // v = min(v, b.max[i])
        q[i] = v;
    }
}

fixed16 ClosestPtSegmentSegment(vec3 p1, vec3 q1, vec3 p2, vec3 q2,
    fixed16& s, fixed16& t, vec3& c1, vec3& c2)
{
    vec3 d1 = q1 - p1; // Direction vector of segment S1
    vec3 d2 = q2 - p2; // Direction vector of segment S2
    vec3 r = p1 - p2;
    fixed16 a = glm::dot(d1, d1); // Squared length of segment S1, always nonnegative
    fixed16 e = glm::dot(d2, d2); // Squared length of segment S2, always nonnegative
    fixed16 f = glm::dot(d2, r);

    // Check if either or both segments degenerate into points
    if (a <= std::numeric_limits<fixed16>::epsilon() && e <= std::numeric_limits<fixed16>::epsilon()) {
        // Both segments degenerate into points
        s = t = fixed16(0.0f);
        c1 = p1;
        c2 = p2;
        return glm::dot(c1 - c2, c1 - c2);
    }
    if (a <= std::numeric_limits<fixed16>::epsilon()) {
        // First segment degenerates into a point
        s = fixed16(0.0f);
        t = f / e; // s = 0 => t = (b*s + f) / e = f / e
        t = std::clamp(t, fixed16(0.0f), fixed16(1.0f));
    }
    else {
        fixed16 c = glm::dot(d1, r);
        if (e <= std::numeric_limits<fixed16>::epsilon()) {
            // Second segment degenerates into a point
            t = fixed16(0.0f);
            s = std::clamp(-c / a, fixed16(0.0f), fixed16(1.0f)); // t = 0 => s = (b*t - c) / a = -c / a
        }
        else {
            // The general nondegenerate case starts here
            fixed16 b = glm::dot(d1, d2);
            fixed16 denom = a * e - b * b; // Always nonnegative

            // If segments not parallel, compute closest point on L1 to L2, and
            // clamp to segment S1. Else pick arbitrary s (here 0)
            if (denom != fixed16(0.0f)) {
                s = std::clamp((b * f - c * e) / denom, fixed16(0.0f), fixed16(1.0f));
            }
            else s = fixed16(0.0f);

            // Compute point on L2 closest to S1(s) using
            // t = glm::dot((P1+D1*s)-P2,D2) / glm::dot(D2,D2) = (b*s + f) / e
            t = (b * s + f) / e;

            // If t in [0,1] done. Else clamp t, recompute s for the new value
            // of t using s = glm::dot((P2+D2*t)-P1,D1) / glm::dot(D1,D1)= (t*b - c) / a
            // and clamp s to [0, 1]
            if (t < fixed16(0.0f)) {
                t = fixed16(0.0f);
                s = std::clamp(-c / a, fixed16(0.0f), fixed16(1.0f));
            }
            else if (t > fixed16(1.0f)) {
                t = fixed16(1.0f);
                s = std::clamp((b - c) / a, fixed16(0.0f), fixed16(1.0f));
            }
        }
    }

    c1 = p1 + d1 * s;
    c2 = p2 + d2 * t;
    return glm::dot(c1 - c2, c1 - c2);
}

fixed16 SqDistPointSegment(vec3 a, vec3 b, vec3 c)
{
    vec3 ab = b - a, ac = c - a, bc = c - b;
    fixed16 e = glm::dot(ac, ab);
    // Handle cases where c projects outside ab
    if (e <= fixed16(0.0f)) return glm::dot(ac, ac);
    fixed16 f = glm::dot(ab, ab);
    if (e >= f) return glm::dot(bc, bc);
    // Handle case where c projects onto ab
    return glm::dot(ac, ac) - e * e / f;
}

int TestCollision(const Sphere& a, const Sphere& b)
{
	// 中心点距离是否超过半径之和
	auto d = a.c - b.c;
	auto dist2 = glm::dot(d, d);
	auto radius_sum = a.r + b.r;
	// 避免开平方操作
	return dist2 <= radius_sum * radius_sum; 
}

int TestCollision(const Circular& a, const Circular& b)
{
	auto deltaX = a.c.x - b.c.x;
	auto deltaY = a.c.y - b.c.y;
	auto disSqrt = deltaX * deltaX + deltaY * deltaY;

    auto radiusSum = a.r + b.r;
    return disSqrt < (radiusSum* radiusSum);
}

int TestCollision(const AABB& a, const AABB& b)
{
    if (fpm::abs(a.c[0] - b.c[0]) > (a.r[0] + b.r[0])) return 0;
    if (fpm::abs(a.c[1] - b.c[1]) > (a.r[1] + b.r[1])) return 0;
    if (fpm::abs(a.c[2] - b.c[2]) > (a.r[2] + b.r[2])) return 0;
    return 1;
}

int TestCollision(const OBB& a, const OBB& b)
{
    fixed16 ra, rb;
    mat3 R(fixed16(0.0f)), AbsR(fixed16(0.0f));

    // Compute rotation matrix expressing b in a's coordinate frame
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            R[i][j] = glm::dot(a.u[i], b.u[j]);

    // Compute translation vector t
    vec3 t = b.c - a.c;
    // Bring translation into a's coordinate frame
    t = vec3(glm::dot(t, a.u[0]), glm::dot(t, a.u[1]), glm::dot(t, a.u[2]));

    // Compute common subexpressions. Add in an epsilon term to
    // counteract arithmetic errors when two edges are parallel and
    // their cross product is (near) null (see text for details)
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            AbsR[i][j] = fpm::abs(R[i][j]) + std::numeric_limits<fixed16>::epsilon();

    // Test axes L = A0, L = A1, L = A2
    for (int i = 0; i < 3; i++) {
        ra = a.e[i];
        rb = b.e[0] * AbsR[i][0] + b.e[1] * AbsR[i][1] + b.e[2] * AbsR[i][2];
        if (fpm::abs(t[i]) > ra + rb) return 0;
    }

    // Test axes L = B0, L = B1, L = B2
    for (int i = 0; i < 3; i++) {
        ra = a.e[0] * AbsR[0][i] + a.e[1] * AbsR[1][i] + a.e[2] * AbsR[2][i];
        rb = b.e[i];
        if (fpm::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return 0;
    }

    // Test axis L = A0 x B0
    ra = a.e[1] * AbsR[2][0] + a.e[2] * AbsR[1][0];
    rb = b.e[1] * AbsR[0][2] + b.e[2] * AbsR[0][1];
    if (fpm::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return 0;

    // Test axis L = A0 x B1
    ra = a.e[1] * AbsR[2][1] + a.e[2] * AbsR[1][1];
    rb = b.e[0] * AbsR[0][2] + b.e[2] * AbsR[0][0];
    if (fpm::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return 0;

    // Test axis L = A0 x B2
    ra = a.e[1] * AbsR[2][2] + a.e[2] * AbsR[1][2];
    rb = b.e[0] * AbsR[0][1] + b.e[1] * AbsR[0][0];
    if (fpm::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return 0;

    // Test axis L = A1 x B0
    ra = a.e[0] * AbsR[2][0] + a.e[2] * AbsR[0][0];
    rb = b.e[1] * AbsR[1][2] + b.e[2] * AbsR[1][1];
    if (fpm::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return 0;

    // Test axis L = A1 x B1
    ra = a.e[0] * AbsR[2][1] + a.e[2] * AbsR[0][1];
    rb = b.e[0] * AbsR[1][2] + b.e[2] * AbsR[1][0];
    if (fpm::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return 0;

    // Test axis L = A1 x B2
    ra = a.e[0] * AbsR[2][2] + a.e[2] * AbsR[0][2];
    rb = b.e[0] * AbsR[1][1] + b.e[1] * AbsR[1][0];
    if (fpm::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return 0;

    // Test axis L = A2 x B0
    ra = a.e[0] * AbsR[1][0] + a.e[1] * AbsR[0][0];
    rb = b.e[1] * AbsR[2][2] + b.e[2] * AbsR[2][1];
    if (fpm::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return 0;

    // Test axis L = A2 x B1
    ra = a.e[0] * AbsR[1][1] + a.e[1] * AbsR[0][1];
    rb = b.e[0] * AbsR[2][2] + b.e[2] * AbsR[2][0];
    if (fpm::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return 0;

    // Test axis L = A2 x B2
    ra = a.e[0] * AbsR[1][2] + a.e[1] * AbsR[0][2];
    rb = b.e[0] * AbsR[2][1] + b.e[1] * AbsR[2][0];
    if (fpm::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return 0;

    // Since no separating axis found, the OBBs must be intersecting
    return 1;
}

int TestCollision(const Sphere& s, const Capsule& capsule)
{
    // Compute (squared) distance between sphere center and capsule line segment
    fixed16 dist2 = SqDistPointSegment(capsule.a, capsule.b, s.c);

    // If (squared) distance smaller than (squared) sum of radii, they collide
    fixed16 radius = s.r + capsule.r;
    return dist2 <= radius * radius;
}

int TestCollision(const Capsule& capsule1,const Capsule& capsule2)
{
    // Compute (squared) distance between the inner structures of the capsules
    fixed16 s, t;
    vec3 c1(fixed16(0)), c2(fixed16(0));
    fixed16 dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b,
        capsule2.a, capsule2.b, s, t, c1, c2);

    // If (squared) distance smaller than (squared) sum of radii, they collide
    auto radius = capsule1.r + capsule2.r;
    return dist2 <= radius * radius;
}

// Returns true if sphere s intersects AABB b, false otherwise
int TestCollision(const Sphere& s, const AABB& b)
{
    // Compute squared distance between sphere center and AABB
    fixed16 sqDist = SqDistPointAABB(s.c, b);

    // Sphere and AABB intersect if the (squared) distance
    // between them is less than the (squared) sphere radius
    return sqDist <= s.r * s.r;
}

int TestCollision(const Sphere& s, const AABB& b, vec3& p)
{
    // Find point p on AABB closest to sphere center
    ClosestPtPointAABB(s.c, b, p);

    // Sphere and AABB intersect if the (squared) distance from sphere
    // center to point p is less than the (squared) sphere radius
    vec3 v = p - s.c;
    return glm::dot(v, v) <= s.r * s.r;
}

// Returns true if sphere s intersects OBB b, false otherwise.
// The point p on the OBB closest to the sphere center is also returned
int TestCollision(const Sphere& s,const OBB& b,vec3& p)
{
    // Find point p on OBB closest to sphere center
    ClosestPtPointOBB(s.c, b, p);

    // Sphere and OBB intersect if the (squared) distance from sphere
    // center to point p is less than the (squared) sphere radius
    vec3 v = p - s.c;
    return glm::dot(v, v) <= s.r * s.r;
}

// Intersect ray R(t) = p + t*d against AABB a. When intersecting,
// return intersection distance tmin and point q of intersection
int IntersectRayAABB(const vec3& p, const vec3& d,const AABB& a, fixed16& tmin, vec3& q)
{
    //tmin = fixed16(0.0f);          // set to -FLT_MAX to get first hit on line
    //fixed16 tmax = std::numeric_limits<fixed16>::max(); // set to max distance ray can travel (for segment)

    //// For all three slabs
    //for (int i = 0; i < 3; i++) {
    //    fixed16 min = a.c[i] - a.r[i];
    //    fixed16 max = a.c[i] + a.r[i];
    //    if (fpm::abs(d[i]) < std::numeric_limits<fixed16>::epsilon()) {
    //        // Ray is parallel to slab. No hit if origin not within slab
    //        if (p[i] < min || p[i] > max) return 0;
    //    }
    //    else {
    //        // Compute intersection t value of ray with near and far plane of slab
    //        fixed16 ood = fixed16(1.0f) / d[i];
    //        fixed16 t1 = (min - p[i]) * ood;
    //        fixed16 t2 = (max - p[i]) * ood;
    //        // Make t1 be intersection with near plane, t2 with far plane
    //        if (t1 > t2) std::swap(t1, t2);
    //        // Compute the intersection of slab intersections intervals
    //        tmin = std::max(tmin, t1);
    //        tmax = std::min(tmax, t2);
    //        // Exit with no collision as soon as slab intersection becomes empty
    //        if (tmin > tmax) return 0;
    //    }
    //}
    //// Ray intersects all 3 slabs. Return point (q) and intersection t value (tmin) 
    //q = p + d * tmin;
    return 1;
}

// Test if segment specified by points p0 and p1 intersects AABB b
int TestSegmentAABB(const vec3& p0, const vec3& p1, const AABB& b)
{
    vec3 e = b.r;             // Box halflength extents
    vec3 m = (p0 + p1) * fixed16(0.5f) - b.c;       // Segment midpoint Translate box and segment to origin
    vec3 d = p1 - m;                // Segment halflength vector

    // Try world coordinate axes as separating axes
    fixed16 adx = fpm::abs(d.x);
    if (fpm::abs(m.x) > e.x + adx) return 0;
    fixed16 ady = fpm::abs(d.y);
    if (fpm::abs(m.y) > e.y + ady) return 0;
    fixed16 adz = fpm::abs(d.z);
    if (fpm::abs(m.z) > e.z + adz) return 0;

    // Add in an epsilon term to counteract arithmetic errors when segment is
    // (near) parallel to a coordinate axis (see text for detail)
    adx += std::numeric_limits<fixed16>::epsilon() ; ady += std::numeric_limits<fixed16>::epsilon(); adz += std::numeric_limits<fixed16>::epsilon();

    // Try cross products of segment direction vector with coordinate axes
    if (fpm::abs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady) return 0;
    if (fpm::abs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx) return 0;
    if (fpm::abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return 0;

    // No separating axis found; segment must be overlapping AABB
    return 1;
}