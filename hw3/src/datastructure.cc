//Bussiyhteyshakusoftan tietokanta- ja algoritmitoteutus

#include "datastructure.hh"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

Datastructure::Datastructure(){
}

Datastructure::~Datastructure(){
}

// N-komento. Tulostaa reitin pysäkkeinen, joka on ensimmäisenä perillä
// stop_id2:ssa.
void Datastructure::routeSearch(const string& time, const string& stop_id1,
                const string& stop_id2 ){

    //Jos lahto tai paatepysakkia ei ole, cout virhe
    if (Pysakki.find(stop_id1) == Pysakki.end() ||
        Pysakki.find(stop_id2) == Pysakki.end()){

        cout << VIRHE << endl;
        return;

    //Jos lahto ja paate on sama
    } else if (stop_id1 == stop_id2){
        cout << EI_LINJAA << endl;
        return;

    } else {

        Reitti.clear();
        reitti_loytyi = false;
        Tarkastettu_linja.clear();

        int aika = sekunneiksi(time);

        //Annetaan hakuehdot agoritmin pureskeltavaksi
        algorithm2(aika, stop_id1, stop_id2);

        //Jos reittia ei loytynyt
        if (!reitti_loytyi){
            cout << EI_LINJAA << endl;

            Reitti.clear();
            reitti_loytyi = false;
            Tarkastettu_linja.clear();

        //Jos reitti loytyi
        } else {

            //Tallennetaan oikea reitti omaan dequeen
            //tulostusta ja virhetarkastelua varten
            string pysakki = stop_id2;
            deque <tripType> path;

            while (pysakki != stop_id1){
                path.push_front(Reitti.find(pysakki)->second);
                pysakki = Reitti.find(pysakki)->second.stop_id;
            }

            unsigned int i = 0;

            //Virhetarkastelu jos taytyy "vaihtaa" samaan linjaan
            while (i + 1 < path.size()){
                if (Vuoro.find(path.at(i).trip_id)->second
                    == Vuoro.find(path.at(i + 1).trip_id)->second
                    && path.at(i).arrival_time
                        != path.at(i + 1).departure_time){

                    Reitti.clear();
                    reitti_loytyi = false;
                    Tarkastettu_linja.clear();

                    cout << EI_LINJAA << endl;
                    return;
                }
                ++i;
            }

            //Reitin tulostaminen
            string numero;
            int tunnit;
            int minuutit;
            int aika;

            unsigned int n = 0;
            i = 0;
            while (i < path.size()) {

                //Jos tarvitsee vaihtaa bussia,
                //tulostetaan vaihtopysakin saapumisaika
                if (Vuoro.find(path.at(i).trip_id)->second
                    != Vuoro.find(path.at(n).trip_id)->second){

                    numero = Vuoro.find(path.at(n).trip_id)->second;
                    aika = path.at(n).arrival_time;
                    minuutit = aika / 60;
                    tunnit = minuutit / 60;
                    minuutit = minuutit % 60;

                    cout << "Bussi " << numero << ": ";
                    if (tunnit < 10) {
                        cout << "0" << tunnit << ":";
                    } else {
                        cout << tunnit << ":";
                    }
                    if (minuutit < 10){
                        cout << "0" << minuutit;
                    } else {
                        cout << minuutit;
                    }

                    cout << " " << path.at(n).next_stop_id << ", "
                         << Pysakki.find(path.at(n).next_stop_id)
                            ->second.stop_name
                         << endl;
                }

                //Tulostetaan lahdon tiedot
                numero = Vuoro.find(path.at(i).trip_id)->second;
                aika = path.at(i).departure_time;
                minuutit = aika / 60;
                tunnit = minuutit / 60;
                minuutit = minuutit % 60;

                cout << "Bussi " << numero << ": ";
                if (tunnit < 10) {
                    cout << "0" << tunnit << ":";
                } else {
                    cout << tunnit << ":";
                }
                if (minuutit < 10){
                    cout << "0" << minuutit;
                } else {
                    cout << minuutit;
                }

                cout << " " << path.at(i).stop_id
                     << ", " << Pysakki.find(path.at(i).stop_id)
                        ->second.stop_name << endl;

                n = i;
                ++i;
            }

            //Ja viimeiselle pysakille saapuminen

            --i;

            numero = Vuoro.find(path.at(i).trip_id)->second;
            aika = path.at(i).arrival_time;
            minuutit = aika / 60;
            tunnit = minuutit / 60;
            minuutit = minuutit % 60;

            cout << "Bussi " << numero << ": ";
            if (tunnit < 10) {
                cout << "0" << tunnit << ":";
            } else {
                cout << tunnit << ":";
            }
            if (minuutit < 10){
                cout << "0" << minuutit;
            } else {
                cout << minuutit;
            }

            cout << " " << path.at(i).next_stop_id
                 << ", "
                 << Pysakki.find(path.at(i).next_stop_id)->second.stop_name
                 << endl;

        }

        //Tyhjennetaan reitin selvittamisessa kaytetyt tietorakenteet
        Reitti.clear();
        reitti_loytyi = false;
        Tarkastettu_linja.clear();
        return;
    }
}


// Kokeillaan taysin toisenlaista algoritmia
// Aikataulullisesti lyhimman reitin etsiva algoritmi
void Datastructure::algorithm2(int time, string stop_id1, string stop_id2){
    //Iteraattori pysakille
    map<string, stopType>::const_iterator stop_iter;
    stop_iter = Pysakki.find( stop_id1 );

    //Iteraattori pysakin linjoille
    map<string, tripType>::const_iterator trip_iter;
    trip_iter = stop_iter->second.yhteys.begin();

    //Laitetaan lahtopysakki jonoon kasiteltavien listaan
    deque<inQueue> jono;
    inQueue seuraava;
    seuraava.stop_id = stop_id1;
    seuraava.arrival_time = time;
    jono.push_back(seuraava);

    //Kaydaan jono loppuun asti, lisataan kaikkien pysakilta
    //lahtevien yhteyksien seuraavat pysakit jonoon
    while (!jono.empty()){
        seuraava = *jono.begin();
        jono.pop_front();
        stop_iter = Pysakki.find(seuraava.stop_id);
        trip_iter = stop_iter->second.yhteys.begin();

        //Ei tutkita maaranpaasta lahtevia yhteyksia
        if (seuraava.stop_id == stop_id2){
            reitti_loytyi = true;
            continue;
        }

        //Kaydaan kaikki pysakilta lahtevat yhteydet lapi vuorotellen
        while (trip_iter != stop_iter->second.yhteys.end()){

            //Ohitetaan ennen pysakille saapumista lahteneet vuorot
            if  (trip_iter->second.departure_time < seuraava.arrival_time){
                ++trip_iter;
                continue;
            }

            //Jos seuraavalla pysakille ei ole viela kayty, lisataan listaan
            if (Reitti.find(trip_iter->second.next_stop_id) == Reitti.end()){
                inQueue uusi;
                uusi.stop_id = trip_iter->second.next_stop_id;
                uusi.arrival_time = trip_iter->second.arrival_time;
                Reitti[uusi.stop_id] = trip_iter->second;
                jono.push_back(uusi);

                continue;
            }

            //Jos saapumisaika pysakille on aiemmin,
            //kuin edellinen loydetty reitti, tallennetaan
            //uudet tiedot
            if (trip_iter->second.arrival_time <
                Reitti.find(trip_iter->second.next_stop_id)
                    ->second.arrival_time){

                inQueue uusi;
                uusi.stop_id = trip_iter->second.next_stop_id;
                uusi.arrival_time = trip_iter->second.arrival_time;
                Reitti[uusi.stop_id] = trip_iter->second;
                jono.push_back(uusi);

                continue;
            }

            ++trip_iter;
        }
    }
}

// B-komento. Tulostaa pysäkin kautta kulkevat reitit
void Datastructure::routesFromStop(const string& stop_id) const{

    // Kaydaan iteraattorin avulla lapi kyseisen pysakin vuorot

    if (Pysakki.find(stop_id) == Pysakki.end()){
        cout << VIRHE << endl;
        return;
    } else {

        map<string, stopType>::const_iterator it;
        it = Pysakki.find( stop_id );
        map<string, tripType>::const_iterator iter;
        iter = it->second.yhteys.begin();

        string trip_id;
        deque<string> linjat;

        while (iter != it->second.yhteys.end()){
            trip_id = iter->first;

            //Tarkastetaan onko vuorolle olemassa tunnus tietokannassa
            if (Vuoro.find(trip_id) == Vuoro.end()){
                cout << EI_LINJAA << endl;
                return;

            } else if (find(linjat.begin(), linjat.end(),
                       Vuoro.find(trip_id)->second ) == linjat.end()){

                linjat.push_back(Vuoro.find(trip_id)->second);
            }
            ++iter;
        }

        if (linjat.empty()){
            return;
        } else {
            sort(linjat.begin(), linjat.end());
            cout << linjat.at(0);

            deque<string>::size_type i = 1;
            while (i < linjat.size()){
                cout << ", " << linjat.at(i);
                ++i;
            }
            cout << endl;
        }
    }
}

// P-komento. Tulostaa pysäkin nimen.
void Datastructure::stopName(const string& stop_id) const{

    map<string, stopType>::const_iterator it;
    it = Pysakki.find( stop_id );

    if (it == Pysakki.end()){
        cout << VIRHE << endl;
        return;

    } else {
        cout << it->second.stop_name << endl;
        return;
    }
}

// C-komento, Tiedot pysäkkien, linjojen ja vuorojen määrästä
void Datastructure::count() const{
    map<string, stopType>::const_iterator it;
    it = Pysakki.begin();
    int pysakkeja = 0;
    int vuoroja = 0;

    //Lasketaan pysakkien maara
    while (it != Pysakki.end()){
        ++pysakkeja;
        ++it;
    }

    //Lasketaan vuorojen maara
    deque<stopTimeType>::const_iterator iter;
    iter = StopTime.begin();
    string trip_id;

    while (iter != StopTime.end()){
        trip_id = iter->trip_id;
        ++vuoroja;

        while (iter != StopTime.end() &&
               trip_id == iter->trip_id){
            ++iter;
        }
    }

    cout << pysakkeja << " pysäkkiä, "
         << Linjoja << " linjaa, "
         << vuoroja << " vuoroa" << endl;
}

// Tyhjentää tietorakenteen. E-komento
void Datastructure::empty(){

    Pysakki.clear();
    StopTime.clear();
    Vuoro.clear();
    Linjoja = 0;

    return;
}

// R-komento. Lukee GTFS muotoisen datan hakemistosta tietorakenteeseen.
void Datastructure::loadData(const string& directory){

    Pysakki.clear();
    StopTime.clear();
    Vuoro.clear();
    Linjoja = 0;

    // Luetaan bussipysakit ja tunnukset listaan
    luePysakit(directory);

    // Luetaan pysakkien valiset yhteydet
    lueYhteydet(directory);

    //Luetaan trip_idt ja vastaavat vuoronumerot
    lueVuoronumerot(directory);

    Graph();

    return;
}

// Funktio lukee tiedostosta kaikki pysakit ja niiden tiedot
void Datastructure::luePysakit(const string& directory){

    string stop_id;

    string file = directory + "/stops.txt";
    ifstream file_stream( file );

    if( !file_stream )
    {
        file_stream.close();
        cout << "Tiedostoa ei löydy." << endl;
        return;

    } else {
        Pysakki.empty();

        string stop_code;
        string stop_name;

        string line;

        //Luetaan otsikkorivi pois
        getline( file_stream, line );

        while( getline( file_stream, line ))
        {
            stopType uusi_pysakki;
            istringstream line_stream( line );
            getline(line_stream, stop_id, ',');
            getline(line_stream, stop_code, ',');
            getline(line_stream, stop_name, ',');

            uusi_pysakki.stop_code = stop_code;
            uusi_pysakki.stop_name = stop_name;
            Pysakki[stop_id] = uusi_pysakki;
        }
        file_stream.close();

    }
}

// Funktio lukee tiedostosta kaikki pysahdykset ja niiden tiedot
void Datastructure::lueYhteydet(const string& directory){

    string file = directory + "/stop_times.txt";
    ifstream file_stream( file );

    if( !file_stream )
    {
        file_stream.close();
        cout << "Tiedostoa ei löydy." << endl;
        return;

    } else {
        int arrival_time;
        int departure_time;

        string tmp;
        int stop_sequence;
        string trip_id;
        string stop_id;
        stopTimeType uusi_pysahdys;
        string line;

        //Luetaan otsikkorivi pois
        getline( file_stream, line );

        while( getline( file_stream, line ))
        {
            istringstream line_stream( line );

            getline(line_stream, trip_id, ',');

            //Saapumisaika
            getline(line_stream, tmp, ',');
            arrival_time = sekunneiksi(tmp);

            //Lahtoaika
            getline(line_stream, tmp, ',');
            departure_time = sekunneiksi(tmp);

            getline(line_stream, stop_id, ',');
            getline(line_stream, tmp);
            istringstream (tmp) >> stop_sequence;

            uusi_pysahdys.trip_id = trip_id;
            uusi_pysahdys.arrival_time = arrival_time;
            uusi_pysahdys.departure_time = departure_time;
            uusi_pysahdys.stop_id = stop_id;
            uusi_pysahdys.stop_sequence = stop_sequence;

            StopTime.push_back(uusi_pysahdys);

        }
        file_stream.close();
    }
}

void Datastructure::lueVuoronumerot(const string& directory){

    string line;
    string file = directory + "/trips.txt";
    ifstream file_stream( file );

    if( !file_stream )
    {
        file_stream.close();
        cout << "Tiedostoa ei löydy." << endl;
        return;

    } else {

        string route_id;
        string trip_id;
        string turha;

        //Luetaan otsikkorivi pois
        getline( file_stream, line );

        while( getline( file_stream, line ))
        {
            istringstream line_stream( line );

            getline(line_stream, route_id, ',');
            getline(line_stream, turha, ',');
            getline(line_stream, trip_id, ',');

            Vuoro[trip_id] = route_id;
        }
        file_stream.close();
    }

    //Luetaan viela linjojen maara toisesta tiedostosta
    file = directory + "/routes.txt";
    ifstream file_stream2( file );

    if( !file_stream2 )
    {
        file_stream2.close();
        cout << "Tiedostoa ei löydy." << endl;
        return;

    } else {
        //Luetaan otsikkorivi pois
        getline( file_stream2, line );

        //Lasketaan vain kokonaismaara, kun tiedolla ei muuta tehda
        while( getline( file_stream2, line )){
            ++Linjoja;
        }
        file_stream2.close();
    }

}

//Funktio tallentaa lahtevat bussiyhteydet jokaiselle pysakille
void Datastructure::Graph(){

    string trip_id;
    string stop_id;
    string next_stop_id;
    int arrival_time;
    int departure_time;

    deque<stopTimeType>::size_type i = 0;
    deque<stopTimeType>::size_type n = 1;

    while (n < StopTime.size()){

        while (n < StopTime.size() &&
               StopTime.at(i).trip_id == StopTime.at(n).trip_id){

            //Vuoron ja pysakin numero
            trip_id = StopTime.at(i).trip_id;
            stop_id = StopTime.at(i).stop_id;

            //Lahtoaika ja saapumisaika seuraavalle pysakille
            departure_time = StopTime.at(i).departure_time;
            arrival_time = StopTime.at(n).arrival_time;

            //Seuraavan pysakin numero
            next_stop_id = StopTime.at(n).stop_id;

            //Tallennetaan arvot oikean pysakin tietoihin
            Pysakki[stop_id].yhteys[trip_id].trip_id = trip_id;
            Pysakki[stop_id].yhteys[trip_id].stop_id = stop_id;
            Pysakki[stop_id].yhteys[trip_id].departure_time = departure_time;
            Pysakki[stop_id].yhteys[trip_id].arrival_time = arrival_time;
            Pysakki[stop_id].yhteys[trip_id].next_stop_id = next_stop_id;

            //Siirrytaan seuraavalle pysakille
            ++i;
            ++n;
        }

        //Siirrytaan seuraavaan bussivuoroon
        ++i;
        ++n;
    }
}

int Datastructure::sekunneiksi (const string& line){

    int aika;
    int paluuarvo;
    string tmp;
    istringstream line_stream( line );

    //Tunnit sekunneiksi
    getline(line_stream, tmp, ':');
    istringstream (tmp) >> paluuarvo;
    paluuarvo = paluuarvo * 3600;

    //Minuutit sekunneiksi
    getline(line_stream, tmp, ':');
    istringstream (tmp) >> aika;
    aika = aika * 60;
    paluuarvo += aika;

    //Saapumisajan sekunnit
    getline(line_stream, tmp, ',');
    istringstream (tmp) >> aika;
    paluuarvo += aika;

    return paluuarvo;
}
