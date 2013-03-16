#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[])
{
    string filename = string(argv[1]);

    // read file
    vector<int> data;
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
        data.push_back(v);
    }
    cout << data.size() << " integers readed." << endl;

    priority_queue<int, vector<int>, greater<int> > h_high;
    priority_queue<int> h_low;
    vector<int> median;
    // initialize
    median.push_back(data[0]);
    median.push_back(min(data[0], data[1]));
    h_high.push(max(data[0], data[1]));
    h_low.push(min(data[0], data[1]));
    for (int i=2; i<data.size(); ++i)
    {
        int d = data[i];

        // push data to queue
        if (d >= h_high.top())
        {
            h_high.push(d);
        }
        else
        {
            h_low.push(d);
        }

        // balance queue
        if (h_high.size() > h_low.size() + 1)
        {
            h_low.push(h_high.top());
            h_high.pop();
        }
        else if (h_low.size() > h_high.size() + 1)
        {
            h_high.push(h_low.top());
            h_low.pop();
        }

        // get median
        double med = 0.0;
        if (h_high.size() > h_low.size())
        {
            med = h_high.top();
        }
        else
        {
            med = h_low.top();
        }

        median.push_back(med);
    }

    int res = 0.0;
    for (auto it=median.begin(); it!=median.end(); ++it)
    {
        res += *it;
        if (res >= 10000)
        {
            res -= 10000;
        }
    }
    cout << res << endl;

    return 0;
}

