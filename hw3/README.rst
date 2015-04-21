Kääntäminen
===========

Harjoitustyö käyttää Qmakea ohjelman kääntämiseen. Qt Creatorissa projektin
käyttämiseen riittää projektitiedosto  **hw3_nysse.pro**:n avaaminen.

Komentoriviltä ohjelma voidaan kääntää qmake:n avulla. Lintulassa tarvittava
qmake:n versio löytyy nimellä **qmake-qt5**. Muissa ympäristöissä **qmake** voi
löytyä nimellä **qmake** ::

  cd hw3
  qmake-qt5
  make

Tämän jälkeen käännetty pääohjelma löytyy hakemistosta *app* nimellä *app*,
joka voidaan ajaa seuraavasti::

  app/app

Käännetty testiohjelma puolestaan löytyy hakemistosta *tests* nimellä *tests*,
joka voidaan ajaa seuraavasti::

  tests/tests

Kääntämisongelma Lintulassa
---------------------------

Lintulassa *qmake* nimellä oleva ohjelma on liian vanha versio
*qmake*:sta. Käytä *qmake-qt5* ohjelmaa. Jos saat seuraavan tyylisiä
varoituksia, niin olet luultavasti käyttänyt väärää versiota. Voit korjata
ongelman poistamalla kaikki *qmake*:n luomat *Makefile* tiedostot.

::

  main.cc:7:1: error: stray ‘\’ in program
   const std::string TESTDIR=std::string(TESTMATERIALDIR);


Testiajoista
============

Oletuksena testiohjelma ajaa kaikki testit ja tulostaa vain niiden testien
tiedot, jotka eivät menneet läpi.

Testeille on annettu tageja, joten niitä pystyy ajamaan myös yksi
kerrallaan. Esimerkiksi jos halutaan ajaa testi numero **31**, voidaan
komentoriviltä ajaa testiohjelma seuraavasti::

  tests/tests [31]

Testiohjelman saa tulostamaan kaikkien testien tiedot antamalla
komentoriviparametrin **-s**

::

  tests/tests -s

On myös mahdollista, että ohjelmasi ei läpäise testiä, koska se löytää joko
nopeamman reitin kohteeseen tai käyttää eri reittiä kuin testissä, mutta on
kuitenkin samaan aikaan perillä kohteessa. Tällöin kannattaa lukea testin
kuvaus läpi ja erityisesti kiinnittää huomiota mahdollisiin tehtävänannossa
annettuihin rajoituksiin. Jos olet tämänkin jälkeen varma, että ohjelmasi
toimii oikein, niin lähetä viestiä kurssihenkilökunnalle (tiraka@cs.tut.fi).

Parametrien anto testiohjelmalle Qt Creatorilla
-----------------------------------------------

- Valitse vasemmasta laidasta **Projects**.
- Valitse ylälaidasta *Desktop:n* alta **Run**
- Valitse *Run configuration:* **tests**
- Lisää *Arguments:* kohtaan haluamasi parametrit

Muistirajoitus
==============

Ohjelman muistinkäyttö on Linuxissa (jossa myös automaattitestit ajetaan)
rajoitettu 128 megaan. Valitettavasti Windowsissa muistinkäyttöä ei voida
rajoittaa, joten Windows käyttäjien omalla vastuulla on huolehtia, ettei
muistinkäyttö ylity ja palautettaessa tule ikävää yllätystä
automaattitesteistä.
