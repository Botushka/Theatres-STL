#include "globals.hh"

// Define the split function here instead of in main.cpp
std::vector<std::string> split(const std::string& str, char delim)
{
    std::vector<std::string> result = {""};
    bool inside_quotation = false;
    for (char current_char : str)
    {
        if (current_char == '"')
        {
            inside_quotation = !inside_quotation;
        }
        else if (current_char == delim && !inside_quotation)
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    return result;
}
