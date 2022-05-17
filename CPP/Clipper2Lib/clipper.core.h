/*******************************************************************************
* Author    :  Angus Johnson                                                   *
* Version   :  10.0 (beta) - aka Clipper2                                      *
* Date      :  18 May 2022                                                     *
* Website   :  http://www.angusj.com                                           *
* Copyright :  Angus Johnson 2010-2022                                         *
* Purpose   :  Core Clipper Library structures and functions                   *
* License   :  http://www.boost.org/LICENSE_1_0.txt                            *
*******************************************************************************/

#ifndef CLIPPER_CORE_H
#define CLIPPER_CORE_H

#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include < algorithm >

namespace Clipper2Lib 
{

//#define REVERSE_ORIENTATION
//#define USINGZ

	static double const PI = 3.141592653589793238;

// Point ------------------------------------------------------------------------

template <typename T>
struct Point {
	T x;
	T y;

	template <typename T2>
	//inline 
		void Init(const T2 x_ = 0, const T2 y_ = 0)
	{
		if (std::numeric_limits<T>::is_integer &&
			!std::numeric_limits<T2>::is_integer)
		{
			x = static_cast<T>(std::round(x_));
			y = static_cast<T>(std::round(y_));
		}
		else
		{
			x = static_cast<T>(x_);
			y = static_cast<T>(y_);
		}
	}

#ifdef USINGZ
	T z;

	explicit Point() : x(0), y(0), z(0) {};

	template <typename T2>
	explicit Point(const T2 x_ = 0, const T2 y_ = 0)
	{
		Init(x_, y_);
		z = 0;
	}

	template <typename T2>
	explicit Point<T>(const Point<T2>& p) 
	{ 
		Init(p.x, p.y);
		z = 0;
	}

	friend std::ostream& operator<<(std::ostream& os, const Point& point)
	{
		os << point.x << "," << point.y << "," << point.z;
		return os;
	}

#else

	explicit Point() : x(0), y(0) {};

	template <typename T2>
	explicit Point(const T2 x_ = 0, const T2 y_ = 0) { Init(x_, y_); }

	template <typename T2>
	explicit Point<T>(const Point<T2>& p) { Init(p.x, p.y); }

	friend std::ostream& operator<<(std::ostream& os, const Point& point)
	{
		os << point.x << "," << point.y;
		return os;
	}

#endif

	friend bool operator==(const Point &a, const Point &b) 
	{
		return a.x == b.x && a.y == b.y;
	}

	friend bool operator!=(const Point& a, const Point& b)
	{
		return !(a == b);
	}

	Point operator * (const double scale) const
	{
		return Point(x * scale, y * scale);
	}

	inline Point<T> operator-() const
	{
		return Point<T>(-x,-y);
	}

	inline Point operator+(const Point &b) const
	{
		return Point(x+b.x, y+b.y);
	}

	inline Point operator-(const Point &b) const
	{
		return Point(x-b.x, y-b.y);
	}

};

//nb: using 'using' here (instead of typedef) as they can be used in templates
using Point64 = Point<int64_t>;
using PointD = Point<double>;

template <typename T>
using Path = std::vector<Point<T>>;
template <typename T>
using Paths = std::vector<Path<T>>;

using Path64 = Path<int64_t>;
using PathD = Path<double>;
using Paths64 = std::vector< Path64>;
using PathsD = std::vector< PathD>;

inline Path64 ScalePath(const Path64& path, double scale)
{
	Path64 result;
	result.reserve(path.size());
	for (const Point64 pt : path)
		result.push_back(pt * scale);
	return result;
}

inline Paths64 ScalePaths(const Paths64& paths, double scale)
{
	Paths64 result;
	result.reserve(paths.size());
	for (const Path64 path : paths)
		result.push_back(ScalePath(path, scale));
	return result;
}

inline PathD ScalePath(const PathD& path, double scale)
{
	PathD result;
	result.reserve(path.size());
	for (const PointD pt : path)
		result.push_back(pt * scale);
	return result;
}

inline PathsD ScalePaths(const PathsD& paths, double scale)
{
	PathsD result;
	result.reserve(paths.size());
	for (const PathD path : paths)
		result.push_back(ScalePath(path, scale));
	return result;
}

inline Path64 ScalePath64(const PathD& path, double scale)
{
	Path64 result;
	result.reserve(path.size());
	for (const PointD pt : path)
		result.push_back(Point64(pt * scale));
	return result;
}

inline Paths64 ScalePaths64(const PathsD& paths, double scale)
{
	Paths64 result;
	result.reserve(paths.size());
	for (const PathD path : paths)
		result.push_back(ScalePath64(path, scale));
	return result;
}

inline PathD ScalePathD(const Path64& path, double scale)
{
	PathD result;
	result.reserve(path.size());
	for (const Point64 pt : path)
		result.push_back(PointD(pt.x * scale, pt.y * scale));
	return result;
}

inline PathsD ScalePathsD(const Paths64& paths, double scale)
{
	PathsD result;
	result.reserve(paths.size());
	for (const Path64 path : paths)
		result.push_back(ScalePathD(path, scale));
	return result;
}

static PathD Path64ToPathD(const Path64& path)
{
	PathD result;
	std::transform(path.cbegin(), path.cend(), std::back_inserter(result),
		[](const Point64& pt) {return PointD(pt); });
	return result;
}

static PathsD Paths64ToPathsD(const Paths64& paths)
{
	PathsD result;
	std::transform(paths.cbegin(), paths.cend(), std::back_inserter(result),
		[](const Path64& path) {return Path64ToPathD(path); });
	return result;
}

static Path64 PathDToPath64(const PathD& path)
{
	Path64 result;
	result.reserve(path.size());
	std::transform(path.cbegin(), path.cend(), std::back_inserter(result),
		[] (const PointD& pt) { return Point64(pt); });
	return result;
}

static Paths64 PathsDToPaths64(const PathsD& paths)
{
	Paths64 result;
	std::transform(paths.cbegin(), paths.cend(), std::back_inserter(result),
		[](const PathD& path) { return PathDToPath64(path); });
	return result;
}

template<typename T>
inline double Sqr(T val)
{
	return static_cast<double>(val) * static_cast<double>(val);
}

inline bool NearEqual(const Point<double>& p1, 
	const Point<double>& p2, double max_dist_sqrd)
{
	return Sqr(p1.x - p2.x) + Sqr(p1.y - p2.y) < max_dist_sqrd;
}

static PathD StripNearEqual(const PathD& path, 
	double max_dist_sqrd, bool is_closed_path)
{
	if (path.size() == 0) return PathD();
	PathD result;
	result.reserve(path.size());
	PathD::const_iterator path_iter = path.cbegin();
	PointD first_pt = *path_iter++, last_pt = first_pt;
	result.push_back(first_pt);
	for (; path_iter != path.cend(); ++path_iter)
	{
		if (!NearEqual(*path_iter, last_pt, max_dist_sqrd))
		{
			last_pt = *path_iter;
			result.push_back(last_pt);
		}
	}
	if (!is_closed_path) return result;
	while (result.size() > 1 &&
		NearEqual(result.back(), first_pt, max_dist_sqrd)) result.pop_back();
	return result;
}

static PathsD StripNearEqual(const PathsD& paths, 
	double max_dist_sqrd, bool is_closed_path)
{
	PathsD result;
	result.reserve(paths.size());
	for (PathsD::const_iterator paths_citer = paths.cbegin();
		paths_citer != paths.cend(); ++paths_citer)
	{
		result.push_back(StripNearEqual(*paths_citer, max_dist_sqrd, is_closed_path));
	}
	return result;
}

template<typename T>
static Path<T> StripDuplicates(const Path<T>& path, bool is_closed_path)
{
	if (path.size() == 0) return Path<T>();
	Path<T> result;
	result.reserve(path.size());
	Path<T>::const_iterator path_iter = path.cbegin();
	Point<T> first_pt = *path_iter++, last_pt = first_pt;
	result.push_back(first_pt);
	for (; path_iter != path.cend(); ++path_iter)
	{
		if (*path_iter != last_pt)
		{
			last_pt = *path_iter;
			result.push_back(last_pt);
		}
	}
	if (!is_closed_path) return result;
	while (result.size() > 1 && result.back() == first_pt) result.pop_back();
	return result;
}

template<typename T>
static Paths<T> StripDuplicates(const Paths<T>& paths, bool is_closed_path)
{
	Paths<T> result;
	result.reserve(paths.size());
	for (Paths<T>::const_iterator paths_citer = paths.cbegin();
		paths_citer != paths.cend(); ++paths_citer)
	{
		result.push_back(StripDuplicates(*paths_citer, is_closed_path));
	}
	return result;
}

// Rect ------------------------------------------------------------------------

template <typename T>
struct Rect;

using Rect64 = Rect<int64_t>;
using RectD = Rect<double>;

template <typename T>
struct Rect {
	T left;
	T top;
	T right;
	T bottom;

	Rect() :
		left(0),
		top(0),
		right(0),
		bottom(0) {}

	Rect(T l, T t, T r, T b) :
		left(l),
		top(t),
		right(r),
		bottom(b) {}


	T Width() const { return right - left; }
	T Height() const { return bottom - top; }
	void Width(T width) { right = left + width; }
	void Height(T height) { bottom = top + height; }

	void Scale(double scale) { 
		left *= scale; 
		top *= scale;
		right *= scale;
		bottom *= scale;
	}

	bool IsEmpty() const { return bottom <= top || right <= left; };

	friend std::ostream &operator<<(std::ostream &os, const Rect<T> &rect) {
		os << "("
		   << rect.left << "," << rect.top << "," << rect.right << "," << rect.bottom
		   << ")";
		return os;
	}
};

// clipper2Exception ---------------------------------------------------------

class Clipper2Exception : public std::exception {
public:
	explicit Clipper2Exception(const char *description) :
		m_descr(description) {}
	virtual const char *what() const throw() { return m_descr.c_str(); }

private:
	std::string m_descr;
};

// Miscellaneous ------------------------------------------------------------


template <typename T>
inline double CrossProduct(const Point<T>& pt1, const Point<T>& pt2, const Point<T>& pt3) {
	return (static_cast<double>(pt2.x - pt1.x) * static_cast<double>(pt3.y - 
		pt2.y) - static_cast<double>(pt2.y - pt1.y) * static_cast<double>(pt3.x - pt2.x));
}

template <typename T>
inline double DotProduct(const Point<T>& pt1, const Point<T>& pt2, const Point<T>& pt3) {
	return (static_cast<double>(pt2.x - pt1.x) * static_cast<double>(pt3.x - pt2.x) + 
		static_cast<double>(pt2.y - pt1.y) * static_cast<double>(pt3.y - pt2.y));
}

template <typename T>
inline double DotProduct(const Point<T>& vec1, const Point<T>& vec2)
{
	return static_cast<double>(vec1.x * vec2.x) + static_cast<double>(vec1.y * vec2.y);
}

template <typename T>
inline double DistanceSqr(const Point<T> pt1, const Point<T> pt2) {
	return Sqr(pt1.x - pt2.x) + Sqr(pt1.y - pt2.y);
}

template <typename T>
inline double Distance(const Point<T> pt1, const Point<T> pt2)
{
	return std::sqrt(DistanceSqr(pt1.x - pt2.x) + Sqr(pt1.y - pt2.y));
}

template <typename T>
const double Length = Distance<T>;

template <typename T>
inline double DistanceFromLineSqrd(const Point<T> &pt, const Point<T> &ln1, const Point<T> &ln2)
{
	//perpendicular distance of point (x³,y³) = (Ax³ + By³ + C)/Sqrt(A² + B²)
	//see http://en.wikipedia.org/wiki/Perpendicular_distance
	double A = static_cast<double>(ln1.y - ln2.y);
	double B = static_cast<double>(ln2.x - ln1.x);
	double C = A * ln1.x + B * ln1.y;
	C = A * pt.x + B * pt.y - C;
	return (C * C) / (A * A + B * B);
}

template <typename T>
bool NearCollinear(const Point<T> &pt1, const Point<T> &pt2, const Point<T> &pt3, double sin_sqrd_min_angle_rads)
{
	double cp = std::abs(CrossProduct(pt1, pt2, pt3));
	return (cp * cp) / (DistanceSqr(pt1, pt2) * DistanceSqr(pt2, pt3)) < sin_sqrd_min_angle_rads;
}

template <typename T>
inline double Area(const Path<T>& path)
{
	if (path.size() == 0) return 0.0;
	double a = 0.0;
	Path<T>::const_iterator path_iter, path_iter_last = --path.cend();
	for (path_iter = path.cbegin(); path_iter != path.cend();
		path_iter_last = path_iter, ++path_iter)
	{
		a += static_cast<double>(path_iter_last->y - path_iter->y) *
			(path_iter_last->x + path_iter->x);
	}
#ifdef REVERSE_ORIENTATION
	return a * -0.5;
#else
	return a * 0.5;
#endif
}

template <typename T>
inline double Area(const Paths<T>& paths)
{
	double a = 0.0;
	for (Paths<T>::const_iterator paths_iter = paths.cbegin();
		paths_iter != paths.cend(); ++paths_iter)
	{
		a += Area<T>(*paths_iter);
	}
	return a;
}

template <typename T>
inline bool IsClockwise(const Path<T>& poly)
{
	return Area<T>(poly) >= 0;
}

template <typename T>
static Path<T> Ellipse(const Point<T>& center,
	double radiusX, double radiusY = 0, int steps = 0)
{
	if (radiusX <= 0) return Path64();
	if (radiusY <= 0) radiusY = radiusX;
	if (steps <= 2)
		steps = static_cast<int>(PI * sqrt((radiusX + radiusY) / 2));
#ifdef REVERSE_ORIENTATION
	double si = std::sin(2 * PI / steps);
#else
	double si = -std::sin(2 * PI / steps);
#endif
	double co = std::cos(2 * PI / steps);
	double dx = co, dy = si;
	Path<T> result;
	result.reserve(steps);
	result.push_back(Point<T>(center.x + radiusX, (double)center.y));
	for (int i = 1; i < steps; ++i)
	{
		result.push_back(Point<T>(center.x + radiusX * dx, center.y + radiusY * dy));
		double x = dx * co - dy * si;
		dy = dy * co + dx * si;
		dx = x;
	}
	return result;
}

template <typename T>
inline double PerpendicDistFromLineSqrd(const Point<T>& pt,
	const Point<T>& line1, const Point<T>& line2)
{
	double a = static_cast<double>(pt.x - line1.x);
	double b = static_cast<double>(pt.y - line1.y);
	double c = static_cast<double>(line2.x - line1.x);
	double d = static_cast<double>(line2.y - line1.y);
	if (c == 0 && d == 0) return 0;
	return Sqr(a * d - c * b) / (c * c + d * d);
}

template <typename T>
static void RDP(const Path<T> path, std::size_t begin,
	std::size_t end, double epsSqrd, std::vector<int>& flags)
{
	Path<T>::size_type idx = 0;
	double max_d = 0;
	while (end > begin && path[begin] == path[end]) flags[end--] = 0;
	for (Path<T>::size_type i = begin + 1; i < end; ++i)
	{
		//PerpendicDistFromLineSqrd - avoids expensive Sqrt()
		double d = PerpendicDistFromLineSqrd(path[i], path[begin], path[end]);
		if (d <= max_d) continue;
		max_d = d;
		idx = i;
	}
	if (max_d <= epsSqrd) return;
	flags[idx] = 1;
	if (idx > begin + 1) RDP(path, begin, idx, epsSqrd, flags);
	if (idx < end - 1) RDP(path, idx, end, epsSqrd, flags);
}

template <typename T>
static Path<T> RamerDouglasPeucker(const Path<T>& path, double epsilon)
{
	Path<T>::size_type len = path.size();
	if (len < 5) return Path<T>(path);
	std::vector<int> flags(len);
	flags[0] = 1;
	flags[len - 1] = 1;
	RDP(path, 0, len - 1, Sqr(epsilon), flags);
	Path<T> result;
	result.reserve(len);
	for (Path<T>::size_type i = 0; i < len; ++i)
		if (flags[i] == 1)
			result.push_back(path[i]);
	return result;
}

template <typename T>
static Paths64 RamerDouglasPeucker(const Paths<T>& paths, double epsilon)
{
	Paths<T> result;
	result.reserve(paths.size());
	for (Path<T> path : paths)
		result.push_back(RamerDouglasPeucker<T>(path, epsilon));
	return result;
}

}  //namespace

#endif  // CLIPPER_CORE_H
