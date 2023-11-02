#include "theatrecommandprocessor.hh"
#include "globals.hh"
#include <iostream>
#include <fstream>
using namespace std;
#include "theatredata.hh"
#include <unordered_set>
#include <algorithm>
#include <set>
#include <sstream>
#include <cctype>
#include <cstring>
#include <limits>
//Class to handle commands

TheatreCommandProcessor::TheatreCommandProcessor(std::map<std::string, std::vector<TheatreData>>& dataMap)
    : theatreDataMap(dataMap) {
}
//HELP METHODS
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
std::string toLower(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

bool compareIgnoreCase(const std::string& str1, const std::string& str2) {
    return strcasecmp(str1.c_str(), str2.c_str()) == 0;
}
bool iequals(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    return std::equal(a.begin(), a.end(), b.begin(),
                      [](char a, char b) {
                          return tolower(a) == tolower(b);
                      });
}
std::string trimString(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, (last - first + 1));
}


std::vector<std::string> splitPlayNames(const std::string& playData, const std::string& delimiter = "***") {
    std::vector<std::string> playNames;
    size_t startPos = 0;
    size_t endPos = 0;
    std::string token;

    while ((endPos = playData.find(delimiter, startPos)) != std::string::npos) {
        token = playData.substr(startPos, endPos - startPos);
        trim(token); // Remove whitespace if necessary
        playNames.push_back(token);
        startPos = endPos + delimiter.length();
    }

    // Add the last token
    token = playData.substr(startPos, std::string::npos);
    trim(token); // Remove whitespace if necessary
    playNames.push_back(token);

    return playNames;
}
void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
//COMMANDS

void TheatreCommandProcessor::processTheatresCommand() {

    std::unordered_set<std::string> uniqueTheatres;


    // Iterate over all theatre data and add unique theatres to the set
    for (const auto& theatreData : theatreDataMap) {
        for (const TheatreData& data : theatreData.second) {
            uniqueTheatres.insert(data.theatre);
        }
    }

    // Move unique theatres to a vector and sort it
    std::vector<std::string> sortedTheatres(uniqueTheatres.begin(), uniqueTheatres.end());
    std::sort(sortedTheatres.begin(), sortedTheatres.end());

    // Print the unique theatres in alphabetical order
    for (const std::string& theatre : sortedTheatres) {
        std::cout << theatre << std::endl;
    }
}

void TheatreCommandProcessor::processPlaysCommand() {
    // Read the rest of the line after the command
    std::string remainder;
    std::getline(std::cin, remainder);

    // Check if there's anything other than whitespace in the remainder
    if (!std::all_of(remainder.begin(), remainder.end(), [](unsigned char c){ return std::isspace(c); })) {
        std::cerr << WRONG_PARAMETERS << endl;;
        return;
    }

    std::unordered_set<std::string> uniquePlays;

    // Iterate over all theatre data and add unique plays to the set
    for (const auto& theatreEntry : theatreDataMap) {
            for (const auto& playData : theatreEntry.second) {
                std::string playName = playData.play;
                replaceAll(playName, "/", " *** "); // Correctly call your replaceAll function
                uniquePlays.insert(playName);
            }
        }

    // Convert to vector and sort alphabetically
    std::vector<std::string> sortedPlays(uniquePlays.begin(), uniquePlays.end());
    std::sort(sortedPlays.begin(), sortedPlays.end());

    // Print the unique plays in alphabetical order
    for (const std::string& play : sortedPlays) {
        std::cout << play << std::endl;
    }
}

void TheatreCommandProcessor::processTheatresOfPlayCommand(const std::string& playInput) {
    // Start by determining if the input is quoted
    std::string sanitizedInput;
    if (playInput.front() == '"' && playInput.back() == '"') {
        // Remove the quotes
        sanitizedInput = playInput.substr(1, playInput.length() - 2);
    } else {
        sanitizedInput = playInput;
    }
    // Sanitize the input play name
    std::string sanitizedPlay = sanitizeTheatreName(sanitizedInput);
    std::transform(sanitizedPlay.begin(), sanitizedPlay.end(), sanitizedPlay.begin(), ::tolower);

    std::unordered_set<std::string> uniqueTheatres;

    // Iterate over all theatre data to find matching theatres
    for (const auto& data : theatreDataMap) {
        for (const TheatreData& theatreData : data.second) {
            std::vector<std::string> playNames = split(theatreData.play, '/');
            for (const std::string& playName : playNames) {
                // Convert play name to lower case for case-insensitive comparison
                std::string lowerCasePlayName = playName;
                std::transform(lowerCasePlayName.begin(), lowerCasePlayName.end(), lowerCasePlayName.begin(), ::tolower);

                if (iequals(lowerCasePlayName, sanitizedPlay)) {
                    uniqueTheatres.insert(data.first); // Insert the theatre name into the set
                    break; // Found a match, no need to check other aliases
                }
            }
        }
    }

    // If no matching theatres were found, print "Error: unknown play"
    if (uniqueTheatres.empty()) {
        std::cout << PLAY_NOT_FOUND<< endl;
    } else {
        // Sort and print the unique theatres in alphabetical order
        std::vector<std::string> sortedTheatres(uniqueTheatres.begin(), uniqueTheatres.end());
        std::sort(sortedTheatres.begin(), sortedTheatres.end());

        for (const std::string& theatre : sortedTheatres) {
            std::cout << theatre << std::endl;
        }
    }
}
std::string TheatreCommandProcessor::translateAliasToPlayName(const std::string& alias) {
    // Convert alias to lowercase for case-insensitive comparison
    std::string lowerCaseAlias = alias;
    std::transform(lowerCaseAlias.begin(), lowerCaseAlias.end(), lowerCaseAlias.begin(), ::tolower);

    // Check if the alias exists in the alias map
    auto aliasIter = aliasMap.find(lowerCaseAlias);
    if (aliasIter != aliasMap.end()) {
        return aliasIter->second; // Return the actual play name
    }

    // If no alias is found, return the input alias as it might be the actual play name
    return alias;
}

std::string TheatreCommandProcessor::sanitizeTheatreName(const std::string& theatreName) {
    std::string sanitized = theatreName;

    // Convert to lowercase for case-insensitive comparison
    std::transform(sanitized.begin(), sanitized.end(), sanitized.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    // Trim from the left (beginning)
    sanitized.erase(sanitized.begin(), std::find_if(sanitized.begin(), sanitized.end(), [](unsigned char c){
        return !std::isspace(c);
    }));

    // Trim from the right (end)
    sanitized.erase(std::find_if(sanitized.rbegin(), sanitized.rend(), [](unsigned char c){
        return !std::isspace(c);
    }).base(), sanitized.end());

    return sanitized;
}
void TheatreCommandProcessor::processPlaysInTheatreCommand(const std::string& input) {
    std::istringstream iss(input);
    std::string theatre, sanitizedTheatre;

    // Check if the input starts with a quote
    if (iss.peek() == '"') {
        // Extract the enclosed string within the quotes
        std::getline(iss.ignore(), theatre, '"');
    } else {
        // Otherwise, read the input as is
        iss >> theatre;
    }

    // Sanitize the theatre name
    sanitizedTheatre = sanitizeTheatreName(theatre);

    std::unordered_set<std::string> uniquePlays;

    // Iterate over all theatre data to find matching plays
    for (const auto& entry : theatreDataMap) {
        // Sanitize and trim the theatre name from the data map for comparison
        std::string trimmedDataTheatre = trim(sanitizeTheatreName(entry.first));
        if (iequals(trimmedDataTheatre, sanitizedTheatre)) {
            for (const TheatreData& data : entry.second) {
                uniquePlays.insert(data.play);
            }
        }
    }

    // Sort and display the plays if found
    if (!uniquePlays.empty()) {
        std::vector<std::string> sortedPlays(uniquePlays.begin(), uniquePlays.end());
        std::sort(sortedPlays.begin(), sortedPlays.end());
        for (const std::string& play : sortedPlays) {
            std::cout << play << std::endl;
        }
    } else {
        std::cout << PLAY_NOT_FOUND << std::endl;
    }
}
void TheatreCommandProcessor::processPlaysInTownCommand(const std::string& townInput) {
    // Sanitize the input town name for comparison
    std::string sanitizedTown = townInput;
    std::transform(sanitizedTown.begin(), sanitizedTown.end(), sanitizedTown.begin(), ::tolower);

    std::map<std::string, std::map<std::string, int>> playsByTheatre;
    bool hasPlays = false;

    // Iterate over the theatre data to find matching plays in the specified town
    for (const auto& theatreDataPair : theatreDataMap) {
        const std::vector<TheatreData>& theatreDataList = theatreDataPair.second;

        for (const TheatreData& theatreData : theatreDataList) {
            // Sanitize the town name from the data for comparison
            std::string dataTown = theatreData.town;
            std::transform(dataTown.begin(), dataTown.end(), dataTown.begin(), ::tolower);

            if (dataTown == sanitizedTown && theatreData.availableSeats != "none" && std::stoi(theatreData.availableSeats) > 0) {
                hasPlays = true;
                std::string playName = theatreData.play;
                replaceAll(playName, "/", " --- "); // Replace "/" with " --- "
                playsByTheatre[theatreData.theatre][playName] = std::stoi(theatreData.availableSeats);
            }
        }
    }

    if (hasPlays) {
        for (const auto& theatrePlaysPair : playsByTheatre) {
            for (const auto& playSeatsPair : theatrePlaysPair.second) {
                std::cout << theatrePlaysPair.first << " : " << playSeatsPair.first << " : " << playSeatsPair.second << std::endl;
            }
        }
    } else {
        std::cout << TOWN_NOT_FOUND << std::endl;
    }
}
void TheatreCommandProcessor::processPlayersInPlayCommand(const std::string& input) {
    std::istringstream iss(input);
    std::string play, theater;

    // Extract the play name
    iss >> std::ws; // skip leading whitespaces
    if (iss.peek() == '"') {
        iss.ignore(); // ignore the quote
        std::getline(iss, play, '"'); // extract play name
    } else {
        iss >> play;
    }

    // Normalize play name using alias map if necessary
    play = translateAliasToPlayName(play);

    bool playFound = false, theaterFound = false;
    std::map<std::string, std::set<std::string>> theaterActorsMap; // Use a set to avoid duplicate actor names

    // Extract the theater name if provided
    if (iss >> std::ws && iss.peek() != EOF) { // Check if there's more input
        if (iss.peek() == '"') {
            iss.ignore(); // ignore the quote
            std::getline(iss, theater, '"'); // extract theater name
        } else {
            iss >> theater;
        }
        theater = sanitizeTheatreName(theater);
        theaterFound = true; // Assuming we found a theatre name, we'll verify it exists later
    }

    // Find the play in the theatreDataMap and print actors
    for (const auto& theatreEntry : theatreDataMap) {
        if (theaterFound && sanitizeTheatreName(theatreEntry.first) == theater) {
            theaterFound = true; // Verify that the theatre actually exists in our map
        }
        for (const TheatreData& data : theatreEntry.second) {
            if (data.play.find(play) != std::string::npos) {
                playFound = true;
                if (theater.empty() || sanitizeTheatreName(data.theatre) == theater) {
                    theaterActorsMap[sanitizeTheatreName(data.theatre)].insert(data.actor);
                }
            }
        }
    }

    // Check for errors first
    if (!playFound) {
        std::cerr << PLAY_NOT_FOUND << std::endl;
        return;
    } else if (theaterFound && theaterActorsMap.empty()) {
        std::cerr << THEATRE_NOT_FOUND << std::endl;
        return;
    }

    // If theatres found for the given play, print actors sorted by theatre and actor name
    for (const auto& theaterActors : theaterActorsMap) {
        for (const std::string& actor : theaterActors.second) {
            std::cout << theaterActors.first << " : " << actor << std::endl;
        }
    }
}








