#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <stdlib.h>

using namespace ::std;
class Odbijacz;
class Kula;
//---------------------------------------------------------PLANSZA----
class Plansza
{
private:
    int k, n, m;               // n wiersz, m kolumna, k liczba tur

public:
    vector<Kula *> tabk;       // wektor na kulki
    vector<Odbijacz *> tabo;   // wektor na odbijacze
    void sprawdz_poz_kul();    // sprawdza czy kulki sa na krawedziach, albo czy przeszły przez zjadajacy
    void sprawdz_usun();       // sprawdza czy jakis odbjacz zostal zniszczony
    void egz_ceche_kul(int c); // egzekwuje cechę kulek
    void egz_ceche_odb();      // egzekwuje cechę odbijaczy
    void zderzenie();          // obsluguje zderzenia kulek
    void przesun();            // przesuwa kulki
    void wypisz();             // wypisuje wspolrzedne
    int getk();
    int getn();
    int getm();
    void setk(int c);
    int getr();

    // Techniczne
    Plansza(int k, int n, int m, vector<Kula *> tabk, vector<Odbijacz *> tabo);
    Plansza(const Plansza &);
    Plansza &operator=(const Plansza &);
    ~Plansza();
}; //---------------------------------------------------------PLANSZA----

//-------------------------------------------------------KULA
class Kula
{
private:
    double p, r, q, a, b, x, y;                 // p predkosc, (r,q)-punkt startowy, (x,y) położenie, [a,b]-wektor ruchu przyjmujący wartości np. [-1,0] - w lewo, [1,1] - gorny lewy skos
public:
    void wypisz();                              // wypisuje wsporzedne
    void przesun();                             // przesuwa kulke
    bool czy_na_kul(Kula &k);                   // sprawdza czy jest na tym samym polu co inna kulka
    bool czy_na_odb(Odbijacz &);                // sprawdza czy jest na odbijaczu
    void zderzenie(Kula &);                     // obsluguje zderzenie kulek (jak bedzie wiecej niz dwie to wszytsko jest OK, ponieważ jest to wypadkowa sił, a pętla for sprawdza kulki po kolei)
    virtual void cecha(Odbijacz &o, int k) = 0; // funkcja wirtualna, która opisuje typ kulek, k obecna liczba rund, l poczatkowa liczba rund
    void setr(double c);
    void setq(double c);
    void setx(double c);
    void sety(double c);
    void seta(double c);
    void setb(double c);
    void setp(double c);
    double getr() const;
    double getq() const;
    double getx() const;
    double gety() const;
    double geta() const;
    double getb() const;
    double getp() const;

    // techniczne
    Kula(double, double, double, double, double, double, double);
    Kula(const Kula &) = default;
    Kula() = delete;
    virtual ~Kula()=default;
    Kula &operator=(const Kula &) = default;
}; //---------------------------------------------------------KULA----

//---------------------------------------------------------ODBIJACZ----
class Odbijacz
{
private:
    double x, y; // (x,y) połozenie na planszy
public:
    double getx() const;
    double gety() const;
    void setx(double c);
    void sety(double c);
    bool czy_na(Kula &);
    virtual void cecha(Kula &) = 0; // funkcja wirtualna, która opisuje typ odbijacza
    Odbijacz(double, double);
    Odbijacz(const Odbijacz &) = default;
    Odbijacz() = delete;
    virtual ~Odbijacz()=default;
    Odbijacz &operator=(const Odbijacz &) = default;
}; //---------------------------------------------------------ODBIJACZ----

//---------------------------------------------------------ZWYKLA----
class Zwykla : public Kula
{
public:
    Zwykla(double p, double r, double q, double a, double b, double x, double y) : Kula(p, r, q, a, b, x, y) {}
    void cecha(Odbijacz &o, int k) override {}
};
//---------------------------------------------------------ZWYKLA----

//---------------------------------------------------------TARAN----
class Taran : public Kula
{
public:
    Taran(double p, double r, double q, double a, double b, double x, double y) : Kula(p, r, q, a, b, x, y) {}
    void cecha(Odbijacz &o, int k) override
    {
        if (czy_na_odb(o))
        {
            o.setx(-1);
            o.sety(-1);
        }
    }
};
//---------------------------------------------------------TARAN----

//---------------------------------------------------------WYBUCHOWA----
class Wybuchowa : public Kula
{
public:
    int k; // liczba tur po jakiej wybucha
    Wybuchowa(double p, double r, double q, double a, double b, double x, double y, int k) : Kula(p, r, q, a, b, x, y), k(k) {}
    void cecha(Odbijacz &o, int k) override
    {
        if (k == 0)
        {
            if (czy_na_odb(o))
            {
                o.setx(-1);
                o.sety(-1);
            }
            setp(0);
        }
    }
};
//---------------------------------------------------------WYBUCHOWA----

//---------------------------------------------------------LEWY----
class Lewy : public Odbijacz
{
public:
    Lewy(double x, double y) : Odbijacz(x, y) {}
    void cecha(Kula &k) override
    {
        if (k.geta() == 0 || k.getb() == 0)
        {
            double temp = k.geta();
            k.seta(k.getb());
            k.setb(temp);
        }
        else
        {
            k.seta(k.geta() * (-1));
            k.setb(k.getb() * (-1));
        }
    }
}; //---------------------------------------------------------LEWY----

//---------------------------------------------------------PRAWY----
class Prawy : public Odbijacz
{
public:
    Prawy(double x, double y) : Odbijacz(x, y) {}
    void cecha(Kula &k) override
    {
        if (k.geta() == 0 || k.getb() == 0)
        {
            double temp = k.geta() * (-1);
            k.seta(k.getb() * (-1));
            k.setb(temp);
        }
        else
        {
            k.seta(k.geta() * (-1));
            k.setb(k.getb() * (-1));
        }
    }
}; //---------------------------------------------------------PRAWY----

//---------------------------------------------------------ZNIKAJACY----
class Znikajacy : public Odbijacz
{
public:
    Znikajacy(double x, double y) : Odbijacz(x, y) {}
    void cecha(Kula &k) override
    {
        k.setp(0);
    }
};
//---------------------------------------------------------ZNIKAJACY----

//---------------------------------------------------------SPOWALNIAJACY----
class Spowalniajacy : public Odbijacz
{
public:
    Spowalniajacy(double x, double y) : Odbijacz(x, y) {}
    void cecha(Kula &k) override
    {
        k.setp(k.getp() * 0.5);
    }
};
//---------------------------------------------------------SPOWALNIAJACY----

//---------------------------------------------------------PRZYSPIESZAJACY----
class Przyspieszajacy : public Odbijacz
{
public:
    Przyspieszajacy(double x, double y) : Odbijacz(x, y) {}
    void cecha(Kula &k) override
    {
        k.setp(k.getp() * 2);
    }
};
//---------------------------------------------------------PRZYSPIESZAJACY----

//---------------------------------------------------------MGLOWY----
class Mglowy : public Odbijacz
{
public:
    Mglowy(double x, double y) : Odbijacz(x, y) {}
    void cecha(Kula &k) override{};
};
//---------------------------------------------------------MGLOWY----

//---------------------------------------------------------LOSOWY----
class Losowy : public Odbijacz
{
public:
    Losowy(double x, double y) : Odbijacz(x, y) {}
    void cecha(Kula &k) override
    {

        int t;
        srand(time(NULL));
        k.seta(rand() % 3 - 1);
        if (k.geta() == 0)
        {
            t = rand() % 2 + 1;
            k.setb(pow(-1, t));
        }
        else
        {
            srand(time(NULL) + 1);
            k.setb(rand() % 3 - 1);
        }
    };
};
//---------------------------------------------------------LOSOWY----
//-------------------------------------------------------implementacja PLANSZA--
void Plansza::sprawdz_poz_kul()
{
    for (int i = 0; i < tabk.size(); i++)
    {
        if (tabk[i]->getx() <= 1 || tabk[i]->getx() >= n || tabk[i]->gety() <= 1 || tabk[i]->gety() >= m || tabk[i]->getp() == 0)
        {
            tabk[i]->wypisz();
            tabk[i]->setp(0);
            tabk[i]->setx(-1);
            tabk[i]->sety(-1);
        }
    }
}
void Plansza::sprawdz_usun()
{

    for (int j = 0; j < tabo.size(); j++)
    {
        if (tabo[j]->getx() == -1)
        {
            tabo.erase(tabo.begin() + j);
            j--;
        }
    }
    for (int j = 0; j < tabk.size(); j++)
    {
        if (tabk[j]->getx() == -1)
        {
            tabk.erase(tabk.begin() + j);
            j--;
        }
    }
}

void Plansza::przesun()
{
    for (int i = 0; i < tabk.size(); i++)
    {
        tabk[i]->przesun();
        zderzenie();
    }
}
void Plansza::egz_ceche_kul(int c)
{
    for (int i = 0; i < tabk.size(); i++)
    {
        for (int j = 0; j < tabo.size(); j++)
        {
            tabk[i]->cecha(*tabo[j], c);
        }
    }
}
void Plansza::egz_ceche_odb()
{
    for (int i = 0; i < tabo.size(); i++)
    {
        for (int j = 0; j < tabk.size(); j++)
        {
            if (tabo[i]->czy_na(*tabk[j]))
            {
                tabo[i]->cecha(*tabk[j]);
            }
        }
    }
}
void Plansza::zderzenie()
{
    for (int i = 0; i < tabk.size(); i++)
    {
        for (int j = i + 1; j < tabk.size(); j++)
        {
            tabk[i]->zderzenie(*tabk[j]);
        }
    }
}
void gra(Plansza &p, int c)
{
    cout << "\nwczytano:\n";
    cout << "liczba tur: " << p.getk() << ", szerokosc: " << p.getn() << ", wysokosc: " << p.getm() << ", liczba tur kuli wybuchowej: " << c << "\n";
    while (p.getk() >= 1)
    {
        p.przesun();
        c--;
        p.egz_ceche_kul(c);
        p.sprawdz_poz_kul();
        p.sprawdz_usun();
        p.egz_ceche_odb();
        p.setk(p.getk()-1); // przyjmuje konwencje:jesli dwie kulki spotkaja sie na odbijaczu, to odbijacz dziala na wektory predkości zamienione wczesniej przez fakt zderzenia
    }
}
int Plansza::getk()
{
    return k;
}
int Plansza::getn()
{
    return n;
}
int Plansza::getm()
{
    return m;
}
void Plansza::setk(int c)
{
    k = c;
}
Plansza::Plansza(int e, int f, int g, vector<Kula *> taba, vector<Odbijacz *> tabb)
{
    assert(e > 0);
    assert(f > 0);
    assert(g > 0);
    k = e;
    n = f + 2;
    m = g + 2;
    tabk = taba;
    tabo = tabb;
}
Plansza::Plansza(const Plansza &plansza)
{
    n = plansza.n;
    m = plansza.m;
    k = plansza.k;
    // Tworzenie kopii elementów z drugiej planszy
    vector<Kula *> tabk;
    vector<Odbijacz *> tabo;
    for (int i = 0; i < tabk.size(); i++)
        tabk[i] = plansza.tabk[i];

    for (int i = 0; i < tabo.size(); i++)
        tabo[i] = plansza.tabo[i];
}
Plansza &Plansza::operator=(const Plansza &plansza)
{
    if (this != &plansza)
    {
        // usuwam stare tablice
        tabk.clear();
        tabo.clear();

        // kopiuję drugi obiekt
        n = plansza.n;
        m = plansza.m;
        k = plansza.k;
        vector<Kula *> tabk;
        vector<Odbijacz *> tabo;
        for (int i = 0; i < tabk.size(); i++)
            tabk[i] = plansza.tabk[i];

        for (int i = 0; i < tabo.size(); i++)
            tabo[i] = plansza.tabo[i];
    }
}
Plansza::~Plansza()
{
    tabk.clear();
    tabo.clear();
} //-------------------------------------------------------implementacja PLANSZA--

//-------------------------------------------------------implementacja ODBIJACZ--
double Odbijacz::getx() const
{
    return x;
}
double Odbijacz::gety() const
{
    return y;
}
void Odbijacz::setx(double c)
{
    x = c;
}
void Odbijacz::sety(double c)
{
    y = c;
}
bool Odbijacz::czy_na(Kula &k)
{
    if (getx() == int(k.getx()) && gety() == int(k.gety()))
    {
        return true;
    }
    else
        return false;
}
Odbijacz::Odbijacz(double x, double y) : x(x), y(y) {}

//-------------------------------------------------------implementacja KULA--
void Kula::setr(double c)
{
    r = c;
}
void Kula::setq(double c)
{
    q = c;
}
void Kula::setx(double c)
{
    x = c;
}
void Kula::sety(double c)
{
    y = c;
}
void Kula::seta(double c)
{
    a = c;
}
void Kula::setb(double c)
{
    b = c;
}
void Kula::setp(double c)
{
    p = c;
}
double Kula::getp() const
{
    return p;
}
double Kula::getr() const
{
    return r;
}
double Kula::getq() const
{
    return q;
}
double Kula::getx() const
{
    return x;
}
double Kula::gety() const
{
    return y;
}
double Kula::geta() const
{
    return a;
}
double Kula::getb() const
{
    return b;
}
void Kula::przesun()
{
    setx(getx() + getp() * geta());
    sety(gety() + getp() * getb());
}
bool Kula::czy_na_kul(Kula &k)
{
    if (int(getx()) == int(k.getx()) && int(gety()) == int(k.gety()))
    {
        return true;
    }
    else
        return false;
}
bool Kula::czy_na_odb(Odbijacz &o)
{
    if (int(getx()) == o.getx() && int(gety()) == o.gety())
    {
        return true;
    }
    else
        return false;
}
void Kula::zderzenie(Kula &k)
{
    if (czy_na_kul(k))
    {
        double ka = k.geta();
        double kb = k.getb();
        k.seta(geta());
        k.setb(getb());
        seta(ka);
        setb(kb);
    }
}
void Kula::wypisz()
{
    cout << "(" << getr() << "," << getq() << ")----->(" << getx() << "," << gety() << ")\n";
}

Kula::Kula(double p, double r, double q, double a, double b, double x, double y) : p(p), r(r), q(q), x(x), y(y), a(a), b(b)
{
}
//-------------------------------------------------------implementacja KULA--
//-------------------------------------------------------WCZYTYWANIE--
void wczytaj(int c)
{
    ifstream plik("dane.txt");
    if (plik.good() == false)
    {
        cout << "Nie udalo sie wczytac pliku\n";
        exit(0);
    }
    char znak;
    vector<char> info;
    vector<Kula *> tabk;
    vector<Odbijacz *> tabo;
    int k, n, m;
    plik >> k >> n >> m;
    // sprawdzam poprawnosc danych o rozmiarze
    int l = 0;
    if (plik.is_open())
    {
        while (plik.get(znak))
        {
            if (znak == '\n')
            {
                cout << "\n";
                cout << "|";
                l++; // ta iteracja jest tylko po to zeby ominac w czytaniu pierwsza linijke
            }
            if (znak != '\n')
            {
                if (l > 0)
                {
                    info.push_back(znak);
                    cout << znak << "|";
                }
            }
        }
    }

    for (int i = 0; i < info.size(); i++)
    {
        if (!isspace(info[i]))
        {
            int y, x;
            y = (i / (n + 2)) + 1;
            x = i - (n + 2) * (y - 1) + 1; // zamiana numeru w wektorze na wspolrzedne (x,y) na planszy
            //--------------------------------------------------KULE NA ROGACH------------
            if (x == 1 && y == 1) // LEWY GORNY ROG
            {

                if (info[i] == 'Z')
                {
                    tabk.push_back(new Zwykla(1, x, y, 1, 1, x, y));
                }
                if (info[i] == 'T')
                {
                    tabk.push_back(new Taran(1, x, y, 1, 1, x, y));
                    if (info[i] == 'W')
                    {
                        tabk.push_back(new Wybuchowa(1, x, y, 1, 1, x, y, c));
                    }
                }
            }
            else if (x == n + 2 && y == 1) // PRAWY GORNY ROG
            {
                if (info[i] == 'Z')
                {
                    tabk.push_back(new Zwykla(1, x, y, -1, 1, x, y));
                }
                if (info[i] == 'T')
                {
                    tabk.push_back(new Taran(1, x, y, -1, 1, x, y));
                }
                if (info[i] == 'W')
                {
                    tabk.push_back(new Wybuchowa(1, x, y, -1, 1, x, y, c));
                }
            }
            else if (x == 1 && y == m + 2) // LEWY DOLNY ROG
            {
                if (info[i] == 'Z')
                {
                    tabk.push_back(new Zwykla(1, x, y, 1, -1, x, y));
                }
                if (info[i] == 'T')
                {
                    tabk.push_back(new Taran(1, x, y, 1, -1, x, y));
                }
                if (info[i] == 'W')
                {
                    tabk.push_back(new Wybuchowa(1, x, y, 1, -1, x, y, c));
                }
            }
            else if (x == n + 2 && y == m + 2) // PRAWY DOLNY ROG
            {
                if (info[i] == 'Z')
                {
                    tabk.push_back(new Zwykla(1, x, y, -1, -1, x, y));
                }
                if (info[i] == 'T')
                {
                    tabk.push_back(new Taran(1, x, y, -1, -1, x, y));
                }
                if (info[i] == 'W')
                {
                    tabk.push_back(new Wybuchowa(1, x, y, -1, -1, x, y, c));
                }
            }
            //--------------------------------------------------KULE NA ROGACH------------
            //--------------------------------------------------KULE NA BOKACH------------
            else if (x == 1) // KULA NA LEWYM BOKU
            {
                if (info[i] == 'Z')
                {
                    tabk.push_back(new Zwykla(1, x, y, 1, 0, x, y));
                }
                if (info[i] == 'T')
                {
                    tabk.push_back(new Taran(1, x, y, 1, 0, x, y));
                }
                if (info[i] == 'W')
                {
                    tabk.push_back(new Wybuchowa(1, x, y, 1, 0, x, y, c));
                }
            }
            else if (x == n + 2) // KULA NA PRAWYM BOKU
            {
                if (info[i] == 'Z')
                {

                    tabk.push_back(new Zwykla(1, x, y, -1, 0, x, y));
                }
                if (info[i] == 'T')
                {
                    tabk.push_back(new Taran(1, x, y, -1, 0, x, y));
                }
                if (info[i] == 'W')
                {
                    tabk.push_back(new Wybuchowa(1, x, y, -1, 0, x, y, c));
                }
            }
            else if (y == m + 2) // KULA NA DOLNYM BOKU
            {
                if (info[i] == 'Z')
                {

                    tabk.push_back(new Zwykla(1, x, y, 0, -1, x, y));
                }
                if (info[i] == 'T')
                {

                    tabk.push_back(new Taran(1, x, y, 0, -1, x, y));
                }
                if (info[i] == 'W')
                {
                    tabk.push_back(new Wybuchowa(1, x, y, 0, -1, x, y, c));
                }
            }
            else if (y == 1) // KULA NA GORNYM BOKU
            {
                if (info[i] == 'Z')
                {
                    tabk.push_back(new Zwykla(1, x, y, 0, 1, x, y));
                }
                if (info[i] == 'T')
                {

                    tabk.push_back(new Taran(1, x, y, 0, 1, x, y));
                }
                if (info[i] == 'W')
                {
                    tabk.push_back(new Wybuchowa(1, x, y, 0, 1, x, y, c));
                }
            }

            else
            {
                if (info[i] == 'U')
                {
                    tabo.push_back(new Lewy(x, y));
                }
                if (info[i] == 'u')
                {
                    tabo.push_back(new Prawy(x, y));
                }
                if (info[i] == 'L')
                {
                    tabo.push_back(new Losowy(x, y));
                }
                if (info[i] == 'S')
                {
                    tabo.push_back(new Spowalniajacy(x, y));
                }
                if (info[i] == 'M')
                {
                    tabo.push_back(new Mglowy(x, y));
                }
                if (info[i] == 'P')
                {
                    tabo.push_back(new Przyspieszajacy(x, y));
                }
                if (info[i] == 'Z')
                {
                    tabo.push_back(new Znikajacy(x, y));
                }
            }
        }
    }
    Plansza plansza(k, n, m, tabk, tabo);
    gra(plansza, c);
    for (int i = 0; i < plansza.tabk.size(); i++)
    {
        plansza.tabk[i]->wypisz();
    }
}
int main()
{
    int c;
    int d;
    cout << "Podaj liczbe tur po jakiej ma wybuchnac kula Wybuchowa: \n";
    cin >> c;
    cout << "\n";
    wczytaj(c);
    cout << "koniec gry\n";
    cin >> d;
    return 0;
}