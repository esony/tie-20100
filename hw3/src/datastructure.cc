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

Datastructure::Datastructure(){

}

Datastructure::~Datastructure(){

}

// N-komento. Tulostaa reitin pysäkkeinen, joka on ensimmäisenä perillä
// stop_id2:ssa.
void Datastructure::routeSearch(const std::string& time, const std::string& stop_id1,
                const std::string& stop_id2 ) const{

}

// B-komento. Tulostaa pysäkin kautta kulkevat reitit
void Datastructure::routesFromStop(const std::string& stop_id) const{

}

// P-komento. Tulostaa pysäkin nimen.
void Datastructure::stopName(const std::string& stop_id) const{

}

// C-komento, Tiedot pysäkkien, linjojen ja vuorojen määrästä
void Datastructure::count() const{

}

// Tyhjentää tietorakenteen. E-komento
void Datastructure::empty(){

}

// R-komento. Lukee GTFS muotoisen datan hakemistosta tietorakenteeseen.
void Datastructure::loadData(const std::string& directory){

}
