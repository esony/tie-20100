/*
 *
 *
 */

/*

 * Pysakit
 * stops.txt
 *
 * stop_id,     stop_code,  stop_name,      stop_lat, stop_lon
 * 4078,        4078,       Kaukajärvi,     61.47075, 23.88934


 * Pysahtymisajat pysakeilla
 * stop_times.txt
 *
 * trip_id,     arrival_time,   departure_time,     stop_id,    stop_sequence
 * 4361253488,  16:50:00,       16:50:00,           4078,       1


 * Matkat
 * trips.txt
 *
 * route_id,    service_id,         trip_id,    trip_headsign,  direction_id,   shape_id
 * 10,          TAL_ARKI_K5_2015,   4361253488, Keskustori,     0,              1244140780008

 */


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
                const string& stop_id2 ) const{
    string time2;

    if (Pysakki.find(stop_id1) == Pysakki.end() ||
        Pysakki.find(stop_id2) == Pysakki.end()){

        cout << VIRHE << endl;
        return;
    }
    time2 = time;
    cout << EI_LINJAA << endl;
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
//        iter = Pysakki.find(stop_id)->second.yhteys.begin();
        iter = it->second.yhteys.begin();

        string trip_id;

        deque<string> linjat;

        while (iter != it->second.yhteys.end()){
            trip_id = iter->first;

            //Tarkastetaan onko vuorolle olemassa tunnus tietokannassa
            if (Vuoro.find(trip_id) == Vuoro.end()){
                cout << EI_LINJAA << endl;
                return;

            } else if (find(linjat.begin(),
                            linjat.end(),
                            Vuoro.find(trip_id)->second ) == linjat.end()){

                linjat.push_back(Vuoro.find(trip_id)->second);
            }

            ++iter;
        }

        if (linjat.size() == 0){
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
        int aika;
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

            //Saapumisajan tunnit, muutetaan sekunneiksi
            getline(line_stream, tmp, ':');
            istringstream (tmp) >> arrival_time;
            arrival_time = arrival_time * 3600;

            //Saapumisajan minuutit, muutetaan sekunneiksi
            getline(line_stream, tmp, ':');
            istringstream (tmp) >> aika;
            aika = aika * 60;
            arrival_time += aika;

            //Saapumisajan sekunnit
            getline(line_stream, tmp, ',');
            istringstream (tmp) >> aika;
            arrival_time += aika;



            //Lahtoajan tunnit, muutetaan sekunneiksi
            getline(line_stream, tmp, ':');
            istringstream (tmp) >> departure_time;
            departure_time = departure_time * 3600;

            //Saapumisajan minuutit, muutetaan sekunneiksi
            getline(line_stream, tmp, ':');
            istringstream (tmp) >> aika;
            aika = aika * 60;
            departure_time += aika;

            //Saapumisajan sekunnit
            getline(line_stream, tmp, ',');
            istringstream (tmp) >> aika;
            departure_time += aika;

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

        while (n < StopTime.size() && StopTime.at(i).trip_id == StopTime.at(n).trip_id){

            //Vuoron ja pysakin numero
            trip_id = StopTime.at(i).trip_id;
            stop_id = StopTime.at(i).stop_id;

            //Lahtoaika ja saapumisaika seuraavalle pysakille
            departure_time = StopTime.at(i).departure_time;
            arrival_time = StopTime.at(n).arrival_time;

            //Seuraavan pysakin numero
            next_stop_id = StopTime.at(n).stop_id;

            //Tallennetaan arvot oikean pysakin tietoihin
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
