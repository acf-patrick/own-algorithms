#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

/* dommage... le C++ n'a pas de fonction toute prête */
int randint(int a, int b);

/* génère toute les combinaisons d'indices possibles */
void comb(int nbr, std::vector< std::vector<int> >& out, std::vector<int> index, bool first = true);

/* Prédicat pour arranger l'ordre de nos sous-ensembles (facultatif) */
bool compare( std::vector<int> a, std::vector<int> b) { return a.size()<b.size(); }

/** 
* retourne les sous-ensembles sous forme de tableau à deux dimensions
* ex: {1, 2, 3} => {{}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}}
*/
std::vector< std::vector<int> > combinaison(const std::vector<int>& ensemble)
{
    std::vector< std::vector<int> > index;
    // generate all possibilities
    comb(ensemble.size(), index, std::vector<int>());

    for (int i=0; i<(int)index.size(); ++i)
    {
        for (int j=0; j<(int)index[i].size(); ++j)
            index[i][j] = ensemble[index[i][j]];
        std::sort(index[i].begin(), index[i].end());
    }
    std::sort(index.begin(), index.end(), compare);
    return index;
}

int main()
{
    int card;
    std::cin >> card;
    srand(time(0));
    std::vector<int> ens;
    std::cout << "ensemble : {";
    int i, j;
    for (i=0; i<card; ++i)
    {
        ens.push_back(randint(0, 20));
        std::cout << ens[i] << ((i==card-1)?"":", ");
    }
    std::cout << "}\n";
    std::vector< std::vector<int> > s_ens(combinaison(ens));
    std::cout << "Nombre de sous-ensemble : " << s_ens.size() << std::endl;
    std::cout << "sous-ensembles : ";
    for (i=0; i<(int)s_ens.size(); ++i)
    {
        std::cout << '{';
        for (j=0; j<(int)s_ens[i].size(); ++j)
            std::cout << s_ens[i][j] << ((j == (int)s_ens[i].size()-1)?"":", ");
        std::cout << '}' << ((i == (int)s_ens.size() - 1)?"":", ");
    }
    return 0;
}

void comb(int nbr, std::vector< std::vector<int> >& out, std::vector<int> index, bool first)
{
    // safety check
    if (nbr<=0)
        return;

    int start;
    if (first)
    {
        out.push_back(std::vector<int>());
        start = 0;
    }
    else
        start = index[index.size()-1]+1;
    for (int i=start; i<nbr; ++i)
    {
        std::vector<int> tmp(index);
        tmp.push_back(i);
        out.push_back(tmp);
        if (i == nbr-1)
            return;
        comb(nbr, out, tmp, false);
    }
}

int randint(int a, int b)
{
    if (a>b) return randint(b, a);
    return ((rand()/(float)RAND_MAX)*(b-a)+a);
}
