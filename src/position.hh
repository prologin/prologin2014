#ifndef POSITION_HH
# define POSITION_HH

# include "constant.hh"
# include <stdlib.h>
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
    size_t operator()(const position& p) const
    {
        return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
    }
};

#endif /* !POSITION_HH */
