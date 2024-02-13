//#include "RoomBuilder.h"
//#include "SDL2/SDL_log.h"
//
//absl::flat_hash_map<Entrances, std::vector<Room>*>
//MapBuilder::MapRoomsToEntrances(const std::vector<Room>& rooms) {
//    absl::flat_hash_map<Entrances, std::vector<Room>*> entrances_to_rooms;
//    for (const Room& room : rooms) {
//        auto result = entrances_to_rooms.find(room.m_entrances);
//        if (result != entrances_to_rooms.end()) {
//            std::vector<Room>* cur_rooms = result->second;
//            cur_rooms->push_back(room);
//            continue;
//        }
//        std::vector<Room> new_room({room});
//        entrances_to_rooms.emplace(room.m_entrances, &new_room);
//    }
//
//    return entrances_to_rooms;
//}
//
//std::array<std::array<Room, 10>, 10>* MapBuilder::BuildMap(
//    const std::vector<Room>&  /*rooms*/, int  /*room_limit*/) {
//    std::array<std::array<Room, 10>, 10>* res_map;
//    return res_map;
//}
