#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    string filename = string(argv[1]);
    int left = 2500;
    int right = 4000;

    // read file
    vector<unsigned int> data;
    ifstream ifs;
    ifs.open(filename, ios::in);
    if (ifs.fail())
    {
        cerr << "open file error." << endl;
        return -1;
    }
    while (!ifs.eof())
    {
        unsigned int v;
        ifs >> v;
        if (v<right)
        {
            data.push_back(v);
        }
    }
    cout << data.size() << " integers readed." << endl;

    unordered_set<unsigned int> res;
    for (auto it1=data.begin(); it1!=data.end(); ++it1)
    {
        for (auto it2=it1+1; it2!=data.end(); ++it2)
        {
            if (*it1 == *it2)
            {
                continue;
            }
            unsigned int sum = *it1 + *it2;
            if (sum >= 2500 && sum <=4000)
            {
                res.insert(sum);
            }
        }
    }
    cout << res.size() << endl;

    return 0;
}

