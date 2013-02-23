#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <list>

using namespace std;

struct edge;
struct vertex;

struct vertex
{
    int _id;
//    list<edge*> _edges;

    vertex(int id)
    {
        _id = id;
    }
};

struct edge
{
    vertex* _v1;
    vertex* _v2;

    edge(vertex* v1, vertex* v2)
    {
        _v1 = v1;
        _v2 = v2;
    }

    void standarize()
    {
//        if (_v1->_id > _v2->_id)
//        {
//            swap(_v1, _v2);
//        }
    }
};

struct graph
{
    unordered_map<int, vertex*> _vertices;
    list<edge*> _edges;

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
        for (auto it=_edges.begin(); it!=_edges.end(); ++it)
        {
            delete *it;
            *it = nullptr;
        }
    }

    explicit graph(const graph& g)
    {
        for (auto it=g._edges.begin(); it!=g._edges.end(); ++it)
        {
            add_edge((*it)->_v1->_id, (*it)->_v2->_id);
        }
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
            }
        }

        // new edge
        edge* e = new edge(it1->second, it2->second);
        e->standarize();

        // add to edge list
        _edges.push_back(e);

//        // add to vertex
//        e->_v1->_edges.push_back(e);
//        e->_v2->_edges.push_back(e);

        return true;
    }

    void random_contradiction()
    {
        // choose random edge
        int r = rand() % _edges.size();
        auto it_ec = _edges.begin();
        advance(it_ec, r);
        edge* e_c = *it_ec;
        vertex* v1 = e_c->_v1;
        vertex* v2 = e_c->_v2;

        for (auto it=_edges.begin(); it!=_edges.end(); )
        {
            edge* e = *it;
            // modify edge end point
            if (e->_v1 == v2)
            {
                e->_v1 = v1;
            }
            if (e->_v2 == v2)
            {
                e->_v2 = v1;
            }
            e->standarize();
            // remove self loop
            if (e->_v1 == e->_v2)
            {
                it = _edges.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // remove vertex
        _vertices.erase(v2->_id);
    }

    int try_min_cut()
    {
        srand(time(NULL));
        while (_vertices.size() > 2)
        {
            random_contradiction();
        }
        return _edges.size();
    }
};

int main(int argc, char* argv[])
{
    string filename = string(argv[1]);
    int n_try = atoi(argv[2]);
    // cout << filename << endl;

    // read file and construct the graph
    graph g;
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
        //cout << line << endl;
        int n_edge = 0;
        if (line.length() > 0)
        {
            istringstream ss(line);
            int v1_id;
            int v2_id;
            ss >> v1_id;
            while (!ss.eof())
            {
                ss >> v2_id;
                if (v2_id == -1)
                {
                    break;
                }
                if (v1_id < v2_id)
                {
                    g.add_edge(v1_id, v2_id);
                    ++n_edge;
                }
                v2_id = -1;
            }
        }
        ++n_line;
        cout << "read line " << n_line << ", " << n_edge << " edges added." << endl;
    }
    ifs.close();
    cout << "graph constructed, with " << g._vertices.size() << " vertices and " << g._edges.size() << " edges." << endl;

    // try min_cut
    int cut_min = g._edges.size();
    for (int i=0; i<n_try; ++i)
    {
        graph g_cut(g);
        // cout << "graph constructed, with " << g_cut._vertices.size() << " vertices and " << g_cut._edges.size() << " edges." << endl;
        int cut_this = g_cut.try_min_cut();
        if (cut_this < cut_min)
        {
            cut_min = cut_this;
        }
        cout << "min-cut try round " << i << ", cut_this = " << cut_this << ", cut_min = " << cut_min << endl;
    }

    return 0;
}
