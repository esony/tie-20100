#include "catch.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <tuple>

#include "datastructure.hh"
#include "ioredirector.hh"
#include "signaltranslator.hh"
#include "utility.hh"

#ifndef TESTMATERIALDIR
#define TESTMATERIALDIR "material"
#endif

const std::string TESTDIR=std::string(TESTMATERIALDIR);

/*
Writing tests:
https://github.com/philsquared/Catch/tree/master/docs
https://github.com/philsquared/Catch/blob/master/docs/tutorial.md
*/

SCENARIO( "Tyhjältä tietorakenteelta määrän kysyminen", "[C][1]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN("Tulostetaan pysäkkien, linjojen ja vuorojen määrä")
        {
            redirector.start();
            DS.count();
            result = redirector.stop();
            THEN( "Määrät ovat oikeat")
            {
                REQUIRE( result == "0 pysäkkiä, 0 linjaa, 0 vuoroa" );
            }
        }
    }
}

SCENARIO( "Datan lukeminen ja määrien kysyminen", "[R][C][2]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN("Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            AND_WHEN("Tulostetaan pysäkkien, linjojen ja vuorojen määrä")
            {
                redirector.start();
                DS.count();
                result = redirector.stop();
                THEN( "Määrät ovat oikeat")
                {
                    REQUIRE( result == "2169 pysäkkiä, 53 linjaa, 3622 vuoroa");
                }
            }
        }
    }
}

SCENARIO( "Tietorakenteen tyhjentäminen", "[E][3]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            AND_WHEN("Tyhjennetään (E-komento)")
            {
                DS.empty();

                AND_WHEN("Tulostetaan pysäkkien, linjojen ja vuorojen määrä")
                {
                    redirector.start();
                    DS.count();
                    result = redirector.stop();
                    THEN( "Määrät ovat oikeat")
                    {
                        REQUIRE( result == "0 pysäkkiä, 0 linjaa, 0 vuoroa" );
                    }
                }
            }
        }
    }
}

SCENARIO( "Tyhjän tietorakenteen tyhjentäminen", "[E][4]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN("Tyhjennetään (E-komento)")
        {
            DS.empty();

            AND_WHEN("Tulostetaan pysäkkien, linjojen ja vuorojen määrä")
            {
                redirector.start();
                DS.count();
                result = redirector.stop();
                THEN( "Määrät ovat oikeat")
                {
                    REQUIRE( result == "0 pysäkkiä, 0 linjaa, 0 vuoroa" );
                }
            }
        }
    }
}

SCENARIO( "Pysäkin kautta kulkevat linjat, yksi linja", "[B][5]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            AND_WHEN("Haetaan pysäkin 5413 kautta kulkevia linjoja (B-komento)")
            {
                redirector.start();
                DS.routesFromStop("5413");
                result = redirector.stop();
                THEN( "Tulostetaan pysäkin kautta kulkevat linjat")
                {
                    REQUIRE( result == "91" );
                }
            }
        }
    }
}

SCENARIO( "Pysäkin kautta kulkevat linjat, monta linjaa", "[B][6]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            AND_WHEN("Haetaan pysäkin 3735 kautta kulkevia linjoja (B-komento)")
            {
                redirector.start();
                DS.routesFromStop("3735");
                result = redirector.stop();
                THEN( "Tulostetaan pysäkin kautta kulkevat linjat")
                {
                    REQUIRE( result == "13, 20, 24, 38, 65" );
                }
            }
        }
    }
}

SCENARIO( "Pysäkin kautta kulkevat linjat, ei pysäkkiä", "[B][7]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            AND_WHEN("Haetaan pysäkin 9999 kautta kulkevia linjoja (B-komento)")
            {
                redirector.start();
                DS.routesFromStop("9999");
                result = redirector.stop();
                THEN( "Tulostetaan virheilmoitus")
                {
                    REQUIRE( result == VIRHE );
                }
            }
        }
    }
}

SCENARIO( "Pysäkin kautta kulkevat linjat, ei pysäkkiä, haetaan tyhjästä tietorakenteesta", "[B][8]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN("Haetaan pysäkin 3735 kautta kulkevia linjoja (B-komento)")
        {
            redirector.start();
            DS.routesFromStop("3735");
            result = redirector.stop();
            THEN( "Tulostetaan virheilmoitus")
            {
                REQUIRE( result == VIRHE );
            }
        }
    }
}

SCENARIO( "Pysäkin kautta kulkevat linjat, ei pysäkkiä, haetaan tyhjennetystä tietorakenteesta", "[B][9]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            AND_WHEN("Tyhjennetään (E-komento)")
            {
                DS.empty();

                AND_WHEN("Haetaan pysäkin 3735 kautta kulkevia linjoja (B-komento)")
                {
                    redirector.start();
                    DS.routesFromStop("3735");
                    result = redirector.stop();
                    THEN( "Tulostetaan virheilmoitus")
                    {
                        REQUIRE( result == VIRHE );
                    }
                }
            }
        }
    }
}

SCENARIO( "Pysäkin kautta kulkevat linjat, pysäkki ennen tyhjennystä olemassa, haetaan tyhjennetystä tietorakenteesta", "[B][10]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            AND_WHEN("Tyhjennetään (E-komento)")
            {
                DS.empty();

                AND_WHEN("Haetaan pysäkin 5413 kautta kulkevia linjoja (B-komento)")
                {
                    redirector.start();
                    DS.routesFromStop("5413");
                    result = redirector.stop();
                    THEN( "Tulostetaan virheilmoitus")
                    {
                        REQUIRE( result == VIRHE );
                    }
                }
            }
        }
    }
}


SCENARIO( "Pysäkin nimi, yksiosainen nimi", "[P][11]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            AND_WHEN("Haetaan pysäkin 1556 nimeä (P-komento)")
            {
                redirector.start();
                DS.stopName("1556");
                result = redirector.stop();
                THEN( "Tulostetaan pysäkin nimi")
                {
                    REQUIRE( result == "Siirtolapuutarha" );
                }
            }
        }
    }
}

SCENARIO( "Pysäkin nimi, moniosainen nimi", "[P][12]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            AND_WHEN("Haetaan pysäkin 1029 nimeä (P-komento)")
            {
                redirector.start();
                DS.stopName("1029");
                result = redirector.stop();
                THEN( "Tulostetaan pysäkin nimi")
                {
                    REQUIRE( result == "Pohtolankatu 25" );
                }
            }
        }
    }
}

SCENARIO( "Pysäkin nimi, monimutkainen nimi", "[P][13]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            AND_WHEN("Haetaan pysäkin 10010 nimeä (P-komento)")
            {
                redirector.start();
                DS.stopName("10010");
                result = redirector.stop();
                THEN( "Tulostetaan pysäkin nimi")
                {
                    REQUIRE( result == "HAMK (Valkeakoski)" );
                }
            }
        }
    }
}

SCENARIO( "Pysäkin nimi, ei pysäkkiä", "[P][14]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            AND_WHEN("Haetaan pysäkin 9999 nimeä (P-komento)")
            {
                redirector.start();
                DS.stopName("9999");
                result = redirector.stop();
                THEN( "Tulostetaan virheilmoitus")
                {
                    REQUIRE( result == VIRHE );
                }
            }
        }
    }
}

SCENARIO( "Pysäkin nimi, ei pysäkkiä, haetaan tyhjästä tietorakenteesta", "[P][15]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN("Haetaan pysäkin 1556 nimeä (P-komento)")
        {
            redirector.start();
            DS.stopName("1556");
            result = redirector.stop();
            THEN( "Tulostetaan virheilmoitus")
            {
                REQUIRE( result == VIRHE );
            }
        }
    }
}

SCENARIO( "Pysäkin nimi, ei pysäkkiä, haetaan tyhjennetystä tietorakenteesta", "[P][16]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");

            AND_WHEN("Tyhjennetään (E-komento)")
            {
                DS.empty();

                AND_WHEN("Haetaan pysäkin 9999 nimeä (P-komento)")
                {
                    redirector.start();
                    DS.stopName("9999");
                    result = redirector.stop();
                    THEN( "Tulostetaan virheilmoitus")
                    {
                        REQUIRE( result == VIRHE );
                    }
                }
            }
        }
    }
}

SCENARIO( "Pysäkin nimi, pysäkki ennen tyhjennystä olemassa, haetaan tyhjennetystä tietorakenteesta", "[P][17]" )
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");

            AND_WHEN("Tyhjennetään (E-komento)")
            {
                DS.empty();

                AND_WHEN("Haetaan pysäkin 1556 nimeä (P-komento)")
                {
                    redirector.start();
                    DS.stopName("1556");
                    result = redirector.stop();
                    THEN( "Tulostetaan virheilmoitus")
                    {
                        REQUIRE( result == VIRHE );
                    }
                }
            }
        }
    }
}

void reitti_testi(const std::string& stop1, const std::string& stop2,
                  const std::string& time,
                  const std::string& material,
                  const std::string& outfile )
{
    set_limits();
    std::string directory = TESTDIR + "/" + material;

    std::ifstream ifile(outfile);
    std::stringstream buffer;

    buffer << ifile.rdbuf();
    ifile.close();

    std::string expected_result{buffer.str()};

    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data '" + material + "' (R-komento)")
        {
            DS.loadData(directory);
            AND_WHEN("Haetaan nopein reitti pysäkiltä " + stop1 +
                     " pysäkille " + stop2 + " kellonajan " +  time + " jälkeen.")
            {
                redirector.start();
                DS.routeSearch(time, stop1, stop2);
                result = redirector.stop();
                THEN( "Tulostetaan reitin tiedot")
                {
                    REQUIRE( result == expected_result );
                }
            }
        }
    }
}

std::vector<std::string> lue_reitit( const std::string& outfile,
                                     unsigned int trips)
{
    set_limits();
    std::vector<std::string> alternatives;
    for(unsigned int i = 1; i <= trips; i++)
    {
        std::ifstream ifile(outfile + "." + std::to_string(i));
        std::stringstream buffer;

        buffer << ifile.rdbuf();
        ifile.close();

        std::string expected_result{buffer.str()};

        if( alternatives.empty() )
        {
            alternatives.push_back(expected_result);
        }
        else
        {
            for( auto& j: alternatives )
            {
                j += expected_result;
            }

        }
        if( i < trips )
        {
            std::vector<std::string> from;

            std::string line;

            std::ifstream ifile(outfile + "." + std::to_string(i) + ".transfer.1");
            while( std::getline(ifile, line ))
            {
                from.push_back(line + "\n");
            }

            ifile.close();

            std::vector<std::string> to;
            ifile.open(outfile + "." + std::to_string(i) + ".transfer.2");
            while( std::getline(ifile, line ))
            {
                to.push_back(line + "\n");
            }
            ifile.close();


            std::vector<std::string> combinations;
            std::string combination;
            for( unsigned int j = 0; j < from.size(); ++j )
            {
                combination = std::accumulate(from.begin(),
                                              from.begin() + j + 1,
                                              std::string(""));

                combination = std::accumulate(to.begin() + j,
                                              to.end(),
                                              combination);
                combinations.push_back(combination);
            }

            std::vector<std::string> tmp;

            for( const auto& j: alternatives )
            {

                for( const auto& k: combinations )
                {
                    std::string tmp_string = j + k;
                    tmp.push_back(tmp_string);
                }
            }
            alternatives = tmp;
        }
    }

    std::vector<std::string> results;
    for( auto& i: alternatives )
    {
       results.push_back(trim_multiline_string(i));
    }
    return results;
}

void reitti_testi_vaihdoilla(const std::string& stop1,
                                 const std::string& stop2,
                                 const std::string& time,
                                 const std::string& material,
                                 const std::string& outfile,
                                 unsigned int trips)
{
    set_limits();
    std::string directory = TESTDIR + "/" + material;

    std::vector<std::string> alternatives = lue_reitit(outfile, trips);

    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data '" + material + "' (R-komento)")
        {
            DS.loadData(directory);
            AND_WHEN("Haetaan nopein reitti pysäkiltä " + stop1 +
                     " pysäkille " + stop2 + " kellonajan " +  time + " jälkeen.")
            {
                redirector.start();
                DS.routeSearch(time, stop1, stop2);
                result = redirector.stop();
                THEN( "Tulostetaan reitin tiedot")
                {
                    bool found = false;
                    for( auto j = alternatives.begin();
                         j != alternatives.end(); ++j)
                    {
                        std::string expected_result = *j;

                        if( result == expected_result )
                        {
                            found = true;
                            REQUIRE( result == expected_result );
                            break;
                        }
                    }
                    if( ! found )
                    {
                        std::string all_alternative_routes;
                        for( auto j = alternatives.begin(); j != alternatives.end(); ++j)
                        {
                            all_alternative_routes += *j;
                            if( std::distance(j, alternatives.end()) > 1 )
                            {
                                all_alternative_routes += "\n\nTAI\n\n";
                            }
                        }
                        INFO("Tällä reitillä voi vaihto tapahtua useammassa kohdassa. Mikä tahansa edellä mainituista reiteistä hyväksytään oikeaksi vastaukseksi.");
                        REQUIRE( result == all_alternative_routes );
                    }
                }
            }
        }
    }
}

std::tuple<std::string,std::string> last_n_lines(const std::string& line_to_check, unsigned int lines)
{
    std::stringstream ss{line_to_check};
    std::vector<std::string> elems;
    std::string line;

    while (std::getline(ss, line ) )
    {
        elems.push_back(line);
    }

    std::ostringstream ofs_start;
    for( unsigned int i = 0; i < elems.size() - lines; i++ )
    {
        ofs_start << elems.at(i) << std::endl;
    }

    std::ostringstream ofs_end;
    for( unsigned int i = elems.size() - lines; i < elems.size(); i++ )
    {
        ofs_end << elems.at(i) << std::endl;
    }
    return std::make_tuple(trim_multiline_string(ofs_start.str()),
                           trim_multiline_string(ofs_end.str()));
}

void reitti_testi_loppuosa(const std::string& stop1,
                           const std::string& stop2,
                           const std::string& time,
                           const std::string& material,
                           const std::string& outfile,
                           unsigned int stops_to_check)
{
    set_limits();
    std::string directory = TESTDIR + "/" + material;

    std::ifstream ifile(outfile);
    std::stringstream buffer;

    buffer << ifile.rdbuf();
    ifile.close();

    std::string expected_result{trim_multiline_string(buffer.str())};

    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data '" + material + "' (R-komento)")
        {
            DS.loadData(directory);
            AND_WHEN("Haetaan nopein reitti pysäkiltä " + stop1 +
                     " pysäkille " + stop2 + " kellonajan " +  time + " jälkeen.")
            {
                redirector.start();
                DS.routeSearch(time, stop1, stop2);
                result = redirector.stop();
                std::string not_checked;
                std::tie(not_checked, result) = last_n_lines(result, stops_to_check);
                THEN( "Tulostetaan reitin tiedot")
                {
                    INFO("Tässä testissä katsotaan vain reitin loppuosaa, joka on yksikäsitteinen. Reitin alkuosa voi kulkea useampaakin eri reittiä, jonka takia sitä ei tarkasteta. Sinun reittisi alkuosa kulki seuraavasti:\n\n" + not_checked);
                    REQUIRE( result == expected_result );
                }
            }
        }
    }
}



void reitti_testi_ei_vuoroa(const std::string& stop1, const std::string& stop2,
                  const std::string& time,
                  const std::string& material)
{

    set_limits();
    std::string directory = TESTDIR + "/" + material;

    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data '" + material + "' (R-komento)")
        {
            DS.loadData(directory);
            AND_WHEN("Haetaan nopein reitti pysäkiltä " + stop1 +
                     " pysäkille " + stop2 + " kellonajan " +  time + " jälkeen.")
            {
                redirector.start();
                DS.routeSearch(time, stop1, stop2);
                result = redirector.stop();
                THEN( "Tulostetaan EI_LINJAA")
                {
                    REQUIRE( result == EI_LINJAA );
                }
            }
        }
    }
}


void reitti_testi_tyhja(const std::string& stop1, const std::string& stop2,
                        const std::string& time)
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN("Haetaan nopein reitti pysäkiltä " + stop1 +
             " pysäkille " + stop2 + " kellonajan " +  time + " jälkeen.")
        {
            redirector.start();
            DS.routeSearch(time, stop1, stop2);
            result = redirector.stop();
            THEN( "Tulostetaan VIRHE")
            {
                REQUIRE( result == VIRHE );
            }
        }
    }
}

void reitti_testi_tyhjennetty(const std::string& stop1,
                              const std::string& stop2,
                              const std::string& time)
{
    set_limits();
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        IORedirector redirector;
        std::string result;
        Datastructure DS;

        WHEN( "Luetaan data 'full' (R-komento)")
        {
            DS.loadData(TESTDIR + "/full");
            DS.empty();
            AND_WHEN("Haetaan nopein reitti pysäkiltä " + stop1 +
                     " pysäkille " + stop2 + " kellonajan " +  time + " jälkeen.")
            {
                redirector.start();
                DS.routeSearch(time, stop1, stop2);
                result = redirector.stop();
                THEN( "Tulostetaan VIRHE")
                {
                    REQUIRE( result == VIRHE );
                }
            }
        }
    }
}

SCENARIO( "Nopein reitti. Pelkästään suora yhteys olemassa. Pieni testiaineisto", "[N][18][simple]" )
{
    reitti_testi("3549", "0601", "16:50", "simple",
                 TESTDIR + "/simple/testdata_out/suora.out");
}

SCENARIO( "Nopein reitti. Suora yhteys olemassa kahdella eri vuorolla, lähtöajankohta ennen ensimmäistä vuoroa. Pieni testiaineisto", "[N][19][simple]" )
{
    reitti_testi("5404", "5422", "11:00", "simple",
                 TESTDIR + "/simple/testdata_out/suora2.out");
}

SCENARIO( "Nopein reitti. Suora yhteys olemassa kahdella eri vuorolla, lähtöajankohta ennen toista vuoroa. Pieni testiaineisto", "[N][20][simple]" )
{
    reitti_testi("5404", "5422", "12:00", "simple",
                 TESTDIR + "/simple/testdata_out/suora3.out");
}

SCENARIO( "Nopein reitti. Suora yhteys olemassa kahdella eri vuorolla, lähtöajankohta viimeisen vuoron jälkeen. Pieni testiaineisto", "[N][21][simple]" )
{
    reitti_testi_ei_vuoroa("5404", "5422", "20:00", "simple");
}

SCENARIO( "Nopein reitti. Reitti yhdellä vaihdolla. Pieni testiaineisto", "[N][22][simple]" )
{
    reitti_testi_vaihdoilla("3735", "7035", "16:30", "simple",
                            TESTDIR + "/simple/testdata_out/yksi_vaihto_ei_suoraa.out",
                            2);
}

SCENARIO( "Nopein reitti. Suora yhteys nopeampi kuin vaihdolla. Pieni testiaineisto", "[N][23][simple]" )
{
    reitti_testi("3737", "3505", "12:30", "simple",
                 TESTDIR + "/simple/testdata_out/suora4.out");
}

SCENARIO( "Nopein reitti. Yhdellä vaihdolla nopeampi kuin suora reitti. Pieni testiaineisto", "[N][24][simple]" )
{
    reitti_testi("3737", "3505", "12:28", "simple",
                 TESTDIR + "/simple/testdata_out/yksi_vaihto_nopein.out");
}

SCENARIO( "Nopein reitti. Ei reittiä pysäkkien välillä. Pieni testiaineisto", "[N][25][simple]" )
{
    reitti_testi_ei_vuoroa("3737", "4078", "12:28", "simple");
}

SCENARIO( "Nopein reitti. Pelkästään suora yhteys olemassa. Koko testiaineisto", "[N][26][full]" )
{
    reitti_testi("5404", "5425", "14:41", "full",
                 TESTDIR + "/full/testdata_out/suora.out");
}

SCENARIO( "Nopein reitti. Suora yhteys nopein. Koko testiaineisto", "[N][27][full]" )
{
    reitti_testi("3737", "3503", "12:30", "full",
                 TESTDIR + "/full/testdata_out/suora2.out");
}

SCENARIO( "Nopein reitti. Reitti yhdellä vaihdolla nopeampi kuin suora reitti. Koko testiaineisto", "[N][28][full]" )
{
    reitti_testi("3737", "3503", "12:28", "full",
                 TESTDIR + "/full/testdata_out/yksi_vaihto_nopein.out");
}

SCENARIO( "Nopein reitti. Yksi vaihto ja suoraa yhteyttä ei olemassa. Koko testiaineisto", "[N][29][full]" )
{

    reitti_testi_vaihdoilla("0513", "3012", "16:12", "full",
                            TESTDIR + "/full/testdata_out/yksi_vaihto_ei_suoraa.out",
                            2);
}

SCENARIO( "Nopein reitti. Yksi vaihto, toinen esimerkki. Koko testiaineisto", "[N][30][full]" )
{
    reitti_testi("3937", "4025", "13:01", "full",
                 TESTDIR + "/full/testdata_out/yksi_vaihto_nopein2.out" );
}

SCENARIO( "Nopein reitti. Kolme vaihtoa, tarkastetaan vain reitin loppuosa. Koko testiaineisto", "[N][31][full]" )
{
    reitti_testi_loppuosa("5425", "7144", "13:00", "full",
                          TESTDIR + "/full/testdata_out/kolme_vaihtoa.out.4",
                          30);
}

SCENARIO( "Nopein reitti. Perillä seuraavan vuorokauden aikana. Koko testiaineisto", "[N][32][full]" )
{
    reitti_testi_ei_vuoroa("3523", "0052", "23:38", "full");
}

SCENARIO( "Nopein reitti. Lähtöajankohta viimeisen vuoron jälkeen mahdollisesti seuraavana päivänä. Koko testiaineisto", "[N][33][full]" )
{
    reitti_testi_ei_vuoroa("3523", "0501", "23:40", "full");
}

SCENARIO( "Nopein reitti. Lähtöajankohta viimeisen vuoron jälkeen, mahdollisesti perillä seuraavan vuorokauden aikana. Koko testiaineisto", "[N][34][full]" )
{
    reitti_testi_ei_vuoroa("3523", "0052", "23:40", "full");
}

SCENARIO( "Nopein reitti. Ei yhteyttä. Koko testiaineisto", "[N][35][full]" )
{
    reitti_testi_ei_vuoroa("0001", "1651", "00:05", "full");
}

SCENARIO( "Nopein reitti. Ei yhteyttä, koska ainoa yhteys käyttäen samaa linjaa uudestaan, mikä ei ole sallittua. Koko testiaineisto", "[N][36][full]" )
{
    reitti_testi_ei_vuoroa("5412", "5417", "16:00", "full");
}

SCENARIO( "Reitin kysyminen tyhjältä tietorakenteelta", "[N][37][full]" )
{
    reitti_testi_tyhja("3525", "2012", "12:15");
}

SCENARIO( "Reitin kysyminen tyhjennetyltä tietorakenteelta", "[N][38][full]" )
{
    reitti_testi_tyhjennetty("3525", "2012", "12:15");
}
