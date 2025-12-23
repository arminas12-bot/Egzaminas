#include "funkcijos.h"

int main()
{
    setlocale(LC_ALL, "");

    vector<string> galunes;
    if (!nuskaityti_galunes_is_failo("galunes.txt", galunes))
    {
        cout << "Nepavyko nuskaityti failo.";
        return 1;
    }

    vector<string> eil_zodziams;
    if (!nuskaityti_teksta_is_failo("Lietuva_tik_tekstas.txt", eil_zodziams))
    {
        cout << "Nepavyko nuskaityti failo.";
        return 1;
    }

    vector<string> eil_nuorodoms;
    if (!nuskaityti_teksta_is_failo("Lietuva_su_nuorodom.txt", eil_nuorodoms))
    {
        cout << "Nepavyko nuskaityti failo.";
        return 1;
    }

    unordered_map<string, int> kiek;
    unordered_map<string, vector<int>> zodzio_eilutes;

    zodziu_skaiciavimas(eil_zodziams, kiek, zodzio_eilutes);

    vector<pair<string, int>> surikiuoti;
    zodziu_surikiavimas(kiek, surikiuoti);

    zodziu_isvedimas_i_faila("isvesti_zodziai.txt", surikiuoti, zodzio_eilutes);

    vector<string> nuorodos;
    nuorodu_istraukimas(eil_nuorodoms, galunes, nuorodos);
    nuorodu_isvedimas_i_faila("isvestos_nuorodos.txt", nuorodos);

    cout << "Baigta.";
    return 0;
}
