#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

std::vector< std::vector<int> > permutations(std::vector<int>);
int foo() { return rand()%100; }
void show(const std::vector<int> v)
{
	int len(v.size());
	std::cout << "{ ";
	for (int i=0; i<len-1; ++i)
		std::cout << v[i] << ", ";
	if (len) std::cout << v[len-1] << " }\n";
}

int main()
{
	srand(time(0));
	int len(rand()%10);

	std::vector<int> ensemble(len);
	std::generate(ensemble.begin(), ensemble.end(), foo);
	std::cout << "Ensemble : ";
	show(ensemble);

    std::vector< std::vector<int> > perm(permutations(ensemble));
    std::cout << "permutations possibles : \n";
    for (int i=0; i<(int)perm.size(); ++i)
    	show(perm[i]);

    std::cout << perm.size() << " permutations!";
	return 0;
}

std::vector< std::vector<int> > permutations(std::vector<int> ensemble)
{
	std::vector< std::vector<int> > ret(1, ensemble);
	int length(ensemble.size());

	if (length <= 1)
		return ret;
	if (length == 2)
	{
		std::reverse(ensemble.begin(), ensemble.end());
		ret.push_back(ensemble);
		return ret;
	}

	for (int i=0; i<length; ++i)
	{
		std::vector<int> tmp;
		for (int j=0; j<length; ++j)
			if (i != j)
				tmp.push_back(ensemble[j]);
		// Permutations des 'length-1'
		std::vector< std::vector<int> > result = permutations(tmp);
		for (int k=0; k<(int)result.size(); ++k)
		{
			result[k].insert(result[k].begin(), ensemble[i]);
			if (std::find(ret.begin(), ret.end(), result[k]) == ret.end())
				ret.push_back(result[k]);
		}
	}
	return ret;
}
