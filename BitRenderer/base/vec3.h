/*
 * 长度为3的向量类
 */
#ifndef VEC3_H
#define VEC3_H

#include "common.h"

class Vec3
{
private:
	double e_[3];

public:
	Vec3() : e_{0,0,0} {}

	Vec3(double t0, double t1, double t2) : e_{t0,t1,t2} {}

	~Vec3() = default;

	// 用于设置相机外参、颜色
	Vec3(float t[3]) : e_{t[0],t[1],t[2]} {}

	Vec3(const Vec3& v) : e_{ v.e_[0],v.e_[1], v.e_[2] } {}

	Vec3& operator=(const Vec3& v) 
	{
		std::copy(std::begin(v.e_),std::end(v.e_),std::begin(e_));
		return *this;
	}

	Vec3(Vec3&& v) noexcept : e_{ v.e_[0],v.e_[1], v.e_[2] } {}

	Vec3& operator=(Vec3&& v) noexcept
	{
		std::copy(std::begin(v.e_), std::end(v.e_), std::begin(e_));
		return *this;
	}

	Vec3 operator-() const { return Vec3(-e_[0], -e_[1], -e_[2]); }
	double operator[](int i) const { return e_[i]; }
	double& operator[](int i) { return e_[i]; }

	Vec3& operator+=(const Vec3& v)
	{
		e_[0] += v.e_[0];
		e_[1] += v.e_[1];
		e_[2] += v.e_[2];
		return *this;
	}

	Vec3& operator*=(double t)
	{
		e_[0] *= t;
		e_[1] *= t;
		e_[2] *= t;
		return *this;
	}

	Vec3& operator/=(double t)
	{
		return *this *= 1 / t;
	}

public:
	double length_squared() const
	{
		return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2];
	}

	double length() const
	{
		return std::sqrt(length_squared());
	}

	bool near_zero() const
	{
		return (fabs(e_[0]) < kEpsilon) && (fabs(e_[1]) < kEpsilon) && (fabs(e_[2]) < kEpsilon);
	}

	static Vec3 random() 
	{
		return Vec3(random_double(), random_double(), random_double());
	}

	static Vec3 random(double min, double max) 
	{
		return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	double x() const
	{
		return e_[0];
	}

	double y() const
	{
		return e_[1];
	}

	double z() const
	{
		return e_[2];
	}
};

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline Vec3 operator*(double t, const Vec3& v)
{
	return Vec3(t * v.x(), t * v.y(), t * v.z());
}

inline Vec3 operator*(const Vec3& v, double t) 
{
	return t * v;
}

inline Vec3 operator/(Vec3 v, double t) 
{
	return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v) 
{
	return u.x() * v.x()
		 + u.y() * v.y()
		 + u.z() * v.z();
}

inline Vec3 cross(const Vec3& u, const Vec3& v)
{
	return Vec3(
		u.y() * v.z() - u.z() * v.y(),
		u.z() * v.x() - u.x() * v.z(),
		u.x() * v.y() - u.y() * v.x());
}

inline Vec3 unit_vector(Vec3 v) 
{
	return v / v.length();
}

inline Vec3 random_in_unit_sphere() 
{
	while (true)
	{
		auto p = Vec3::random(-1., 1.);
		if (p.length_squared() < 1)
			return p;
	}
}

// 单位球上采样单位向量
inline Vec3 random_unit_vector()
{
	return unit_vector(random_in_unit_sphere());
}

inline Vec3 random_on_hemisphere(const Vec3& normal) 
{
	Vec3 on_unit_sphere = random_unit_vector();
	// 在法向量所在的半球
	if (dot(on_unit_sphere, normal) > 0.) 
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}

// 反射，v是入射光线方向，n是法线方向
inline Vec3 reflect(const Vec3& v, const Vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

// 折射，uv入射光线方向，n是法线方向，etai_over_etat是折射率比值
// 参见RayTracingInOneWeekend 11.2
inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) 
{
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

// 模拟圆形透镜，随机采样
inline Vec3 random_in_unit_disk()
{
	while (true)
	{
		auto p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() < 1)
			return p;
	}
}

// 半球cosine采样
inline Vec3 random_cosine_direction()
{
	auto r1 = random_double();
	auto r2 = random_double();

	auto phi = 2 * kPI * r1;
	auto x = cos(phi) * sqrt(r2);
	auto y = sin(phi) * sqrt(r2);
	auto z = sqrt(1 - r2);

	return Vec3(x, y, z);
}


#endif // !VEC3_H