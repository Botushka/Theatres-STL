#include "theatredatareader.hh"
#include <iostream>
#include <fstream>
#include <map>
#include "theatredata.hh"
#include "globals.hh"
#include "theatrecommandprocessor.hh"
#include "main.cpp"


TheatreDataReader::TheatreDataReader(const std::string& filename) : filename(filename) {
}

bool anyFieldEmpty(const std::vector<std::string>& fields) {
    for (const std::string& field : fields) {
        if (field.empty()) {
            return true;
        }
    }
    return false;
}
bool TheatreDataReader::fileIsValid(const std::string& line) {
    std::vector<std::string> fields = split(line, ';');

    // Check if there are the correct number of fields and none of them are empty
    if (fields.size() != NUMBER_OF_FIELDS || anyFieldEmpty(fields)) {
        return false;
    }

    return true;
}

std::vector<TheatreData> TheatreDataReader::readAndValidateInputFile() {
    std::vector<TheatreData> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Throw a runtime_error with the error message
        throw std::runtime_error(FILE_ERROR);
    }

    std::string line;
    int lineCount = 0;
    while (getline(file, line)) {
        lineCount++;
        if (!fileIsValid(line)) {
            std::cerr << EMPTY_FIELD  << lineCount << std::endl;
            exit(EXIT_FAILURE);
        }
        std::vector<std::string> fields = split(line, ';');
        TheatreData entry;
        entry.town = fields[0];
        entry.theatre = fields[1];
        entry.play = fields[2];
        entry.actor = fields[3];
        entry.availableSeats = fields[4];

        data.push_back(entry);
    }

    // Now, populate the map with the data
    for (const TheatreData& entry : data) {
        theatreDataMap[entry.theatre].push_back(entry);
    }
    return data;
}

