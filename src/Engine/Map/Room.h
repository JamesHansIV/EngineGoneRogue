
#include <bitset>
#include <string>
#include <vector>

enum Entrance {
    UP = 1 << 0,
    RIGHT = 1 << 1,
    DOWN = 1 << 2,
    LEFT = 1 << 3,
};

class Entrances {
   public:
    Entrances(std::initializer_list<Entrance> entrances) {
        for (Entrance entrance : entrances) {
            m_entrances.set(static_cast<unsigned>(entrance));
        }
    }

    template <typename H>
    friend H AbslHashValue(H h, const Entrances& e) {
        return H::combine(std::move(h), e.m_entrances);
    }

    friend bool operator==(const Entrances& lhs, const Entrances& rhs) {
        // return true if the objects are equal, false otherwise
        return lhs.m_entrances == rhs.m_entrances;
    }

    bool test(Entrance e) { return m_entrances.test(static_cast<unsigned>(e)); }

    std::bitset<4> m_entrances;  // see below
   private:
};

class Room {
   public:
    Entrances m_entrances;

   private:
    std::string m_TextureID;
    int m_room_id;
    int m_Height;
    int m_SrcTileSize;
    int m_DestTileSize;
};

// # start Rooms, # normal rooms, # end rooms -> Graph of Rooms with a max of 4
// edges. Start and endpoint Only need # of rooms as each room will be
// associated with the index
//
// Room:
// roomId: identifier of room
// roomType: type of room start | normal | end | shop | reward
// adjacent_rooms
// roomLayout: layout of room
// dynamicStore: store rest of game objects
//
// Algorithm:
// 1. Randomly pick a starting room from # of start rooms as num
// 2. Create Room class based on roomLayout(num)
// 3. For each direction (up, down, left, right) there is a percentage that
// another room will be generated
// 4. Repeat until there are X rooms
// 5. Check floorplan for consistency
// 6. Tag rooms as certain types based on conditions
//
// More of the details of the algorithmthat are not explained can be based off
// of this article:
// https://www.boristhebrave.com/2020/09/12/dungeon-generation-in-binding-of-isaac/
//
// Notes:
//  Could have constraints for each floor and num of door info and use that in
//  algorithm Binding of isaac generation:
//  https://bindingofisaacrebirth.fandom.com/wiki/Level_Generation
