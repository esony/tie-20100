#include "catch.hpp"
#include <iostream>
#include <string>
#include <fstream>

#include "datastructure.hh"
#include "utility.hh"
#include "ioredirector.hh"
#include "signaltranslator.hh"

using std::string;
const string TESTDIR=std::string(SOURCEDIR) + "/";

/*
Writing tests:
https://github.com/philsquared/Catch/tree/master/docs
https://github.com/philsquared/Catch/blob/master/docs/tutorial.md
*/

SCENARIO( "Määrän tutkiminen tyhjälle tietorakenteelle", "[C]" )
{

    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS

        Datastructure DS;
        THEN( "Tietorakenteessa on 0 eri makeistyyppiä")
        {
            REQUIRE( DS.count() == 0 );

            AND_WHEN( "Tyhjennetään")
            {
                DS.empty();
                THEN( "Tietorakenteessa on 0 eri makeistyyppiä")
                {
                    REQUIRE( DS.count() == 0 );
                }
            }
        }
    }
}

SCENARIO( "Tyhjästä tietorakenteesta hakeminen", "[F]" )
{


    IORedirector redirector;
    std::string result;
    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );

        WHEN("Haetaan ID:llä LKS:1235:123")
        {
            redirector.start();
            DS.find("LKS:1235:123");
            result = redirector.stop();

            THEN("Tulostetaan ei varastossa")
            {
                REQUIRE(result == EI_SAATAVILLA);
            }
        }
    }
}

SCENARIO( "Uuden makeisen lisääminen tyhjään tietorakenteeseen", "[A]" )
{

    IORedirector redirector;
    std::string result;

    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );

        WHEN("Lisätään makeinen 'LKS:1234:123 42.2 50 Panda Lakupala'")
        {
            DS.add("LKS:1234:123", "42.2", 50, "Panda Lakupala");
            THEN("Tietorakenteessa on 1 eri makeistyyppiä")
            {
                REQUIRE( DS.count() == 1);
            }
            AND_WHEN("Haetaan ID:llä 'LKS:1234:123'")
            {
                redirector.start();
                DS.find("LKS:1234:123");
                result = redirector.stop();

                THEN("Tulostetaan tuotteen varastosaldo")
                {
                    REQUIRE( result == "Panda Lakupala 50 42.2");
                }
            }
        }
    }
}

SCENARIO( "Uuden makeisen lisääminen", "[A]" )
{


    IORedirector redirector;
    std::string result;

    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );
        ReadFile( TESTDIR + "varasto1.txt", DS );
        REQUIRE( DS.count() == 5 );

        WHEN("Lisätään makeinen 'TIK:1231:321 91.9 11 Tikkari'")
        {
            DS.add("TIK:1231:321", "91.9", 11, "Tikkari");
            THEN("Tietorakenteessa on 6 eri makeistyyppiä")
            {
                REQUIRE( DS.count() == 6);
            }
            AND_WHEN("Haetaan ID:llä 'TIK:1231:321'")
            {
                redirector.start();
                DS.find("TIK:1231:321");
                result = redirector.stop();

                THEN("Tulostetaan tuotteen varastosaldo")
                {
                    REQUIRE( result == "Tikkari 11 91.9");
                }
            }
        }
    }
}

SCENARIO( "Varastosaldon kasvattaminen", "[A]" )
{


    IORedirector redirector;
    std::string result;

    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );

        WHEN("Lisätään makeinen 'LKS:1234:123 42.2 50 Panda Lakupala'")
        {
            DS.add("LKS:1234:123", "42.2", 50, "Panda Lakupala");
            THEN("Tietorakenteessa on 1 eri makeistyyppiä")
            {
                REQUIRE( DS.count() == 1);
            }
            AND_WHEN("Haetaan ID:llä 'LKS:1234:123'")
            {
                redirector.start();
                DS.find("LKS:1234:123");
                result = redirector.stop();
                THEN("Tulostetaan tuotteen varastosaldo")
                {
                    REQUIRE( result == "Panda Lakupala 50 42.2");
                }
            }
            AND_WHEN("Lisätään makeinen 'LKS:1234:123 42.2 55 Panda Lakupala'")
            {
                DS.add("LKS:1234:123", "42.2", 55, "Panda Lakupala");
                THEN("Tietorakenteessa on 1 eri makeistyyppiä")
                {
                    REQUIRE( DS.count() == 1);
                }
                AND_WHEN("Haetaan ID:llä 'LKS:1234:123'")
                {
                    redirector.start();
                    DS.find("LKS:1234:123");
                    result = redirector.stop();

                    THEN("Tulostetaan tuotteen varastosaldo")
                    {
                        REQUIRE( result == "Panda Lakupala 105 42.2");
                    }
                }
            }
            AND_WHEN("Lisätään makeinen 'SUK:8787:324 93.93 120 Fazer Maitosuklaa'")
            {
                DS.add("SUK:8787:324", "93.93", 120, "Fazer Maitosuklaa");
                THEN("Tietorakenteessa on 2 eri makeistyyppiä")
                {
                    REQUIRE( DS.count() == 2);
                }
                AND_WHEN("Haetaan ID:llä 'SUK:8787:324'")
                {
                    redirector.start();
                    DS.find("SUK:8787:324");
                    result = redirector.stop();

                    THEN("Tulostetaan tuotteen varastosaldo")
                    {
                        REQUIRE( result == "Fazer Maitosuklaa 120 93.93");
                    }
                }
            }
        }
    }
}

SCENARIO( "Määrän tutkiminen", "[C]" )
{

    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );

        WHEN( "Luetaan alkutilanne varasto1.txt" ) {
            ReadFile( TESTDIR + "varasto1.txt", DS );
            THEN( "Tietorakenteessa on 5 eri makeistyyppiä")
            {
                REQUIRE( DS.count() == 5 );

                AND_WHEN( "Tyhjennetään")
                {
                    DS.empty();
                    THEN( "Tietorakenteessa on 0 eri makeistyyppiä")
                    {
                        REQUIRE( DS.count() == 0 );
                    }
                }
            }
        }
    }
}

SCENARIO( "Vähennetään makeisen varastosaldoa", "[D]" )
{


    IORedirector redirector;
    std::string result;

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );
        ReadFile( TESTDIR + "varasto1.txt", DS );
        REQUIRE( DS.count() == 5 );

        WHEN("Poistetaan makeista 'SUK:8787:324' 21 kappaletta")
        {
            redirector.start();
            DS.substract("SUK:8787:324", 21);
            result = redirector.stop();

            THEN("Tulostetaan tuotteen varastosaldo")
            {
                REQUIRE( result == "Saldo: 99 Hylly: 93.93");
                AND_THEN("Määrä on 5")
                {
                    REQUIRE( DS.count() == 5);
                }
            }

            AND_WHEN("Haetaan ID:llä 'SUK:8787:324'")
            {
                redirector.start();
                DS.find("SUK:8787:324");
                result = redirector.stop();

                THEN("Tulostetaan tuotteen varastosaldo")
                {
                    REQUIRE( result == "Fazer Maitosuklaa 99 93.93");
                }
            }
        }
    }
}

SCENARIO( "Poistetaan tyhjästä tietorakenteesta", "[D]" )
{


    IORedirector redirector;
    std::string result;

    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );

        WHEN("Poistetaan makeista 'SUK:8787:324' 21 kappaletta")
        {
            redirector.start();
            DS.substract("SUK:8787:324", 21);
            result = redirector.stop();

            THEN("Tulostetaan EI_SAATAVILLA")
            {
                REQUIRE( result == EI_SAATAVILLA);
                AND_THEN("Tietorakenteessa on 0 eri makeistyyppiä")
                {
                    REQUIRE( DS.count() == 0);
                }
            }
        }
    }
}


SCENARIO( "Vähennetään makeisen varastosaldoa liikaa", "[D]" )
{


    IORedirector redirector;
    std::string result;

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );
        ReadFile( TESTDIR + "varasto1.txt", DS );
        REQUIRE( DS.count() == 5 );

        WHEN("Poistetaan makeista 'SUK:8787:324' 130 kappaletta (liikaa)")
        {
            redirector.start();
            DS.substract("SUK:8787:324", 130);
            result = redirector.stop();

            THEN("Tulostetaan EI_VARASTOA ja varastosaldo")
            {
                std::stringstream ei_varastoa;
                ei_varastoa << EI_VARASTOA << std::endl << "Saldo: 120 Hylly: 93.93";

                REQUIRE( result == ei_varastoa.str());

                AND_THEN("Tietorakenteessa on 5 eri makeistyyppiä")
                {
                    REQUIRE( DS.count() == 5);
                }
                AND_WHEN("Haetaan ID:llä 'SUK:8787:324'")
                {
                    redirector.start();
                    DS.find("SUK:8787:324");
                    result = redirector.stop();
                    THEN("Tulostetaan tuotteen varastosaldo")
                    {
                        REQUIRE( result == "Fazer Maitosuklaa 120 93.93");
                    }
                }
            }
        }
    }
}

SCENARIO( "Vähennetään tuntematonta makeista", "[D]" )
{


    IORedirector redirector;
    std::string result;

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );
        ReadFile( TESTDIR + "varasto1.txt", DS );
        REQUIRE( DS.count() == 5 );

        WHEN("Poistetaan tuntematonta makeista 'SUK:8787:322' 10 kappaletta")
        {
            redirector.start();
            DS.substract("SUK:8787:322", 10);
            result = redirector.stop();

            THEN("Tulostetaan EI_SAATAVILLA")
            {
                REQUIRE( result == EI_SAATAVILLA);
                AND_THEN("Tietorakenteessa on 5 eri makeistyyppiä")
                {
                    REQUIRE( DS.count() == 5);
                }
            }
        }
    }
}

SCENARIO( "Vähennetään makeisen koko varastosaldo", "[D]" )
{


    IORedirector redirector;
    std::string result;

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );
        ReadFile( TESTDIR + "varasto1.txt", DS );
        REQUIRE( DS.count() == 5 );

        WHEN("Poistetaan makeista 'SUK:8787:324' 120 kappaletta (kaikki)")
        {
            redirector.start();
            DS.substract("SUK:8787:324", 120);
            result = redirector.stop();

            THEN("Tulostetaan varastosaldo")
            {
                REQUIRE( result == "Saldo: 0 Hylly: 93.93");
            }
            AND_THEN("Tietorakenteessa on 4 eri makeistyyppiä")
            {
                REQUIRE( DS.count() == 4);
            }
            AND_WHEN("Haetaan ID:llä 'SUK:8787:324'")
            {
                redirector.start();
                DS.find("SUK:8787:324");
                result = redirector.stop();
                THEN("Tulostetaan EI_SAATAVILLA")
                {
                    REQUIRE( result == EI_SAATAVILLA);
                    AND_THEN("Tietorakenteessa on 4 eri makeistyyppiä")
                    {
                        REQUIRE( DS.count() == 4);
                    }
                }
            }
        }
    }
}

SCENARIO( "Poistetaan, haetaan ja lisätään makeisia", "[D][A][F]" )
{


    IORedirector redirector;
    std::string result;

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );
        ReadFile( TESTDIR + "varasto1.txt", DS );
        REQUIRE( DS.count() == 5 );

        WHEN("Poistetaan makeista 'SUK:8787:324' 120 kappaletta (kaikki)")
        {
            redirector.start();
            DS.substract("SUK:8787:324", 120);
            result = redirector.stop();

            THEN("Tulostetaan varastosaldo")
            {
                REQUIRE( result == "Saldo: 0 Hylly: 93.93");
            }
            AND_THEN("Tietorakenteessa on 4 eri makeistyyppiä")
            {
                REQUIRE( DS.count() == 4);
            }
            AND_WHEN("Haetaan ID:llä 'SUK:8787:324'")
            {
                redirector.start();
                DS.find("SUK:8787:324");
                result = redirector.stop();
                THEN("Tulostetaan EI_SAATAVILLA")
                {
                    REQUIRE( result == EI_SAATAVILLA);
                    AND_THEN("Tietorakenteessa on 4 eri makeistyyppiä")
                    {
                        REQUIRE( DS.count() == 4);
                    }
                }
            }
            AND_WHEN("Lisätään makeinen 'SUK:8787:324 41.1 11 Fazer Maitosuklaa2'")
            {
                DS.add("SUK:8787:324", "41.1", 11, "Fazer Maitosuklaa2");
                THEN("Tietorakenteessa on 5 eri makeistyyppiä")
                {
                    REQUIRE( DS.count() == 5);
                }
                AND_WHEN("Haetaan ID:llä 'SUK:8787:324'")
                {
                    redirector.start();
                    DS.find("SUK:8787:324");
                    result = redirector.stop();
                    THEN("Tulostetaan tuotteen varastosaldo")
                    {
                        REQUIRE( result == "Fazer Maitosuklaa2 11 41.1");
                    }
                }
                AND_WHEN( "Tyhjennetään")
                {
                    DS.empty();
                    THEN( "Tietorakenteessa on 0 eri makeistyyppiä")
                    {
                        REQUIRE( DS.count() == 0 );
                    }
                    AND_WHEN("Lisätään makeinen 'SUK:8787:324 41.1 11 Fazer Maitosuklaa3'")
                    {
                        DS.add("SUK:8787:324", "11.1", 55, "Fazer Maitosuklaa3");
                        THEN("Tietorakenteessa on 1 eri makeistyyppiä")
                        {
                            REQUIRE( DS.count() == 1);
                        }
                        AND_WHEN("Haetaan ID:llä 'SUK:8787:324'")
                        {
                            redirector.start();
                            DS.find("SUK:8787:324");
                            result = redirector.stop();
                            THEN("Tulostetaan tuotteen varastosaldo")
                            {
                                REQUIRE( result == "Fazer Maitosuklaa3 55 11.1");
                            }
                        }
                    }
                }
            }
        }
    }
}

SCENARIO( "Tyhjältä tietorakenteelta määrän kysyminen", "[C]" )
{


    GIVEN("Tyhjä tietorakenne")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        WHEN("kysytään erilaisten määrä")
        {
            THEN("Tietorakenteessa on 0 eri makeistyyppiä")
            {
                REQUIRE( DS.count() == 0 );
            }
        }
    }
}

SCENARIO( "Haetaan tuotteen varastosaldo", "[F]" )
{


    IORedirector redirector;
    std::string result;

    GIVEN("Luetaan alkutilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );
        ReadFile( TESTDIR + "varasto1.txt", DS );
        REQUIRE( DS.count() == 5 );

        WHEN("Haetaan ID:llä LKS:1234:123")
        {
            redirector.start();
            DS.find("LKS:1234:123");
            result = redirector.stop();
            THEN("Tulostetaan tuotteen varastosaldo")
            {
                REQUIRE( result == "Panda Lakupala 100 42.2");
            }
        }
    }
}

SCENARIO( "Haetaan tuntemattoman tuotteen varastosaldo", "[F]" )
{


    IORedirector redirector;
    std::string result;

    GIVEN("Luetaan alkutilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        Datastructure DS;
        REQUIRE( DS.count() == 0 );
        ReadFile( TESTDIR + "varasto1.txt", DS );
        REQUIRE( DS.count() == 5 );

        WHEN("Haetaan ID:llä LKS:1235:123")
        {
            redirector.start();
            DS.find("LKS:1235:123");
            result = redirector.stop();

            THEN("Tulostetaan ei varastossa")
            {
                REQUIRE(result == EI_SAATAVILLA);
            }
        }
    }
}

void small_file_test1(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );
    WHEN("Halutaan tietää eri makeistyyppien määrä")
    {
        THEN("Tietorakenteessa on " + std::to_string( item_count) + " eri makeistyyppiä")
        {
            REQUIRE( DS.count() == item_count );
        }
    }
}
void small_file_test2(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );
    REQUIRE( DS.count() == item_count);

    WHEN( "Tyhjennetään")
    {
        DS.empty();
        THEN( "Tietorakenteessa on 0 eri makeistyyppiä")
        {
            REQUIRE( DS.count() == 0 );
        }
        AND_WHEN("Haetaan jokaista tiedostossa olevaa ID:tä yksi kerrallaan")
        {
            std::string line;
            std::string ID;
            std::string loc;
            unsigned int amnt;
            std::string name;
            std::string expected_result;
            std::ifstream file_stream( file );
            std::vector<std::vector<std::string> > results;
            while( std::getline( file_stream, line ))
            {
                std::istringstream line_stream( line );
                getline(line_stream, ID, ';');
                line_stream >> amnt;
                line_stream.get();
                getline(line_stream, loc, ';');
                getline(line_stream, name);
                redirector.start();
                DS.find(ID);
                result = redirector.stop();
                AND_WHEN("Haetaan ID:tä '" + ID + "'")
                {
                    THEN("Tulostetaan EI_SAATAVILLA")
                    {
                        REQUIRE(result == EI_SAATAVILLA);
                    }
                }
            }
            file_stream.close();
        }
    }
}

void small_file_test3(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );
    WHEN("Haetaan jokaista tiedostossa olevaa ID:tä yksi kerrallaan")
    {

        REQUIRE( DS.count() == item_count );
        std::string line;
        std::string ID;
        std::string loc;
        unsigned int amnt;
        std::string name;
        std::string expected_result;
        std::ifstream file_stream( file );
        std::vector<std::vector<std::string> > results;
        while( std::getline( file_stream, line ))
        {
            std::istringstream line_stream( line );
            getline(line_stream, ID, ';');
            line_stream >> amnt;
            line_stream.get();
            getline(line_stream, loc, ';');
            getline(line_stream, name);
            redirector.start();
            DS.find(ID);
            result = redirector.stop();
            expected_result = name + " " + std::to_string(amnt) + " " + loc;
            AND_WHEN("Haetaan ID:tä '" + ID + "'")
            {
                THEN("Tulostetaan ID:n varastosaldo")
                {
                    REQUIRE(result == expected_result);
                }
            }
        }
        file_stream.close();
    }
}

void small_file_test4(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );

    WHEN("Poistetaan jokaiselta tuotekoodilta yhtä lukuunottamatta koko varastosaldo")
    {
        REQUIRE( DS.count() == item_count );
        Datastructure DS;
        REQUIRE( DS.count() == 0 );
        ReadFile( file, DS );
        REQUIRE( DS.count() == item_count );
        std::string line;
        std::string ID;
        std::string loc;
        unsigned int amnt;
        std::string name;
        std::string expected_result;
        std::ifstream file_stream( file );
        std::vector<std::vector<std::string> > results;

        while( std::getline( file_stream, line ))
        {
            std::istringstream line_stream( line );
            getline(line_stream, ID, ';');
            line_stream >> amnt;
            line_stream.get();
            getline(line_stream, loc, ';');
            getline(line_stream, name);
            redirector.start();
            DS.substract(ID, amnt - 1);
            result = redirector.stop();

            expected_result = "Saldo: 1 Hylly: " + loc;
            AND_WHEN("Poistetaan ID:ltä '" + ID + "' " + std::to_string(amnt) + " makeista")
            {
                THEN("Tulostetaan ID:n jäljellä oleva varastosaldo ja hyllypaikka")
                {
                    REQUIRE(result == expected_result);
                }
            }
        }
        file_stream.close();
    }
}

void small_file_test5(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );
    REQUIRE( DS.count() == item_count);

    WHEN("Poistetaan jokaiselta tuotekooldita enemmän kuin varastosaldoa on.")
    {
        std::string line;
        std::string ID;
        std::string loc;
        int amnt;
        std::string name;
        std::string expected_result;
        std::ifstream file_stream( file );
        std::vector<std::vector<std::string> > results;
        while( std::getline( file_stream, line ))
        {
            std::istringstream line_stream( line );
            getline(line_stream, ID, ';');
            line_stream >> amnt;
            line_stream.get();
            getline(line_stream, loc, ';');
            getline(line_stream, name);
            redirector.start();
            DS.substract(ID, amnt + 1);
            result = redirector.stop();
            expected_result = "Saldo: " + std::to_string(amnt) + " Hylly: " + loc;
            std::stringstream ei_varastoa;
            ei_varastoa << EI_VARASTOA << std::endl << expected_result;

            expected_result = ei_varastoa.str();
            AND_WHEN("Poistetaan ID:ltä '" + ID + "' " + std::to_string(amnt) + " makeista")
            {
                THEN("Tulostetaan EI_VARASTOA ja varastosaldo")
                {
                    REQUIRE(result == expected_result);
                }
            }
        }
        file_stream.close();
    }
}

void small_file_test6(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );

    WHEN("Poistetaan jokaiselta tuotekoodilta koko varastosaldo.")
    {
        REQUIRE( DS.count() == item_count );
        std::string line;
        std::string ID;
        std::string loc;
        int amnt;
        std::string name;
        std::string expected_result;
        std::ifstream file_stream( file );
        std::vector<std::vector<std::string> > results;
        while( std::getline( file_stream, line ))
        {
            std::istringstream line_stream( line );
            getline(line_stream, ID, ';');
            line_stream >> amnt;
            line_stream.get();
            getline(line_stream, loc, ';');
            getline(line_stream, name);
            redirector.start();
            DS.substract(ID, amnt);
            result = redirector.stop();
            expected_result = "Saldo: 0 Hylly: " + loc;
            AND_WHEN("Poistetaan ID:ltä '" + ID + "' " + std::to_string(amnt) + " makeista")
            {
                THEN("Tulostetaan ID:n jäljellä oleva varastosaldo ja hyllypaikka")
                {
                    REQUIRE(result == expected_result);
                }
            }
        }
        file_stream.close();
    }
}

void small_file_test7(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );

    WHEN("Poistetaan jokaiselta tuotekoodilta koko varastosaldo.")
    {
        REQUIRE( DS.count() == item_count );
        std::string line;
        std::string ID;
        std::string loc;
        int amnt;
        std::string name;
        std::string expected_result;
        std::ifstream file_stream( file );
        std::vector<std::vector<std::string> > results;
        while( std::getline( file_stream, line ))
        {
            std::istringstream line_stream( line );
            getline(line_stream, ID, ';');
            line_stream >> amnt;
            line_stream.get();
            getline(line_stream, loc, ';');
            getline(line_stream, name);
            redirector.start();
            DS.substract(ID, amnt);
            result = redirector.stop();
            expected_result = "Saldo: 0 Hylly: " + loc;
            AND_WHEN("Poistetaan ID:ltä '" + ID + "' " + std::to_string(amnt) + " makeista")
            {
                THEN("Tulostetaan ID:n jäljellä oleva varastosaldo ja hyllypaikka")
                {
                    REQUIRE(result == expected_result);
                }
            }
        }
        file_stream.close();
        AND_WHEN("Luetaan varastotilanne uudestaan")
        {
            ReadFile(file, DS);
            THEN("Lukumäärä vastaa odotettua")
            {
                REQUIRE(DS.count() == item_count );
            }
            AND_WHEN("Haetaan jokaista tiedostossa olevaa ID:t yksi kerrallaan")
            {
                std::string line;
                std::string ID;
                std::string loc;
                unsigned int amnt;
                std::string name;
                std::string expected_result;
                std::ifstream file_stream( file );
                std::vector<std::vector<std::string> > results;
                while( std::getline( file_stream, line ))
                {
                    std::istringstream line_stream( line );
                    getline(line_stream, ID, ';');
                    line_stream >> amnt;
                    line_stream.get();
                    getline(line_stream, loc, ';');
                    getline(line_stream, name);
                    redirector.start();
                    DS.find(ID);
                    result = redirector.stop();
                    expected_result = name + " " + std::to_string(amnt) + " " + loc;
                    AND_WHEN("Haetaan ID:tä '" + ID + "'")
                    {
                        THEN("Tulostetaan ID:n varastosaldo")
                        {
                            REQUIRE(result == expected_result);
                        }
                    }
                }
                file_stream.close();
            }
        }
    }
}


void big_file_test1(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );
    REQUIRE( DS.count() == item_count );

    WHEN("Halutaan tietää eri makeistyyppien määrä")
    {
        THEN("Tietorakenteessa on " + std::to_string(item_count) + " eri makeistyyppiä")
        {
            REQUIRE( DS.count() == item_count );
        }
    }
}
void big_file_test2(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );
    REQUIRE( DS.count() == item_count );
    WHEN( "Tyhjennetään")
    {
        DS.empty();
        THEN( "Tietorakenteessa on 0 eri makeistyyppiä")
        {
            REQUIRE( DS.count() == 0 );
        }
        AND_WHEN("Haetaan jokaista tiedostossa olevaa ID:t yksi kerrallaan")
        {
            std::string line;
            std::string ID;
            std::string loc;
            unsigned int amnt;
            std::string name;
            std::string expected_result;
            std::ifstream file_stream( file );
            std::vector<std::vector<std::string> > results;
            while( std::getline( file_stream, line ))
            {
                std::istringstream line_stream( line );
                getline(line_stream, ID, ';');
                line_stream >> amnt;
                line_stream.get();
                getline(line_stream, loc, ';');
                getline(line_stream, name);
                redirector.start();
                DS.find(ID);
                result = redirector.stop();
                std::vector<std::string> tmp { ID, result, EI_SAATAVILLA};
                results.push_back(tmp);
            }
            file_stream.close();
            // TODO: Lisää ID virheilmoitukseen
            THEN("Tulostetaan EI_SAATAVILLA")
            {
                for( auto result_vector: results )
                {
                    ID = result_vector.at(0);
                    result = result_vector.at(1);
                    expected_result = result_vector.at(2);
                    REQUIRE(result == expected_result);
                }
            }

        }
    }
}
void big_file_test3(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );
    REQUIRE( DS.count() == item_count );
    WHEN("Haetaan jokaista tiedostossa olevaa ID:tä yksi kerrallaan")
    {

        REQUIRE( DS.count() == item_count );
        std::string line;
        std::string ID;
        std::string loc;
        unsigned int amnt;
        std::string name;
        std::string expected_result;
        std::ifstream file_stream( file );
        std::vector<std::vector<std::string> > results;
        while( std::getline( file_stream, line ))
        {
            std::istringstream line_stream( line );
            getline(line_stream, ID, ';');
            line_stream >> amnt;
            line_stream.get();
            getline(line_stream, loc, ';');
            getline(line_stream, name);
            redirector.start();
            DS.find(ID);
            result = redirector.stop();
            expected_result = name + " " + std::to_string(amnt) + " " + loc;
            std::vector<std::string> tmp { ID, result, expected_result};
            results.push_back(tmp);
        }
        file_stream.close();
        // TODO: Lisää ID virheilmoitukseen
        THEN("Haetut tiedot vastaavat tiedostossa olevaa")
        {
            for( auto result_vector: results )
            {
                ID = result_vector.at(0);
                result = result_vector.at(1);
                expected_result = result_vector.at(2);
                REQUIRE(result == expected_result);
            }
        }
    }
}
void big_file_test4(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );
    REQUIRE( DS.count() == item_count );

    WHEN("Poistetaan jokaiselta tuotekoodilta yhtä lukuunottamatta koko varastosaldo")
    {
        REQUIRE( DS.count() == item_count );
        Datastructure DS;
        REQUIRE( DS.count() == 0 );
        ReadFile( file, DS );
        REQUIRE( DS.count() == item_count );
        std::string line;
        std::string ID;
        std::string loc;
        unsigned int amnt;
        std::string name;
        std::string expected_result;
        std::ifstream file_stream( file );
        std::vector<std::vector<std::string> > results;

        while( std::getline( file_stream, line ))
        {
            std::istringstream line_stream( line );
            getline(line_stream, ID, ';');
            line_stream >> amnt;
            line_stream.get();
            getline(line_stream, loc, ';');
            getline(line_stream, name);
            redirector.start();
            DS.substract(ID, amnt - 1);
            result = redirector.stop();

            expected_result = "Saldo: 1 Hylly: " + loc;
            std::vector<std::string> tmp { ID, result, expected_result};
            results.push_back(tmp);
        }
        file_stream.close();
        // TODO: Lisää ID virheilmoitukseen
        // TODO: Parempi kuvaus
        THEN("Jokaisen poiston yhteydessä tulostetaan 'Saldo: 1 Hylly: x.y'")
        {
            for( auto result_vector: results )
            {
                ID = result_vector.at(0);
                result = result_vector.at(1);
                expected_result = result_vector.at(2);
                REQUIRE(result == expected_result);
            }
        }
    }
}
void big_file_test5(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );
    REQUIRE( DS.count() == item_count );

    WHEN("Poistetaan jokaiselta tuotekoodilta enemmän kuin varastosaldoa on.")
    {
        std::string line;
        std::string ID;
        std::string loc;
        int amnt;
        std::string name;
        std::string expected_result;
        std::ifstream file_stream( file );
        std::vector<std::vector<std::string> > results;
        while( std::getline( file_stream, line ))
        {
            std::istringstream line_stream( line );
            getline(line_stream, ID, ';');
            line_stream >> amnt;
            line_stream.get();
            getline(line_stream, loc, ';');
            getline(line_stream, name);
            redirector.start();
            DS.substract(ID, amnt + 1);
            result = redirector.stop();
            expected_result = "Saldo: " + std::to_string(amnt) + " Hylly: " + loc;
            std::stringstream ei_varastoa;
            ei_varastoa << EI_VARASTOA << std::endl << expected_result;
            std::vector<std::string> tmp { ID, result, ei_varastoa.str()};
            results.push_back(tmp);
        }
        file_stream.close();
        // TODO: Lisää ID virheilmoitukseen
        // TODO: Parempi kuvaus
        THEN("Jokaisen poiston yhteydessä tulostetaan EI_VARASTOA")
        {
            for( auto result_vector: results )
            {
                ID = result_vector.at(0);
                result = result_vector.at(1);
                expected_result = result_vector.at(2);
                REQUIRE(result == expected_result);
            }
        }
    }
}
void big_file_test6(const string& file, unsigned int item_count)
{
    IORedirector redirector;
    std::string result;
    Datastructure DS;
    REQUIRE( DS.count() == 0 );
    ReadFile( file, DS );
    REQUIRE( DS.count() == item_count );

    WHEN("Poistetaan jokaiselta tuotekoodilta koko varastosaldo.")
    {
        REQUIRE( DS.count() == item_count );
        std::string line;
        std::string ID;
        std::string loc;
        int amnt;
        std::string name;
        std::string expected_result;
        std::ifstream file_stream( file );
        std::vector<std::vector<std::string> > results;
        while( std::getline( file_stream, line ))
        {
            std::istringstream line_stream( line );
            getline(line_stream, ID, ';');
            line_stream >> amnt;
            line_stream.get();
            getline(line_stream, loc, ';');
            getline(line_stream, name);
            redirector.start();
            DS.substract(ID, amnt);
            result = redirector.stop();
            expected_result = "Saldo: 0 Hylly: " + loc;
            std::vector<std::string> tmp { ID, result, expected_result};
            results.push_back(tmp);
        }
        file_stream.close();
        // TODO: Lisää ID virheilmoitukseen
        // TODO: Parempi kuvaus
        THEN("Jokaisen poiston yhteydessä tulostetaan 'Saldo: 0 Hylly: x.y'")
        {
            for( auto result_vector: results )
            {
                ID = result_vector.at(0);
                result = result_vector.at(1);
                expected_result = result_vector.at(2);
                REQUIRE(result == expected_result);
            }
            AND_WHEN("Haetaan jokaista tiedostossa olevaa ID:tä yksi kerrallaan")
            {
                std::ifstream file_stream( file );
                results.clear();
                while( std::getline( file_stream, line ))
                {
                    std::istringstream line_stream( line );
                    getline(line_stream, ID, ';');
                    line_stream >> amnt;
                    line_stream.get();
                    getline(line_stream, loc, ';');
                    getline(line_stream, name);
                    redirector.start();
                    DS.find(ID);
                    result = redirector.stop();
                    std::vector<std::string> tmp { ID, result, EI_SAATAVILLA };
                    results.push_back(tmp);
                }
                file_stream.close();
                // TODO: Lisää ID virheilmoitukseen
                THEN("Jokaisen haun yhteydessä tulostetaan EI_SAATAVILLA")
                {
                    for( auto result_vector: results )
                    {
                        ID = result_vector.at(0);
                        result = result_vector.at(1);
                        expected_result = result_vector.at(2);
                        REQUIRE(result == expected_result);
                    }
                }
            }
        }
    }
}


SCENARIO( "Iso määrä eri tuoteID:eitä, vain yksi makeisryhmä ja valmistaja. Makeistyyppien määrä.", "[ISO][BT1]" )
{


    GIVEN("Luetaan varastotilanne varasto_iso1.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test1(TESTDIR + "varasto_iso1.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä, vain yksi makeisryhmä ja valmistaja. Tyhjennetään.", "[ISO][BT2]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso1.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test2(TESTDIR + "varasto_iso1.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä, vain yksi makeisryhmä ja valmistaja. Haetaan.", "[ISO][BT3]" )
{


    GIVEN("Luetaan varastotilanne varasto_iso1.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test3(TESTDIR + "varasto_iso1.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä, vain yksi makeisryhmä ja valmistaja. Poistetaan melkein koko varastosaldo.", "[ISO][BT4]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso1.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test4(TESTDIR + "varasto_iso1.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä, vain yksi makeisryhmä ja valmistaja. Poistetaan enemmän kuin varastosaldoa on.", "[ISO][BT5]" )
{


    GIVEN("Luetaan varastotilanne varasto_iso1.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test5(TESTDIR + "varasto_iso1.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä, vain yksi makeisryhmä ja valmistaja. Poistetaan koko varastosaldo.", "[ISO][BT6]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso1.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test6(TESTDIR + "varasto_iso1.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja valmistajia, vain yksi makeisryhmä. Makeistyyppien määrä.", "[ISO][BT1]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso2.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test1(TESTDIR + "varasto_iso2.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja valmistajia, vain yksi makeisryhmä. Tyhjennetään.", "[ISO][BT2]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso2.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test2(TESTDIR + "varasto_iso2.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja valmistajia, vain yksi makeisryhmä. Haetaan.", "[ISO][BT3]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso2.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test3(TESTDIR + "varasto_iso2.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja valmistajia, vain yksi makeisryhmä. Poistetaan melkein koko varastosaldo.", "[ISO][BT4]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso2.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test4(TESTDIR + "varasto_iso2.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja valmistajia, vain yksi makeisryhmä. Poistetaan enemmän kuin varastosaldoa on.", "[ISO][BT5]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso2.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test5(TESTDIR + "varasto_iso2.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja valmistajia, vain yksi makeisryhmä. Poistetaan koko varastosaldo.", "[ISO][BT6]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso2.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test6(TESTDIR + "varasto_iso2.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja makeisryhmiä, vain yksi valmistaja. Makeistyyppien määrä.", "[ISO][BT1]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso3.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test1(TESTDIR + "varasto_iso3.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja makeisryhmiä, vain yksi valmistaja. Tyhjennetään.", "[ISO][BT2]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso3.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test2(TESTDIR + "varasto_iso3.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja makeisryhmiä, vain yksi valmistaja. Haetaan.", "[ISO][BT3]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso3.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test3(TESTDIR + "varasto_iso3.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja makeisryhmiä, vain yksi valmistaja. Poistetaan melkein koko varastosaldo.", "[ISO][BT4]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso3.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test4(TESTDIR + "varasto_iso3.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja makeisryhmiä, vain yksi valmistaja. Poistetaan enemmän kuin varastosaldoa on.", "[ISO][BT5]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso3.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test5(TESTDIR + "varasto_iso3.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:eitä ja makeisryhmiä, vain yksi valmistaja. Poistetaan koko varastosaldo.", "[ISO][BT6]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso3.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test6(TESTDIR + "varasto_iso3.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Makeistyyppien määrä", "[ISO][BT1]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso4.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test1(TESTDIR + "varasto_iso4.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Tyhjennetään.", "[ISO][BT2]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso4.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test2(TESTDIR + "varasto_iso4.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Haetaan.", "[ISO][BT3]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso4.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test2(TESTDIR + "varasto_iso4.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Poistetaan melkein koko varastosaldo.", "[ISO][BT4]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso4.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test4(TESTDIR + "varasto_iso4.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Poistetaan enemmän kuin varastosaldoa on.", "[ISO][BT5]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso4.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test5(TESTDIR + "varasto_iso4.txt", 900);
    }
}

SCENARIO( "Iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Poistetaan koko varastosaldo.", "[ISO][BT6]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso4.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test6(TESTDIR + "varasto_iso4.txt", 900);
    }
}


SCENARIO( "Iso määrä eri valmistajia ja makeisryhmiä, vain yksi tuoteID. Makeistyyppien määrä.", "[ISO][BT1]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso5.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test1(TESTDIR + "varasto_iso5.txt", 900);
    }
}

SCENARIO( "Iso määrä eri valmistajia ja makeisryhmiä, vain yksi tuoteID. Tyhjennetään.", "[ISO][BT2]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso5.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test2(TESTDIR + "varasto_iso5.txt", 900);
    }
}

SCENARIO( "Iso määrä eri valmistajia ja makeisryhmiä, vain yksi tuoteID. Haetaan.", "[ISO][BT3]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso5.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test3(TESTDIR + "varasto_iso5.txt", 900);
    }
}

SCENARIO( "Iso määrä eri valmistajia ja makeisryhmiä, vain yksi tuoteID. Poistetaan melkein koko varastosaldo.", "[ISO][BT4]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso5.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test4(TESTDIR + "varasto_iso5.txt", 900);
    }
}

SCENARIO( "Iso määrä eri valmistajia ja makeisryhmiä, vain yksi tuoteID. Poistetaan enemmän kuin varastosaldoa on.", "[ISO][BT5]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso5.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test5(TESTDIR + "varasto_iso5.txt", 900);
    }
}

SCENARIO( "Iso määrä eri valmistajia ja makeisryhmiä, vain yksi tuoteID. Poistetaan koko varastosaldo.", "[ISO][BT6]" )
{

    GIVEN("Luetaan varastotilanne varasto_iso5.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test6(TESTDIR + "varasto_iso5.txt", 900);
    }
}


SCENARIO( "Todella iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Makeistyyppien määrä.", "[TOSIISO][BBT1]" )
{

    GIVEN("Luetaan varastotilanne varasto_tosi_iso.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test1(TESTDIR + "varasto_tosi_iso.txt", 1500);
    }
}

SCENARIO( "Todella iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Tyhjennetään.", "[TOSIISO][BBT2]" )
{

    GIVEN("Luetaan varastotilanne varasto_tosi_iso.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test2(TESTDIR + "varasto_tosi_iso.txt", 1500);
    }
}

SCENARIO( "Todella iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Haetaan.", "[TOSIISO][BBT3]" )
{

    GIVEN("Luetaan varastotilanne varasto_tosi_iso.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test3(TESTDIR + "varasto_tosi_iso.txt", 1500);
    }
}

SCENARIO( "Todella iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Poistetaan melkein koko varastosaldo.", "[TOSIISO][BBT4]" )
{

    GIVEN("Luetaan varastotilanne varasto_tosi_iso.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test4(TESTDIR + "varasto_tosi_iso.txt", 1500);
    }
}

SCENARIO( "Todella iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Poistetaan enemmän kuin varastosaldoa on.", "[TOSIISO][BBT5]" )
{

    GIVEN("Luetaan varastotilanne varasto_tosi_iso.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test5(TESTDIR + "varasto_tosi_iso.txt", 1500);
    }
}

SCENARIO( "Todella iso määrä eri tuoteID:itä, makeisryhmiä ja valmistajia. Poistetaan koko varastosaldo.", "[TOSIISO][BBT6]" )
{

    GIVEN("Luetaan varastotilanne varasto_tosi_iso.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test6(TESTDIR + "varasto_tosi_iso.txt", 1500);
    }
}



SCENARIO( "Pieni varasto. Makeistyyppien määrä.", "[PIENI][ST1]" )
{

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        small_file_test1(TESTDIR + "varasto1.txt", 5);
    }
}

SCENARIO( "Pieni varasto. Tyhjennetään.", "[PIENI][ST2]" )
{

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        small_file_test2(TESTDIR + "varasto1.txt", 5);
    }
}

SCENARIO( "Pieni varasto. Haetaan.", "[PIENI][ST3]" )
{

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        small_file_test3(TESTDIR + "varasto1.txt", 5);
    }
}

SCENARIO( "Pieni varasto. Poistetaan melkein koko varastosaldo.", "[PIENI][ST4]" )
{

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        small_file_test4(TESTDIR + "varasto1.txt", 5);
    }
}

SCENARIO( "Pieni varasto. Poistetaan enemmän kuin varastosaldoa on.", "[PIENI][ST5]" )
{

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        small_file_test5(TESTDIR + "varasto1.txt", 5);
    }
}

SCENARIO( "Pieni varasto. Poistetaan koko varastosaldo.", "[PIENI][ST6]" )
{

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        small_file_test6(TESTDIR + "varasto1.txt", 5);
    }
}

SCENARIO( "Pieni varasto. Poistetaan koko varastosaldo, lisätään uudestaan ja tarkistetaan.", "[PIENI][ST7]" )
{

    GIVEN("Luetaan varastotilanne varasto1.txt")
    {
        UNITTEST_THROW_SIGNALS
        small_file_test7(TESTDIR + "varasto1.txt", 5);
    }
}

SCENARIO("Testataan muistinkulutusta", "[MUISTI][BT1]" )
{

    GIVEN("Luetaan varastotilanne varasto_muisti.txt")
    {
        UNITTEST_THROW_SIGNALS
        big_file_test1(TESTDIR + "varasto_muisti.txt", 9007);
    }
}
