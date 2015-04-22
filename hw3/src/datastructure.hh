// Definition of Datastructure class for UDS/Tiraka homework 3 -

#ifndef _DATASTRUCTURE_HH_
#define _DATASTRUCTURE_HH_

#include <string>

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
                    const std::string& stop_id2 ) const;

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
};

#endif
