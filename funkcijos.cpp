#include "funkcijos.h"

static void prid_eil(vector<int>& eil_nr, int numeris)
{
    for (int esamas : eil_nr) if (esamas == numeris) return;
    eil_nr.push_back(numeris);
}

bool nuskaityti_teksta_is_failo(const string& kel, vector<string>& eilutes)
{
    eilutes.clear();
    ifstream ivedimas(kel);
    if (!ivedimas.is_open()) return false;

    string eilute;
    while (getline(ivedimas, eilute))
    {
        if (!eilute.empty() && eilute.back()=='\r')eilute.pop_back();
        eilutes.push_back(eilute);
    }
    return true;
}

bool nuskaityti_galunes_is_failo(const string& kel, vector<string>& galunes)
{
    galunes.clear();
    ifstream ivedimas(kel);
    if (!ivedimas.is_open()) return false;

    string galune;
    while (ivedimas >> galune)
    {
        if (!galune.empty() && galune.back() == '\r') galune.pop_back();

        for (int i = 0; i < (int)galune.size(); i++)
            galune[i] = (char)tolower((unsigned char)galune[i]);

        if (!galune.empty() && galune[0] == '.')
        {
            string be_tasko;
            for (int i = 1; i < (int)galune.size(); i++) be_tasko.push_back(galune[i]);
            galune = be_tasko;
        }

        if (!galune.empty()) galunes.push_back(galune);
    }
    return true;
}

string isvalyti_zodi(const string& teis)
{
    if (teis.empty()) return "";

    int kaire = 0;
    int desine = (int)teis.size() - 1;

    auto skyryba = [](unsigned char c) {
        if (c < 128)
            return ispunct(c) != 0;

        return c == '„' || c == '“' || c == '–' || c == '—' || c == '"';
        };


    while (kaire <= desine)
    {
        unsigned char c = (unsigned char)teis[kaire];
        if (skyryba(c)) kaire++;
        else break;
    }

    while (desine >= kaire)
    {
        unsigned char c = (unsigned char)teis[desine];
        if (skyryba(c)) desine--;
        else break;
    }

    if (kaire > desine) return "";

    string isvalytas;
    for (int i = kaire; i <= desine; i++)
    {
        unsigned char c = (unsigned char)teis[i];

        if (c >= 'A' && c <= 'Z') isvalytas.push_back((char)(c + 32));
        else if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
            isvalytas.push_back((char)c);
        else if (c >= 128)
            isvalytas.push_back((char)c);
    }

    if (isvalytas.size() >= 2)
    {
        if (isvalytas.substr(0, 2) == "„" || isvalytas.substr(0, 2) == "“")
            isvalytas.erase(0, 2);

        if (isvalytas.size() >= 2 &&
            (isvalytas.substr(isvalytas.size() - 2) == "„" ||
             isvalytas.substr(isvalytas.size() - 2) == "“"))
            isvalytas.erase(isvalytas.size() - 2);
    }

    return isvalytas;
}

static bool turi_raidziu(const string& zodis)
{
    for (unsigned char c : zodis)
    {
        if (isalpha(c)) return true;
        if (c >= 128) return true;
    }
    return false;
}

void zodziu_skaiciavimas(const vector<string>& eilutes,unordered_map<string, int>& kiek,unordered_map<string, vector<int>>& zodzio_eilutes)
{
    kiek.clear();
    zodzio_eilutes.clear();

    for (int eilutes_indeksas = 0; eilutes_indeksas < (int)eilutes.size(); eilutes_indeksas++)
    {
        stringstream srautas(eilutes[eilutes_indeksas]);
        string zodis_teis;

        while (srautas >> zodis_teis)
        {
            string zodis = isvalyti_zodi(zodis_teis);
            if (zodis.empty()) continue;
            if (!turi_raidziu(zodis)) continue;

            kiek[zodis]++;
            prid_eil(zodzio_eilutes[zodis], eilutes_indeksas + 1);
        }
    }
}

void zodziu_surikiavimas(const unordered_map<string, int>& kiek,vector<pair<string, int>>& surikiuoti)
{
    surikiuoti.clear();

    for (const auto& pora : kiek)
        if (pora.second > 1)
            surikiuoti.push_back(pora);

    sort(surikiuoti.begin(), surikiuoti.end(),
        [](const pair<string, int>& a, const pair<string, int>& b)
        {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });
}

void zodziu_isvedimas_i_faila(const string& kel,const vector<pair<string, int>>& surikiuoti,const unordered_map<string, vector<int>>& zodzio_eilutes)
{
    ofstream isvestis(kel);
    if (!isvestis.is_open()) return;

    isvestis << left << setw(25) << "Zodis" << setw(10) << "Viso" << "Eilutes" << endl;

    for (const auto& pora : surikiuoti)
    {
        isvestis << left << setw(25) << pora.first
            << setw(10) << pora.second;

        auto rasta = zodzio_eilutes.find(pora.first);
        if (rasta != zodzio_eilutes.end())
            for (int nr : rasta->second) isvestis << nr << " ";

        isvestis << endl;
    }
}

static bool blogas_galo_simb(char c)
{
    return c == '.' || c == ',' || c == ';' || c == ':' || c == '!' || c == '?' ||
        c == ')' || c == ']' || c == '}' || c == '>' ||
        c == '"' || c == '\'' || c == '[';
}

static string url_galas(string url)
{
    while (!url.empty() && blogas_galo_simb(url.back()))
        url.pop_back();
    return url;
}


static string istr_t(string url)
{
    for (int i = 0; i < (int)url.size(); i++)
        url[i] = (char)tolower((unsigned char)url[i]);

    int ilgis = (int)url.size();
    if (ilgis == 0) return "";

    int pradzia = 0;

    for (int i = 0; i + 2 < ilgis; i++)
    {
        if (url[i] == ':' && url[i + 1] == '/' && url[i + 2] == '/')
        {
            pradzia = i + 3;
            break;
        }
    }

    int pabaiga = ilgis - 1;
    for (int i = pradzia; i < ilgis; i++)
    {
        char c = url[i];
        if (c == '/' || c == '?' || c == '#')
        {
            pabaiga = i - 1;
            break;
        }
    }
    if (pabaiga < pradzia) return "";

    int tasko_pozicija = -1;
    for (int i = pabaiga; i >= pradzia; i--)
    {
        if (url[i] == '.')
        {
            tasko_pozicija = i;
            break;
        }
    }
    if (tasko_pozicija == -1) return "";

    string tld;
    for (int i = tasko_pozicija + 1; i <= pabaiga; i++)
    {
        char c = url[i];
        if (c >= 'a' && c <= 'z') tld.push_back(c);
        else break;
    }

    return tld;
}

static bool yra_sar(const string& tld, const vector<string>& galunes)
{
    if (tld.empty()) return false;
    for (int i = 0; i < (int)galunes.size(); i++)
        if (galunes[i] == tld) return true;
    return false;
}

void nuorodu_istraukimas(const vector<string>& eilutes,const vector<string>& galunes,vector<string>& nuorodos)
{
    nuorodos.clear();

    regex url_regex(
        R"((https?://[^\s<>"']+|www\.[^\s<>"']+|[A-Za-z0-9-]+\.[A-Za-z]{2,}(?:/[^\s<>"']*)?))",
        icase
    );

    for (int eilutes_nr = 0; eilutes_nr < (int)eilutes.size(); eilutes_nr++)
    {
        const string& eilute = eilutes[eilutes_nr];
        auto pradzia = sregex_iterator(eilute.begin(), eilute.end(), url_regex);
        auto pabaiga = sregex_iterator();

        for (auto it = pradzia; it != pabaiga; ++it)
        {
            string url = (*it).str();

            if (url.empty()) continue;

            string be_lauztiniu;
            for (int i = 0; i < (int)url.size(); i++)
            {
                if (url[i] == '[') break;
                be_lauztiniu.push_back(url[i]);
            }
            url = be_lauztiniu;
            url = url_galas(url);
            if (url.empty()) continue;

            string tld = istr_t(url);
            if (!yra_sar(tld, galunes)) continue;

            bool yra = false;
            for (int j = 0; j < (int)nuorodos.size(); j++)
                if (nuorodos[j] == url) { yra = true; break; }

            if (!yra) nuorodos.push_back(url);
        }
    }
    sort(nuorodos.begin(), nuorodos.end());
}

void nuorodu_isvedimas_i_faila(const string& kel, const vector<string>& nuorodos)
{
    ofstream isvestis(kel);
    if (!isvestis.is_open()) return;

    for (int i = 0; i < (int)nuorodos.size(); i++)
        isvestis << i + 1 << ". " << nuorodos[i] << endl;
}
