// Definition of Datastructure class for UDS/Tiraka homework 3 -

#ifndef _DATASTRUCTURE_HH_
#define _DATASTRUCTURE_HH_

#include <string>
#include <deque>
#include <map>

const std::string EI_LINJAA = "Virhe: Ei linjaa!";
const std::string VIRHE = "Virhe: Pysäkkiä ei ole!";

class Datastructure
{
    public:

    Datastructure();

    ~Datastructure();

    // N-komento. Tulostaa reitin pysäkkeinen, joka on ensimmäisenä perillä
    // stop_id2:ssa.
    void routeSearch(const std::string& time, const std::string& stop_id1,
                    const std::string& stop_id2 );

    // B-komento. Tulostaa pysäkin kautta kulkevat reitit
    void routesFromStop(const std::string& stop_id) const;

    // P-komento. Tulostaa pysäkin nimen.
    void stopName(const std::string& stop_id) const;

    // C-komento, Tiedot pysäkkien, linjojen ja vuorojen määrästä
    void count() const;

    // Tyhjentää tietorakenteen. E-komento
    void empty();

    // R-komento. Lukee GTFS muotoisen datan hakemistosta tietorakenteeseen.
    void loadData(const std::string& directory);

    // Kopiorakentaja kielletty
    Datastructure(const Datastructure&) = delete;

    // Sijoitusoperaattori kielletty
    Datastructure& operator=(const Datastructure&) = delete;

private:
    // Your implementation here

    // Bussiyhteyden tietotyyppi
    struct tripType {

        //Linjan numero
        std::string trip_id;

        //Lahtopysakki
        std::string stop_id;
        // Kohdepysakki
        std::string next_stop_id;

        // Lahtoaika pysakilta
        int departure_time;
        // Sapuminen seuraavalle pysakille
        int arrival_time;
    };

    // Pysakki-tietotyyppi
    struct stopType {
        std::string stop_code;
        std::string stop_name;

        // Yhteydet pysakilta
        // yhteys[trip_id] = tripType
        std::map<std::string, tripType> yhteys;

    };

    // Oma tietotyyppi pysahdyksille
    struct stopTimeType {
        std::string trip_id;
        int departure_time;
        int arrival_time;
        std::string stop_id;
        int stop_sequence;
    };

    struct routeType {
        std::string route_id;
        int departure_time;
    };

    struct inQueue {
        std::string stop_id;
        int arrival_time;
    };

    void luePysakit(const std::string& directory);
    void lueYhteydet(const std::string& directory);
    void lueVuoronumerot(const std::string& directory);
    void Graph();
    int sekunneiksi (const std::string& line);
    void algorithm2(int time, std::string stop_id1, std::string stop_id2);

    // Kaikki pysakit tallennetaan omaan mappiin
    // Tallennetaan stop_id avaimen taakse stopType tyyppiin pysakin tiedot
    // Pysakki[stop_id] = stopType
    std::map<std::string, stopType> Pysakki;

    // Tallennetaan kaikki pysahdysajat omaan dequeen
    std::deque<stopTimeType> StopTime;

    // Tallennetaan kaikki vuorot mappiin
    // Avaimena trip_id, arvona route_id
    std::map<std::string, std::string> Vuoro;

    //Joku toteutus tarkastelemaan, ettei saman linjan kaikkia vuoroja kayda lapi
    // Tarkastettu_linja[stop_id] = tripType
    std::map<std::string, routeType> Tarkastettu_linja;

    //Linjojen kokonaismaara
    int Linjoja = 0;

    //Tarvittavia muuttujia reitin tallentamiseen
    bool reitti_loytyi = false;
    std::map<std::string, tripType> Reitti;

};

#endif
