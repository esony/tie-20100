// Definition of Datastructure class for UDS/Tiraka homework 1 -


#ifndef DATASTRUCTURE_HH
#define DATASTRUCTURE_HH

#include <string>

const std::string SEPARATOR = " ";

class Datastructure
{
    public:

    Datastructure();

    ~Datastructure();

    // Adds one value to the datastructure
    void add(std::string rank, unsigned int birthYear,
            unsigned int enlistingYear, std::string shirtColor,
            std::string name);

    // Prints sorted crew list
    void print();

    // Finds and prints youngest person
    void youngest();

    // Finds and prints oldest person
    void oldest();

    // Empties the datastructure
    void empty();

    // returns the size of the datastructure
    size_t size();

    // Copy constructor is forbidden
    Datastructure(const Datastructure&) = delete;
    // Assignment operator is forbidden
    Datastructure& operator=(const Datastructure&) = delete;

    private:

    // Add your own implementation here

};

#endif
