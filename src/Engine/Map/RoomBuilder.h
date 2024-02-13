
//#include <string>
//#include <vector>
//#include "Engine/Map/Room.h"
//#include "absl/container/flat_hash_map.h"
//
//class MapBuilder {
//   public:
//    static std::array<std::array<Room, 10>, 10>* BuildMap(
//        const std::vector<Room>& rooms, int room_limit);
//    static absl::flat_hash_map<Entrances, std::vector<Room>*>
//    MapRoomsToEntrances(const std::vector<Room>& rooms);
//
//   private:
//    Entrance GetOppositeEntrance(Entrance entrance);
//    std::string m_TextureID;
//    int m_room_id;
//    int m_Height;
//    int m_SrcTileSize;
//    int m_DestTileSize;
//};
//
// 1. Map each room by # of entrances
// 2. Pick a random starting Room
// 3. for each entrance on room, pick a random room from map of the opposite direction unless already populated
// 4. continue until there is X amount of rooms
// return 2D array of rooms
//
// Check 2D array for consistency:
