#include <iostream>
#include <math.h>

bool isEQ(float a, float b, float eps = 1.0e7)
{
  return std::fabs(a-b) < eps;
}

struct Vec2 
{
  float x;
  float y;

  Vec2 GetNormalized() const
  {
    float len = sqrt(x * x + y * y);
    return {x / len, y / len};
  }

  Vec2 GetNormal() const
  {
    Vec2 n = {y, -x};
    return n.GetNormalized();
  }
};

float DotProduct(Vec2 pt1, Vec2 pt2)
{
  return pt1.x * pt2.x + pt1.y * pt2.y;
}

Vec2 operator*(Vec2 pt, float a)
{
  return {pt.x * a, pt.y * a};
}

Vec2 operator*(float a, Vec2 pt)
{
  return {pt.x * a, pt.y * a};
}

Vec2 operator*(Vec2 pt1, Vec2 pt2)
{
  return {pt1.x * pt2.x, pt1.y * pt2.y};
}

Vec2 operator-(Vec2 pt1, Vec2 pt2)
{
  return {pt1.x - pt2.x, pt1.y - pt2.y};
}

Vec2 operator+(Vec2 pt1, Vec2 pt2)
{
  return {pt1.x + pt2.x, pt1.y + pt2.y};
}

struct Ray
{
  Vec2 point; // starting point
  Vec2 direction;
};

struct Line
{
  float a;
  float b;
  float c;
};

Line GetLineBy2Pts(const Vec2& pt1, const Vec2& pt2)
{
  const float a = pt1.y - pt2.y;
  const float b = pt2.x - pt1.x;
  const float c = -a * pt1.x - b * pt1.y;
  return {a, b, c};
}

Vec2 FindIntersection(const Line& l1,const Line& l2)
{
  float determinant = l1.a * l2.b - l2.a * l1.b;
  if(isEQ(determinant, 0.f))
  {
    return {0.f, 0.f};
  }
  return {(l2.c * l1.b - l1.c * l2.b)/determinant,
    (l2.a * l1.c - l2.c * l1.a) /determinant};
}

Vec2 Reflect(const Ray& r1, const Ray& r2)
{
  Line l1 = GetLineBy2Pts(r1.point, r1.point + r1.direction);
  Line l2 = GetLineBy2Pts(r2.point, r2.point + r2.direction);
  Vec2 CrossPt = FindIntersection(l1, l2);
  if(r1.direction.x * CrossPt.x > 0.f ||
  r1.direction.y * CrossPt.y > 0.f) // has same sign
  {
    Vec2 norm = r1.direction.GetNormal();
    return r1.direction - 2 * (DotProduct(r1.direction, norm)) * norm; 
  }
}
