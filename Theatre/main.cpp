#include <iostream>
#include <vector>
#include "theatredatareader.hh"
#include "globals.hh"
#include "theatrecommandprocessor.hh"
#include "theatredata.hh"
#include <limits>
std::map<std::string, std::vector<TheatreData>> theatreDataMap;

using namespace std;
//Project description:
// This program makes it easy to handle files consisting
// of finnish theatres, plays, players and locations
//Includes five commands which are:
//-theatres
//-plays
//-theatres_of_play
//-plays_in_theatre
//-plays_in_town
//-players_in_play

// Main function
int main() {
    std::string filename;

        // Ask the user for the input file name
        std::cout << "Input file: ";
        std::cin >> filename;


    // Create an instance of the TheatreDataReader
    TheatreDataReader reader(filename);

    // Read and validate the input file, and store the information
    std::vector<TheatreData> theatreData;
    try {
        theatreData = reader.readAndValidateInputFile();
    } catch (const std::runtime_error& e) {

        std::cout << FILE_ERROR << endl;
        return EXIT_FAILURE;
    }

    TheatreCommandProcessor commandProcessor(theatreDataMap);
    //User interface setup with while loop
       while (true) {
           std::string command;
           std::cout << "the> ";
           std::cin >> command;
    // implementing commands either with else if or with case statement
           if (command == "quit") {
               // Quit the program
               break;
           } else if (command == "theatres") {
               commandProcessor.processTheatresCommand();
           } else if (command == "plays") {
               commandProcessor.processPlaysCommand();
           } else if (command == "theatres_of_play") {
               std::string play;
               std::cin.ignore();
               std::getline(std::cin, play);
               commandProcessor.processTheatresOfPlayCommand(play);
           } else if (command == "players_in_play") {
               std::string playAndTheatre;
               std::cin.ignore();
               std::getline(std::cin, playAndTheatre);
               commandProcessor.processPlayersInPlayCommand(playAndTheatre);
           } else if (command == "plays_in_town") {
               std::string town;
               std::cin.ignore();
               std::getline(std::cin, town);
               commandProcessor.processPlaysInTownCommand(town);
           } else if (command == "plays_in_theatre") {
               std::string theatre;
               std::cin.ignore();
               std::getline(std::cin, theatre);
               commandProcessor.processPlaysInTheatreCommand(theatre);
           } else {
               std::cout << COMMAND_NOT_FOUND << std::endl;
               // Clear the input buffer to avoid processing residual data
               std::cin.clear(); // Clears any error flags
               // Ignore any characters in the input buffer until the end of the current line
               std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
           }
       }

       return EXIT_SUCCESS;
}


