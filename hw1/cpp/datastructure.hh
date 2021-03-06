// Definition of Datastructure class for UDS/Tiraka homework 1 -


#ifndef DATASTRUCTURE_HH
#define DATASTRUCTURE_HH

#include <string>
#include <vector>

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

    struct Henkilo {
        std::string rank;
        unsigned int birthYear;
        unsigned int enlistingYear;
        std::string shirtColor;
        std::string name;
    };

    //Tietokanta, vanhin ja nuorin
    std::vector<Henkilo> tietokanta;
    Henkilo nuorin;
    Henkilo vanhin;
    bool on_sortattu = false;

    //Funktiot
    void quicksort();
    void set_nuorin_vanhin(Henkilo uusi);
    int median(int vasen, int oikea);
    void sort(int vasen, int oikea);

    void siistiminen();
    void ika_insertion(int vasen, int oikea);
    void qsort_ika(int vasen, int oikea);

};

#endif
