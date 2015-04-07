#include "datastructure.hh"

#include <fstream>
#include <sstream>
#include <iostream>
#include <deque>

using namespace std;

Datastructure::Datastructure(): tietokanta(SIZE){
}

Datastructure::~Datastructure(){
}

// Adds candies to the datastructure
void Datastructure::add(const string& ID, const string& location,
        unsigned int amount, const string& name){
    int index = hash(ID);

    //Jos laatikossa ei ole yhtaan alkiota, lisataan tuote sinne
    if (tietokanta[index] == NULL){
        Tuote* uusi = new Tuote;
        uusi->ID = ID;
        uusi->location = location;
        uusi->amount = amount;
        uusi->name = name;
        uusi->next = NULL;
        tietokanta[index] = uusi;

        return;

    //Jos laatikossa on alkioita
    } else {

        //Kaydaan laatikko lapi ja etsitaan oikeaa tuotetta
        Tuote* ptr = tietokanta[index];
        while (ptr->next != NULL){

            //Jos tuote on oikea, lisataan tavaraa
            if (ptr->ID == ID){
                ptr->amount += amount;
                return;

            //Muuten jatketaan eteenpain viimeiseen alkioon asti
            } else {
                ptr = ptr->next;
            }
        }

        //Jos tuote on listalla viimeisena, lisataan tavaraa
        if (ptr->ID == ID){
            ptr->amount += amount;
            return;

        } else {
            //Jos ei loydy listalta, lisataan uusi tuote
            Tuote* uusi = new Tuote;
            uusi->ID = ID;
            uusi->location = location;
            uusi->amount = amount;
            uusi->name = name;
            uusi->next = NULL;

            ptr->next = uusi;
            return;
        }
    }
}

// Removes candies from the datastructure
void Datastructure::substract(const string& ID, unsigned int amount){

    //Etsitaan oikea laatikko ID:n perusteella
    int index = hash(ID);
    Tuote* ptr = tietokanta[index];
    Tuote* P1 = tietokanta[index];

    //Kaydaan laatikko lapi, etsitaan oikea tuote ja vahennetaan varastosta
    while(ptr != NULL){

        //Kun oikea tuote loytyy
        if (ptr->ID == ID){

            //Jos tuotetta varastossa on liian vahan
            if (ptr->amount < amount){
                cout << EI_VARASTOA << endl;
                cout << "Saldo: " << ptr->amount << " Hylly: " << ptr->location << endl;
                return;

            //Jos varastossa on enemman kuin halutaan poistaa
            } else if (ptr->amount > amount){
                ptr->amount -= amount;
                cout << "Saldo: " << ptr->amount << " Hylly: " << ptr->location << endl;
                return;

            //Jos varasto tyhjenee, poistetaan tuote kokonaan
            } else if (ptr->amount == amount){

                //Jos tuote on ensimmainen ja viimeinen laatikossa
                if (ptr == tietokanta[index] && ptr->next == NULL){
                    cout << "Saldo: " << "0" << " Hylly: " << ptr->location << endl;
                    tietokanta[index] = NULL;
                    delete ptr;
                    return;

                //Jos tuote on ensimmainen, muttei viimeinen laatikossa
                } else if (ptr == tietokanta[index]){
                    cout << "Saldo: " << "0" << " Hylly: " << ptr->location << endl;
                    tietokanta[index] = ptr->next;
                    delete ptr;
                    return;

                //Jos edelliset eivat pade, niin poistetaan tuote laatikosta
                } else {
                    cout << "Saldo: " << "0" << " Hylly: " << ptr->location << endl;
                    P1->next = ptr->next;
                    delete ptr;
                    return;
                }
            }

        } else {
            //Jos ei ollut oikea, siirrytaan seuraavaan
            P1 = ptr;
            ptr = ptr->next;

        }
    }

    //Jos ei loydy varastosta, niin tulostellaan
    cout << EI_SAATAVILLA << endl;
    return;
}

// Finds candy and prints its status
void Datastructure::find(const string& ID) const{

    //Etsitaan oikea laatikko ID:n perusteella
    int index = hash(ID);
    Tuote* ptr = tietokanta[index];

    //Kaydaan laatikko lapi, etsitaan oikea tuote ja tulostetaan tiedot
    while(ptr != NULL){
        if (ptr->ID == ID){
            cout << ptr->name << " " << ptr->amount << " " << ptr->location << endl;
            return;

        } else {
            ptr = ptr->next;
        }
    }

    //Jos ei loydy varastosta, niin tulostellaan
    cout << EI_SAATAVILLA << endl;
    return;
}

// Returns how many different types of candy the datastructure contains
size_t Datastructure::count() const{
    size_t n = 0;
    Tuote* ptr;
    for (int i = 0; i < SIZE; i++){
        ptr = tietokanta[i];
        while (ptr != NULL){
            ++n;
            ptr = ptr->next;
        }
    }
    return n;
}

// Empties the datastructure
void Datastructure::empty(){
    Tuote* ptr;
    Tuote* pdel;

    for (int i = 0; i < SIZE; i++){
        ptr = tietokanta[i];
        pdel = ptr;

        while (pdel != NULL){
            ptr = ptr->next;
            delete pdel;
            pdel = ptr;
        }

        tietokanta[i] = NULL;
    }
}

// Saves candy data to file, called when exiting
void Datastructure::save_to_file(const string& filename) const{
    ofstream tulostus(filename);
    Tuote* ptr = tietokanta[0];

    for (int i = 0; i < SIZE; i++){
        ptr = tietokanta[i];

        while (ptr != NULL){
            tulostus << ptr->ID << ';'
                     << ptr->amount << ';'
                     << ptr->location << ';'
                     << ptr->name << endl;
            ptr = ptr->next;
        }
    }
}

//Funktio generoi ID:n perusteella numeron valilta 0-3000
//Numero kertoo laatikon, johon tuote kuuluu
int Datastructure::hash(const string& ID) const{

    unsigned int paluuarvo = 9209;
    for (unsigned int i = 0; i < ID.length(); ++i){
        paluuarvo = (paluuarvo * ID[i] + ID[i]*ID[i]*ID[i] + ID[i]*ID[i] + ID[i]);
    }

    //Palautetaan tehtaillun numeron ja taulukon koon jakojaannos
    return (paluuarvo) % SIZE;
}
