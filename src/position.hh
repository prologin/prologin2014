#ifndef POSITION_HH
# define POSITION_HH

# include "constant.hh"
# include <cstdlib>
# include <functional>

inline bool operator==(const position& a, const position& b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const position& a, const position& b)
{
    return !(a == b);
}

inline bool operator<(const position& a, const position& b)
{
    return a.x < b.x && a.y < b.y;
}

inline position operator-(position a)
{
    return { -a.x, -a.y };
}

inline position &operator+=(position &lhs, position rhs)
{
    lhs.y += rhs.y;
    lhs.x += rhs.x;
    return lhs;
}

inline position &operator-=(position &lhs, position rhs)
{
    return lhs += -rhs;
}

inline position operator+(position lhs, position rhs)
{
    return lhs += rhs;
}

inline position operator-(position lhs, position rhs)
{
    return lhs -= rhs;
}

inline int distance(const position& a, const position& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

struct HashPosition
{
    std::size_t operator()(const position& p) const
    {
        std::size_t s = std::hash<int>()(p.y) + 0x9e3779b9;
        return s ^ (std::hash<int>()(p.x) + 0x9e3779b9 + (s << 6) + (s >> 2));
    }
};

#endif /* !POSITION_HH */
