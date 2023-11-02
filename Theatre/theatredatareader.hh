#ifndef THEATRE_DATA_READER_H
#define THEATRE_DATA_READER_H

#include <map>
#include <vector>
#include <string>
#include "theatredata.hh"

extern std::map<std::string, std::vector<TheatreData>> theatreDataMap;

class TheatreDataReader {
public:
    TheatreDataReader(const std::string& filename);
    std::vector<TheatreData> readAndValidateInputFile();

private:
    const std::string filename;
    bool fileIsValid(const std::string& line);
};

#endif
