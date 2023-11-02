#ifndef THEATREDATA_H
#define THEATREDATA_H

#include <map>
#include <vector>
#include <string>
//Store Theatre data here
struct TheatreData {
    std::string town;
    std::string theatre;
    std::string play;
    std::string actor;
    std::string availableSeats;
};

// Declare the map as an extern variable
extern std::map<std::string, std::vector<TheatreData>> theatreDataMap;

#endif
