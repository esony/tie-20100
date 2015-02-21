
#include "datastructure.hh"

using namespace std;


Datastructure::Datastructure() {

}

Datastructure::~Datastructure() {

}

//Tarkastetaan uusi alkio, onko se nuorin tai vanhin
void Datastructure::set_nuorin_vanhin(Henkilo uusi){

    //Jos tietokanta tyhja, on lisatty alkio seka nuorin etta vanhin
    if tietokanta.empty(){
        nuorin = &uusi;
        vanhin = &uusi;
        return;

    // Tarkistetaan, onko uusi alkio nuorin
    // tai vanhin ja pannaan muistiin, jos on

    } else if (uusi.enlistingYear = nuorin->enlistingYear){
        if (uusi.birthYear > nuorin->birthYear){
            nuorin = &uusi;
        }

    } else if (uusi.enlistingYear > nuorin->enlistingYear) {
        nuorin = &uusi;

    } else if (uusi.enlistingYear = vanhin){
        if (uusi.birthYear < vanhin->birthYear) {
            vanhin = &uusi;
        }

    } else if (uusi.enlistingYear < vanhin->enlistingYear){
        vanhin = &uusi;
    }
}

void Datastructure::add(std::string rank, unsigned int birthYear,
        unsigned int enlistingYear, std::string shirtColor,
        std::string name){

    //Luodaan uusi alkio ja lisataan syotetyt tiedot
    Henkilo uusi;

    uusi.rank = rank;
    uusi.birthYear = birthYear;
    uusi.enlistingYear = enlistingYear;
    uusi. shirtColor = shirtColor;
    uusi.name = name;

    set_nuorin_vanhin(uusi);
    tietokanta.push_back(uusi);
    return;

}

// Prints sorted crew list
void Datastructure::print(){

}

// Finds and prints youngest person
//Arvo nimi, Born: vuosi, Joined: vuosi, paidanväri
//Esim:
//Captain James T. Kirk, Born: 2233, Joined: 2250, Gold

void Datastructure::youngest(){
    if (nuorin == nullptr){
        return;
    } else {
      cout << nuorin->name << ", Born: " << nuorin->birthYear
           << ", Joined: " << nuorin->enlistingYear
           << ", " << nuorin->shirtColor << endl;
    }
}

// Finds and prints oldest person
void Datastructure::oldest(){
    if (vanhin == nullptr){
        return;
    } else {
      cout << vanhin->name << ", Born: " << vanhin->birthYear
           << ", Joined: " << vanhin->enlistingYear
           << ", " << vanhin->shirtColor << endl;
    }
}

// Empties the datastructure
void Datastructure::empty(){

}

// returns the size of the datastructure
size_t Datastructure::size(){

}



