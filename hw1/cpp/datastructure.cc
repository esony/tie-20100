/* .prosta
    main.cc \
    datastructure.cc
    */

#include <iostream>
#include "datastructure.hh"

using namespace std;

Datastructure::Datastructure(): tietokanta(), nuorin(), vanhin (){
}

Datastructure::~Datastructure() {

}

//Tarkastetaan uusi alkio, onko se nuorin tai vanhin
void Datastructure::set_nuorin_vanhin(Henkilo uusi){

    //Jos tietokanta tyhja, on lisatty alkio seka nuorin etta vanhin
    if (tietokanta.empty()){
        nuorin = uusi;
        vanhin = uusi;
        cout << "Tallennettu vanhimmaksi 1: " << nuorin.name << endl;

        return;

    // Tarkistetaan, onko uusi alkio nuorin
    // tai vanhin ja pannaan muistiin, jos on

    } else if (uusi.enlistingYear == nuorin.enlistingYear){
        if (uusi.birthYear > nuorin.birthYear){
            nuorin = uusi;
            cout << "Tallennettu nuorimmaksi 1: " << nuorin.birthYear << endl;
        }

    } else if (uusi.enlistingYear > nuorin.enlistingYear) {
        nuorin = uusi;
        cout << "Tallennettu nuorimmaksi 2" << endl;

    } else if (uusi.enlistingYear == vanhin.enlistingYear){
        if (uusi.birthYear < vanhin.birthYear) {
            vanhin = uusi;
            cout << "Tallennettu vanhimmaksi 1" << endl;
        }

    } else if (uusi.enlistingYear < vanhin.enlistingYear){
        vanhin = uusi;
        cout << "Tallennettu vanhimmaksi 2" << endl;

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
    uusi.shirtColor = shirtColor;
    uusi.name = name;

    set_nuorin_vanhin(uusi);
    tietokanta.push_back(uusi);
    return;

}

//Arvo nimi, Born: vuosi, Joined: vuosi, paidanväri
//Esim:
//Captain James T. Kirk, Born: 2233, Joined: 2250, Gold

// Prints sorted crew list
void Datastructure::print(){

    Henkilo* tulostettava = nullptr;
    vector<Henkilo>::size_type n = 0;

    while (n < tietokanta.size()){
        tulostettava = &tietokanta.at(n);

        cout << tulostettava->rank
             << " " << tulostettava->name
             << ", Born: " << tulostettava->birthYear
             << ", Joined: " << tulostettava->enlistingYear
             << ", " << tulostettava->shirtColor << endl;
        ++n;
    }

}

// Finds and prints youngest person

void Datastructure::youngest(){
    if (nuorin.name == ""){
        cout << "Nuorinta ei oo" << endl;
        return;
    } else {
      cout << "Nuorin tulloo" << endl;
        cout << nuorin.name << ", " << nuorin.shirtColor << endl;
    }
    return;
}

// Finds and prints oldest person
void Datastructure::oldest(){
    if (vanhin.name == ""){
        return;
    } else {
      cout << vanhin.name << ", " << vanhin.shirtColor << endl;
    }
    return;
}

// Empties the datastructure
void Datastructure::empty(){
    nuorin.name = "";
    vanhin.name = "";
    tietokanta.clear();
    return;
}

// returns the size of the datastructure
size_t Datastructure::size(){
    return tietokanta.size();
}



