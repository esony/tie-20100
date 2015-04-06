#include "datastructure.hh"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

Datastructure::Datastructure(){
    for (int i = 0; i < SIZE; i++){
        tietokanta[i] = new Tuote;
        tietokanta[i]->ID = "";
        tietokanta[i]->location = "";
        tietokanta[i]->amount = 0;
        tietokanta[i]->name = "";
        tietokanta[i]->next = NULL;
    }
}

Datastructure::~Datastructure(){

}

// Adds candies to the datastructure
void Datastructure::add(const string& ID, const string& location,
        unsigned int amount, const string& name){
    int index = hash(ID);

    //Jos laatikossa ei ole yhtaan alkiota, lisataan tuote sinne
    if (tietokanta[index]->ID == ""){

        tietokanta[index]->ID = ID;
        tietokanta[index]->location = location;
        tietokanta[index]->amount = amount;
        tietokanta[index]->name = name;
        tietokanta[index]->next = NULL;
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
                cerr << EI_VARASTOA << endl;
                cerr << "Saldo: " << ptr->amount << " Hylly: " << ptr->location << endl;

            //Jos varastossa on enemman kuin halutaan poistaa
            } else if (ptr->amount > amount){
                ptr->amount -= amount;
                cout << "Saldo: " << ptr->amount << " Hylly: " << ptr->location << endl;

            //Jos varasto tyhjenee, poistetaan tuote kokonaan
            } else if (ptr->amount == amount){

                //Jos tuote on ensimmainen ja viimeinen laatikossa
                if (ptr == tietokanta[index] && ptr->next == NULL){
                    cout << "Saldo: " << "0" << " Hylly: " << ptr->location << endl;
                    ptr->ID = "";
                    ptr->location = "";
                    ptr->amount = 0;
                    ptr->name = "";

                //Jos tuote on ensimmainen, muttei viimeinen laatikossa
                } else if (ptr == tietokanta[index]){
                    cout << "Saldo: " << "0" << " Hylly: " << ptr->location << endl;
                    tietokanta[index] = ptr->next;
                    delete ptr;

/*                //Jos tuote viimeisena laatikossa
                } else if (ptr->next = NULL){
                    P1->next = NULL;
                    delete ptr;
*/
                //Jos edelliset eivat pade, niin poistetaan tuote laatikosta
                } else {
                    cout << "Saldo: " << "0" << " Hylly: " << ptr->location << endl;
                    P1->next = ptr->next;
                    delete ptr;
                }
            }
            return;

        } else {
            //Jos ei ollut oikea, siirrytaan seuraavaan
            P1 = ptr;
            ptr = ptr->next;

        }
    }

    //Jos ei loydy varastosta, niin tulostellaan
    cerr << EI_SAATAVILLA << endl;
    return;
}

// Finds candy and prints its status
void Datastructure::find(const string& ID) const{
/*
    Tuote* pdr;

    for (int i = 0; i < SIZE; i++){
        pdr = tietokanta[i];

        while (pdr != NULL){
            cout << "Laatikossa " << i << " " << pdr->name << " " << pdr->amount << " " << pdr->location << endl;
            pdr = pdr->next;
        }
    }
*/

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
            if (ptr->ID != ""){
                ++n;
            }
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

        tietokanta[i] = new Tuote;
        tietokanta[i]->ID = "";
        tietokanta[i]->location = "";
        tietokanta[i]->amount = 0;
        tietokanta[i]->name = "";
        tietokanta[i]->next = NULL;
    }
}

// Saves candy data to file, called when exiting
void Datastructure::save_to_file(const string& filename) const{
    cout << filename << endl;
}

//Funktio generoi ID:n perusteella numeron valilta 0-3000
//Numero kertoo laatikon, johon tuote kuuluu
int Datastructure::hash(const string& ID) const{

    /*
    istringstream virta(ID);
    string tuoteryhma;
    int varastotunnus;
    int valmistaja;
    int tuote;



    //Luetaan virrasta tuoteryhma (josta myohemmin varastotunnus)
    virta >> tuoteryhma;
    //Valimerkki ':'
    virta.get();
    //Valmistajan tunnus
    virta >> valmistaja;
    //Toinen valimerkki ':'
    virta.get();
    //Tuotenumero
    virta >> tuote;

    //Varmistetaan etta luvallinen valimerkki
    if (virta.get() != ':'){
        cerr << "Vaara ID" << endl;
    }

    //Luetaan virrasta valmistajan koodi
    if ( !(virta >> valmistaja) ){
        cerr << "Vaara ID" << endl;
    }
    //Varmistetaan etta se on rajoissa
    if (valmistaja < 1000 || valmistaja > 9999){
        cerr << "Vaara ID" << endl;
    }
    //Varmistetaan etta luvallinen valimerkki
    if (virta.get() != ':'){
        cerr << "Vaara ID" << endl;
    }

    //Luetaan virrasta tuotteen koodi
    if ( !(virta >> tuote) ){
        cerr << "Vaara ID" << endl;
    }

    //Varmistetaan etta se on rajoissa
    if (tuote < 100 || tuote > 999){
        cerr << "Vaara ID" << endl;
        return false;
    }

    //Varmistetaan etta ID ei sisalla muuta roskaa
    if (virta.peek() != EOF){
        cerr << "Vaara ID" << endl;
        return false;
    }


    // Vaihdetaan varastokoodi numeroksi
    if (tuoteryhma == "LKS"){
        varastotunnus = 0;

    } else if (tuoteryhma == "GUM"){
        varastotunnus = 1;

    } else if (tuoteryhma == "SMK"){
        varastotunnus = 2;

    } else if (tuoteryhma == "MIX"){
        varastotunnus = 3;

    } else if (tuoteryhma == "SUK"){
        varastotunnus = 4;

    } else if (tuoteryhma == "TIK"){
        varastotunnus = 5;

    } else if (tuoteryhma == "VHT"){
        varastotunnus = 6;

    } else if (tuoteryhma == "WIN"){
        varastotunnus = 7;

    //Jos se on virheellinen
    } else {
        varastotunnus = 8;
    }

    //Tehtaillaan koko ID:sta yksi numero, mallia xn nnn mmm
    int paluuarvo = varastotunnus;

    // x * 10 000 + nnnn = xn nnn
    paluuarvo = paluuarvo * 10000 + valmistaja;

    // xn nnn * 1 000 + mmm = xn nnn mmm
    paluuarvo = paluuarvo * 1000 + tuote;
    cout << "Tuoteryhma: " << tuoteryhma << ", varastotunnus " << varastotunnus << endl;
    cout << "Valmistaja " << valmistaja << endl;
    cout << "Tuote " << tuote << endl;
    cout << "Menee laatikkoon" << paluuarvo % SIZE << endl;
    */

    //Palautetaan tehtaillun numeron ja taulukon koon jakojaannos
    unsigned int paluuarvo = 0;

    for (unsigned int i = 0; i < ID.length(); i++){
        char n = ID.at(i);
        paluuarvo = paluuarvo * 3 + (int)n;
//        cout << n << " lukuarvo " << int(n) << " paluuarvo " << paluuarvo << endl;
    }
//    cout << "Lukuarvo: " << paluuarvo << ", laatikko: " << paluuarvo % SIZE << endl;
    return paluuarvo % SIZE;
}
