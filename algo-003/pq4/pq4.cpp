#include <list>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <stack>

using namespace std;

struct vertex
{
    int _id;
    list<int> _vc_id;

    vertex(int id)
    {
        _id = id;
    }
};

struct graph
{
    unordered_map<int, vertex*> _vertices;
    unordered_set<int> _visited;
    unordered_set<int> _finished;
//    unordered_map<int, int> _ftime;
    map<int, int> _leader;
    vector<int> _forder;

    graph()
    {
    }

    ~graph()
    {
        for (auto it=_vertices.begin(); it!=_vertices.end(); ++it)
        {
            delete it->second;
            it->second = nullptr;
        }
    }

    int get_edge_size()
    {
        int n_edge = 0;
        for (auto it=_vertices.begin(); it!=_vertices.end(); ++it)
        {
            n_edge += it->second->_vc_id.size();
        }
        return n_edge;
    }

    bool add_edge(int v1_id, int v2_id)
    {
        // create vertex if not exist
        auto it1 = _vertices.find(v1_id);
        if (it1 == _vertices.end())
        {
            vertex* v = new vertex(v1_id);
            _vertices.insert(make_pair(v1_id, v));
            it1 = _vertices.find(v1_id);
            if (it1 == _vertices.end())
            {
                cerr << "create vertex error" << endl;
                return false;
            }
        }
        auto it2 = _vertices.find(v2_id);
        if (it2 == _vertices.end())
        {
            vertex* v = new vertex(v2_id);
            _vertices.insert(make_pair(v2_id, v));
            it2 = _vertices.find(v2_id);
            if (it2 == _vertices.end())
            {
                cerr << "create vertex error" << endl;
                return false;
            }
        }

        it1->second->_vc_id.push_back(v2_id);

        return true;
    }

    void dfs(int v_id)
    {
        // check if visited already
        if (_visited.find(v_id) != _visited.end())
        {
            return;
        }

        _leader[v_id] = 0;

        stack<int> sv;
        sv.push(v_id);
        while (!sv.empty())
        {
            int v_id_now = sv.top();

            if (_visited.find(v_id_now) == _visited.end())
            {
                _visited.insert(v_id_now);
                auto it = _vertices.find(v_id_now);
                for (auto itc=it->second->_vc_id.begin(); itc!=it->second->_vc_id.end(); ++itc)
                {
                    int vc_id = (*itc);
                    if (_visited.find(vc_id) == _visited.end() && _finished.find(vc_id) == _finished.end())
                    {
                        sv.push(vc_id);
                    }
                }
            }
            else
            {
                // revisit means finished
                if (_finished.find(v_id_now) == _finished.end())
                {
                    _forder.push_back(v_id_now);
                    ++_leader[v_id];
                    _finished.insert(v_id_now);
                }
                sv.pop();
                // cout << "vertex " << v_id_now << " finished, stack size " << sv.size() << endl;
            }
            // cout << "vertex " << v_id_now << " visited, stack size " << sv.size() << endl;
        }
    }

    void dfs_loop(vector<int>& order)
    {
        int n_v = _vertices.size();
        _visited.clear();
        _leader.clear();
        _forder.clear();
        _forder.reserve(n_v);
        _finished.clear();

        // if there is no giving order, random the visiting order
        if (order.empty())
        {
            for (auto it=_vertices.begin(); it!=_vertices.end(); ++it)
            {
                order.push_back(it->first);
            }
            random_shuffle(order.begin(), order.end());
        }

        for (auto it=order.begin(); it!=order.end(); ++it)
        {
            int v_id = *it;
            dfs(v_id);
//            cout << "leader_now: " << v_id << ", ftime_now: " << _forder.size() << endl;
//            cout << _visited.size() << " vertices visited." << endl;
        }
    }
};

int main(int argc, char* argv[])
{
    string filename = string(argv[1]);

    // read file and construct the graph
    graph g;
    graph g_r;
    ifstream ifs;
    ifs.open(filename, ios::in);
    if (ifs.fail())
    {
        cerr << "open file error." << endl;
        return -1;
    }
    int n_line = 0;
    while (!ifs.eof())
    {
        string line;
        getline(ifs, line);
        if (line.length() > 0)
        {
            istringstream ss(line);
            int v1_id;
            int v2_id;
            ss >> v1_id;
            ss >> v2_id;
            g_r.add_edge(v1_id, v2_id);
            g.add_edge(v2_id, v1_id);
        }
        ++n_line;
    }
    ifs.close();
    cout << "graph constructed, with " << g._vertices.size() << " vertices and " << g.get_edge_size() << " edges." << endl;
    cout << "reverse graph constructed, with " << g_r._vertices.size() << " vertices and " << g_r.get_edge_size() << " edges." << endl;

    srand(time(NULL));

    // 1st pass
    vector<int> order;
    order.clear();
    g_r.dfs_loop(order);

    // 2nd pass
    order = g_r._forder;
    reverse(order.begin(), order.end());
    g.dfs_loop(order);

    // result
    vector<int> scc_size;
    for (auto it=g._leader.begin(); it!=g._leader.end(); ++it)
    {
        scc_size.push_back(it->second);
    }
    sort(scc_size.begin(), scc_size.end(), greater<int>());
    cout << "There are " << scc_size.size() << " SCCs, the size of 5 largest is:" << endl;
    for (int i=0; i<5; ++i)
    {
        if (i < scc_size.size())
        {
            cout << scc_size[i] << endl;
        }
        else
        {
            cout << 0 << endl;
        }
    }

    return 0;
}
