#ifndef MAP_HH_
# define MAP_HH_

class Map
{
    public:
        Map();
        Map(const Map& map);
        ~Map();

        bool valid_position(position p) const;
    private:
}

#endif /* !MAP_HH_ */
