#pragma once

#include <string>
#include <vector>

class Room {
public:
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
