
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

        return;

    // Tarkistetaan, onko uusi alkio nuorin
    // tai vanhin ja pannaan muistiin, jos on

    } else if (uusi.enlistingYear == nuorin.enlistingYear){
        if (uusi.birthYear > nuorin.birthYear){
            nuorin = uusi;
        }

    } else if (uusi.enlistingYear > nuorin.enlistingYear) {
        nuorin = uusi;

    }

    if (uusi.enlistingYear == vanhin.enlistingYear){
        if (uusi.birthYear < vanhin.birthYear) {
            vanhin = uusi;
        }

    } else if (uusi.enlistingYear < vanhin.enlistingYear){
        vanhin = uusi;

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

    quicksort();

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

// Listan jarjestelyn aloittava ja viimeisteleva funktio
// Lista quicksortataan ensin jarjestykseen ja
// siivotaan sitten samana vuonna liittyneet
void Datastructure::quicksort(){
    if (onko_sortattu()){
        siistiminen();
        return;

    } else {

        int vasen = 0;
        int oikea = tietokanta.size() - 1;

        sort(vasen, oikea);
        siistiminen();
        return;
    }
}

// Varsinaisen sorttauksen tekeva funktio

void Datastructure::sort(int vasen, int oikea){
    int i = vasen;
    int j = oikea;
    Henkilo n;

    // Etsitaan mediaanin paikka ja suuruus
    int x = median(vasen, oikea);
    unsigned int mediaani = tietokanta[x].enlistingYear;

    // Siirretaan suuremmat mediaanin oikealle ja pienemmat
    // vasemmalle puolelle

    while (i < j){
        while (tietokanta[i].enlistingYear < mediaani ) {
            i++;
        }
        while (tietokanta[j].enlistingYear > mediaani) {
            j--;
        }
       if (i <= j){
            n = tietokanta[i];
            tietokanta[i] = tietokanta[j];
            tietokanta[j] = n;
            i++;
            j--;
       }
    }
    //Jarjestetaan alkiot mediaanin vasemmalla puolella
    if (vasen < j){
    sort(vasen, j);
    }
    //Jarjestetaan alkiot mediaanin oikealla puolella
    if (i < oikea) {
        sort(i, oikea);
    }
    return;

    return;
}

// Funktio siistii quicksortatun listan samana vuonna liittyneet
// henkilot insertionilla
void Datastructure::siistiminen(){
    Henkilo s;
    Henkilo* tutkittava1 = nullptr;
    Henkilo* tutkittava2 = nullptr;
    vector<Henkilo>::size_type n = 0;
    vector<Henkilo>::size_type m = 0;

    while (n + 1 < tietokanta.size()){

        tutkittava1 = &tietokanta[n];
        tutkittava2 = &tietokanta[n+1];

        if (tutkittava1->enlistingYear == tutkittava2->enlistingYear){
           m = n + 1;

           //Etsitaan kaikki samana vuonna liittyneet
           //Ensimmainen paikalla n, viimeinen m
           while (m + 1 < tietokanta.size() &&
                  tutkittava1->enlistingYear == tutkittava2->enlistingYear){
               tutkittava1 = &tietokanta[m];
               tutkittava2 = &tietokanta[m+1];
               m++;
           }
           vector<Henkilo>::size_type i = n + 1;
           vector<Henkilo>::size_type j = i;

           //Insertion sorttaus listan alkioille
           while (i < m){
               j = i;
               s = tietokanta[i];
               while (j >= n + 1 && tietokanta[j - 1].birthYear > s.birthYear){
                   tietokanta[j] = tietokanta[j - 1];
                   j--;
               }
               tietokanta[j] = s;
               i++;
            }

        n = m;
        }
        n++;
    }
    return;
}

int Datastructure::median(int vasen, int oikea){

    int keskimmainen = (vasen-oikea)/2 + oikea;
    unsigned int a = tietokanta[vasen].enlistingYear;
    unsigned int b = tietokanta[keskimmainen].enlistingYear;
    unsigned int c = tietokanta[oikea].enlistingYear;

    // Mediaani a
        if ( (b <= a && a <= c) || (c <= a && a <= b)){
            return vasen;
        }

    // Mediaani b
        else if ( (a <= b && b <= c) || (c <= b && b <= a)){
            return keskimmainen;
        }

    // Mediaani c
        else {
            return oikea;
        }
}

// Funktio tarkastaa onko lista
// valmiiksi palvelusaikajarjestyksessa

bool Datastructure::onko_sortattu(){

    Henkilo* tutkittava1 = nullptr;
    Henkilo* tutkittava2 = nullptr;
    vector<Henkilo>::size_type n = 0;

    while (n + 1 < tietokanta.size()){

        tutkittava1 = &tietokanta.at(n);
        tutkittava2 = &tietokanta.at(n+1);

        if (tutkittava1->enlistingYear > tutkittava2->enlistingYear){
            return 0;
        }
        n++;
    }
    return 1;
}

// Finds and prints youngest person

void Datastructure::youngest(){
    if (nuorin.name == ""){
        return;
    } else {
        cout << nuorin.rank << " " << nuorin.name << ", " << nuorin.shirtColor << endl;
    }
    return;
}

// Finds and prints oldest person
void Datastructure::oldest(){
    if (vanhin.name == ""){
        return;
    } else {
      cout << vanhin.rank << " " << vanhin.name << ", " << vanhin.shirtColor << endl;
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



