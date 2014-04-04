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

inline position operator+(const position& a, const position& b)
{
    return { a.x + b.x, a.y + b.y };
}

inline position operator-(const position& a, const position& b)
{
    return { a.x - b.x, a.y - b.y };
}

inline position operator-(const position& a)
{
    return { -a.x, -a.y };
}

inline int distance(const position& a, const position& b)
{
    return abs(a.x - b.y) + abs(a.y - b.y);
}

struct HashPosition
{
    std::size_t operator()(const position& p) const
    {
        std::size_t s = std::hash<int>()(p.y) + 0x9e3779b9;
        return s ^ std::hash<int>()(p.x) + 0x9e3779b9 + (s<<6) + (s>>2);
    }
};

#endif /* !POSITION_HH */
