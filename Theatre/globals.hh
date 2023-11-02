#ifndef GLOBALS_H
#define GLOBALS_H
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//This header class is used to save all...
//... settings that are used in many classes

const int NUMBER_OF_FIELDS = 5;

// Command prompt
const string PROMPT = "the> ";

// Error and other messages
const string EMPTY_FIELD = " Error: empty field in line ";
const string FILE_ERROR = " Error: input file cannot be opened";
const string WRONG_PARAMETERS = " Error: wrong number of parameters";
const string THEATRE_NOT_FOUND = " Error: unknown theatre";
const string PLAY_NOT_FOUND = " Error: unknown play";
const string PLAYER_NOT_FOUND = " Error: unknown player";
const string TOWN_NOT_FOUND = " Error: unknown town";
const string COMMAND_NOT_FOUND = " Error: unknown command";
const string NOT_AVAILABLE = " No plays available";

// Declare the split function as extern
extern std::vector<std::string> split(const std::string& str, char delim);

#endif
