#include <list>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

typedef unsigned int weight_t;

//typedef pair<weight_t, int> weight_to_t;

//struct greater_weight_to : public binary_function<weight_to_t, weight_to_t, bool>
//{
//    bool operator () (const weight_to_t& w1, const weight_to_t& w2)
//    {
//        return w1.first > w2.first;
//    }
//};

struct vertex
{
    int _id;
    list<pair<int, weight_t> > _edges;

    vertex(int id)
    {
        _id = id;
    }

    void add_edge(int id, weight_t weight)
    {
        _edges.push_back(make_pair(id, weight));
    }
};

struct graph
{
    unordered_map<int, vertex*> _vertices;
//    unordered_map<pair<int, int>, weight_t> _edges;

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
            n_edge += it->second->_edges.size();
        }
        return n_edge;
    }

    bool add_edge(int v1_id, int v2_id, weight_t w)
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

        it1->second->add_edge(v2_id, w);

        return true;
    }

    void dijkstra(int id_s, unordered_map<int, int>& dist)
    {
//        priority_queue<weight_to_t, vector<weight_to_t>, greater_weight_to()> heap;
        multimap<weight_t, int> heap;
        dist.clear();
        dist.insert(make_pair(id_s, 0));

        int id_now = id_s;
        while (id_now != -1)
        {
            auto it_v = _vertices.find(id_now);
            for (auto it_e=it_v->second->_edges.begin(); it_e!=it_v->second->_edges.end(); ++it_e)
            {
                int id_c = it_e->first;
                weight_t w_c = it_e->second;

                auto it_c = dist.find(id_c);
                if (it_c == dist.end())
                {
                    // add new distance
                    int dist_c = dist[id_now] + w_c;
                    dist.insert(make_pair(id_c, dist_c));
                    heap.insert(make_pair(dist_c, id_c));
                }
                else
                {
                    // update distance
                    int dist_c_o = it_c->second;
                    int dist_c_n = dist[id_now] + w_c;
                    if (dist_c_n < dist_c_o)
                    {
                        it_c->second = dist_c_n;
                        for (auto it_h=heap.find(dist_c_o); it_h!=heap.end(); ++it_h)
                        {
                            if (it_h->second == id_c)
                            {
                                heap.erase(it_h);
                            }
                        }
                        heap.insert(make_pair(dist_c_n, id_c));
                    }
                }
            }

            // remove current vertex from the heap
            for (auto it_h=heap.find(dist[id_now]); it_h!=heap.end(); ++it_h)
            {
                if (it_h->second == id_now)
                {
                    heap.erase(it_h);
                }
            }

            // next vertex
            if (!heap.empty())
            {
                id_now = heap.begin()->second;
            }
            else
            {
                id_now = -1;
            }
        }
    }
};

int main(int argc, char* argv[])
{
    string filename = string(argv[1]);

    // read file and construct the graph
    graph g;
    ifstream ifs;
    ifs.open(filename, ios::in);
    if (ifs.fail())
    {
        cerr << "open file error." << endl;
        return -1;
    }
    while (!ifs.eof())
    {
        string line;
        getline(ifs, line);
        if (line.length() > 0)
        {
            istringstream ss(line);
            int v1_id;
            ss >> v1_id;
            while (!ss.eof())
            {
                string str_v2_id_w;
                ss >> str_v2_id_w;
                if (!str_v2_id_w.empty())
                {
                    istringstream sse(str_v2_id_w);
                    string str_v2_id;
                    getline(sse, str_v2_id, ',');
                    string str_w;
                    getline(sse, str_w);
                    int v2_id = atoi(str_v2_id.c_str());
                    weight_t w = atoi(str_w.c_str());

                    g.add_edge(v1_id, v2_id, w);
                }
            }
        }
    }
    ifs.close();
    cout << "graph constructed, with " << g._vertices.size() << " vertices and " << g.get_edge_size() << " edges." << endl;

    unordered_map<int, int> dist;
    g.dijkstra(1, dist);

    vector<int> display = {7,37,59,82,99,115,133,165,188,197};
    for (auto it=display.begin(); it!=display.end(); ++it)
    {
        cout << dist[*it] << ',';
    }

    return 0;
}

