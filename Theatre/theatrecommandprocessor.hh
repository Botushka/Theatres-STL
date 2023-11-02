#ifndef THEATRE_COMMAND_PROCESSOR_H
#define THEATRE_COMMAND_PROCESSOR_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "theatredatareader.hh"
#include "globals.hh"
#include "theatredata.hh"
#include <unordered_map>

class TheatreCommandProcessor {
public:
    TheatreCommandProcessor(std::map<std::string, std::vector<TheatreData>>& dataMap);
    void processCommands(){
        aliasMap["Saituri"] = "Saituri/Miser";
        aliasMap["Miser"] = "Saituri/Miser";
    }
    void processTheatresCommand();
    void processPlaysCommand();
    void processTheatresOfPlayCommand(const std::string& play);
    void processPlayersInPlayCommand(const std::string& input);
    void processPlaysInTownCommand(const std::string& town);
    void processPlaysInTheatreCommand(const std::string& theatre);
    std::string sanitizeTheatreName(const std::string& theatreName);
    std::unordered_map<std::string, std::string> aliasMap;
private:
    std::map<std::string, std::vector<TheatreData>>& theatreDataMap;
    std::string translateAliasToPlayName(const std::string& alias);
};

#endif // THEATRE_COMMAND_PROCESSOR_H
