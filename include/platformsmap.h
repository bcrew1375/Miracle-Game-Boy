#ifndef PLATFORMSMAP_H
#define PLATFORMSMAP_H

#include <map>
#include <string>

// Map available platform strings to integers.
std::map<std::string, int> availablePlatforms {
    {"Game Boy Classic", 0x01},
    {"Super Game Boy", 0x02},
    {"Game Boy Color", 0x03}
};

#endif // PLATFORMSMAP_H
