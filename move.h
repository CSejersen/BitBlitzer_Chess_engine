#pragma once
#include <string>

class Move{
public:
    Move(std::string start, std::string target);
    std::string getStartSquare() const;
    std::string getTargetSquare() const;
private:
    std::string startSquare;
    std::string targetSquare;

};
