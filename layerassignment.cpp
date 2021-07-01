#include "layerassignment.h"
#include <bits/stdc++.h>
using namespace std;

void _set_vertex(la_Vertex2d *v, int r, int c, int p, bool i, Problem *pro);
int _find_id_in_vector(vector<Pos> v, Pos t);
la_Vertex2d *_find_vertex(vector<la_Vertex2d*> vs, int row, int col);

vector<la_Net2d> _transform(vector<Net2D> net, Problem *p);

void _quicksort(vector<la_Net2d> &net, vector<double> &score, int l, int r);
void _net_ordering(vector<la_Net2d> &net2d);

void _DFS_visit(la_Vertex2d *u, vector<la_Vertex2d*> &dft);
vector<la_Vertex2d*> _net_preprocessing(la_Net2d net);

float _minimum_via_cost(la_Vertex2d *v, int layer, int minLay, Problem *p, 
char *dir, vector<vector<vector<float>>> supply, vector<vector<vector<float>>> demand);

void _route_print(la_Route r, Problem *p);
void _layer_assignment_and_print_route(Problem *p, vector<Net2D> &net);

int _find_id_in_vector(vector<Pos> v, Pos t)
{
    int ret = -1;
    for(int i = 0; i < v.size(); i++) if(v.at(i).row == t.row && v.at(i).col == t.col) ret = i;
    return ret;
}

vector<la_Net2d*> _transform(vector<Net2D> net)
{
    cout << "start transforming:" << endl;
    vector<la_Net2d*> la_net;
    for(auto a : net)
    {
        cout << a.name << endl;
        la_Net2d* la_net2d = new la_Net2d;
        a.name.erase(0,1);
        la_net2d->id = stoi(a.name, nullptr, 10);
        la_net2d->minRouteLayConst = a.minRouteLayConst;
        la_net2d->weight = a.weight;
        bool first = true;
        for(auto r : a.route2Ds)
        {
            int layer;
            la_Vertex2d *v = new la_Vertex2d;
            la_Vertex2d *v_pre = new la_Vertex2d;
            la_Edge *e = new la_Edge;
            if(first)
            {
                layer = _find_id_in_vector(a.pin2Ds, r.sIdx);
                if(layer == -1) v->init(r.sIdx.row, r.sIdx.col, -1, false); 
                else v->init(r.sIdx.row, r.sIdx.col, a.pinwithLay.at(layer).lay, true); 
                la_net2d->vertex.push_back(v);
                first = false;
            }
            if(r.sIdx.row == r.eIdx.row)
            {
                layer = _find_id_in_vector(a.pin2Ds, r.sIdx);
                if(layer == -1) v_pre->init(r.sIdx.row, r.sIdx.col, -1, false); 
                else v_pre->init(r.sIdx.row, r.sIdx.col, a.pinwithLay.at(layer).lay, true); 
                for(int i = r.sIdx.col+1; i <= r.eIdx.col; i++) 
                {
                    layer = _find_id_in_vector(a.pin2Ds, r.sIdx);
                    if(layer == -1) v->init(r.sIdx.row, r.sIdx.col, -1, false); 
                    else v->init(r.sIdx.row, r.sIdx.col, a.pinwithLay.at(layer).lay, true); 
                    la_net2d->vertex.push_back(v);
                    e->fir = v_pre; e->sec = v;
                    la_net2d->edge.push_back(e);
                    e->print();
                    v_pre = v;
                }
            }
            else
            {
                layer = _find_id_in_vector(a.pin2Ds, r.sIdx);
                if(layer == -1) v_pre->init(r.sIdx.row, r.sIdx.col, -1, false); 
                else v_pre->init(r.sIdx.row, r.sIdx.col, a.pinwithLay.at(layer).lay, true); 
                for(int i = r.sIdx.row+1; i <= r.eIdx.row; i++) 
                {
                    layer = _find_id_in_vector(a.pin2Ds, r.sIdx);
                    if(layer == -1) v->init(r.sIdx.row, r.sIdx.col, -1, false); 
                    else v->init(r.sIdx.row, r.sIdx.col, a.pinwithLay.at(layer).lay, true);  
                    la_net2d->vertex.push_back(v);
                    e->fir = v_pre; e->sec = v;
                    la_net2d->edge.push_back(e);
                    e->print();
                    v_pre = v;
                }
            }
        }
        la_net.push_back(la_net2d);
    }
    return la_net;
}

void _quicksort(vector<la_Net2d*> net, vector<double> score, int l, int r)
{
    if(l >= r) return;
    int i = l-1;
    for (int j = l; j < r; j++) 
    {
        if (net.at(j)->weight > net.at(r)->weight || 
           (net.at(j)->weight == net.at(r)->weight && score.at(j) > score.at(r))) 
        {
            i++;
            swap(score.at(i), score.at(j));
            swap(net.at(i), net.at(j));
        }
    }
    i++;
    swap(score.at(i), score.at(r));
    swap(net.at(i), net.at(r));
    _quicksort(net, score, l, i-1);
    _quicksort(net, score, i+1, r);
}

//determine the net order
void _net_ordering(vector<la_Net2d*> net2d)
{
    vector<double> score;
    for(auto a : net2d) score.push_back(a->_score());
    _quicksort(net2d, score, 0, net2d.size()-1);
}

void _DFS_visit(la_Vertex2d *u, vector<la_Vertex2d*> dft)
{
    u->color = 1;
    dft.push_back(u);
    for(auto v : u->adjlist)
    {
        if(v->color == 0)
        {
            v->parent = u;
            v->parent_edge->fir = u;
            v->parent_edge->sec = v;
            u->child.push_back(v);
            _DFS_visit(v,dft);
        }
    }
}

//transform the topology of a net into a tree
vector<la_Vertex2d*> _net_preprocessing(la_Net2d *net)
{
    //make adjacency list
    for(auto a : net->edge)
    {
        a->fir->adjlist.push_back(a->sec);
        a->sec->adjlist.push_back(a->fir);
    }

    //DFS
    vector<la_Vertex2d*> dft;
    for(auto a : net->vertex) if(a->is_pin){ _DFS_visit(a,dft); break; } 
    return dft;
}

int _minimum_via_cost(la_Vertex2d *v, int layer, int minRouteLayConst, Problem p, 
char *dir, float ***supply, float ***demand)
{
    if(layer < minRouteLayConst) return INT_MAX;
    if(supply[v->row+1][v->col+1][layer+1] == demand[v->row+1][v->col+1][layer+1]) return INT_MAX;
    int mvc = INT_MAX;
    for(int i = 0; i < pow(v->child.size(), p.NumLayer); i++)
    {
        int c = 0;
        vector<int> vc, child_layer;
        vc.push_back(v->pin_layer);
        for(int j = 0; j < v->child.size(); j++)
        {
            int l = i/pow(v->child.size(), j);
            if(v->child.at(j)->parent_edge->_dir() != dir[l]){ c = -1; break; }
            c += _minimum_via_cost(v->child.at(j), l, minRouteLayConst, p, dir, supply, demand)
            *p.layers.at(layer).powerfac;
            vc.push_back(l);
            child_layer.push_back(l);
        }
        if(c == -1) continue;
        c += *max_element(vc.begin(), vc.end())-*min_element(vc.begin(), vc.end());
        if(c < mvc){ mvc = c; v->child_layer.at(layer) = child_layer; }
    }
    return mvc;
}

void _route_print(la_Route *r, Problem p)
{
    printf("NumRoutes %d\n", p.NumRoute);
    for(int i = 0; i < r->nets.size(); i++)
    {
        for(auto w : r->nets.at(i)->wires)
        {
            printf("%d %d %d %d %d %d N%d\n", w->s_row+1, w->s_col+1, w->s_layer+1, 
            w->e_row+1, w->e_col+1, w->e_layer+1, i+1);
        }
    }
}

void _layer_assignment_and_print_route(Problem p, vector<Net2D> net)
{
    vector<la_Net2d*> net2d;
    net2d = _transform(net);
    _net_ordering(net2d);

    //count supply and demand for each grid
    int numrow = p.GGridBD[2]-p.GGridBD[0]+1, numcol = p.GGridBD[3]-p.GGridBD[1]+1;
    float ***supply = new float**[numrow];
    float ***demand = new float**[numrow];
    for(int i = 0; i < numrow; i++)
    {
        supply[i] = new float*[numcol];
        demand[i] = new float*[numcol];
        for(int j = 0; j < numcol; j++)
        {
            supply[i][j] = new float[p.NumLayer];
            demand[i][j] = new float[p.NumLayer];
            for(int k = 0; k < p.NumLayer; k++)
            {
                supply[i][j][k] = 0;
                demand[i][j][k] = 0;
            }
        }
    }
    for(auto a : p.layers)
    {
        for(int i = 0; i < numrow; i++)
        {
            for(int j = 0; j < numcol; j++) supply[i][j][a.Idx-1] += a.defaultsupply;
        }
    }
    for(auto a : p.NonDefaultSupplies) supply[a.rowIdx-1][a.colIdx-1][a.LayIdx-1] += a.relatedValue;
    for(auto a : p.masterCells)
    {
        for(int k = 0; k < a.blockageCount; k++)
        {
            for(int i = 0; i < numrow; i++)
            {
                for(int j = 0; j < numcol; j++) supply[i][j][a.blkgs.at(k).layer_id-1] -= a.blkgs.at(k).demand;
            }
        }
    } 

    // direction of layer
    char *dir = new char[p.NumLayer];
    for(int i = 0; i < p.NumLayer; i++) dir[i] = p.layers.at(i).direction;

    //assignment
    la_Route *r = new la_Route;
    vector<la_Vertex2d*> dft;
    for(auto a : net2d)
    {
        la_Net3d *net_3d = new la_Net3d;
        dft = _net_preprocessing(a);

        //assignment root pin 
        int mvc = INT_MAX;
        la_Vertex2d *root = dft.at(0);
        root->layer = a->minRouteLayConst;
        for(int i = a->minRouteLayConst; i < p.NumLayer; i++)
        {
            int _mvc = _minimum_via_cost(root, i, a->minRouteLayConst, p, dir, supply, demand);
            if(mvc > _mvc){ mvc = _mvc; root->layer = i; }
        }
        demand[root->row][root->col][root->layer]++;
        dft.erase(dft.begin());
        
        //assignment other vertex
        for(auto a : dft)
        {
            int c_id;
            for(c_id = 0; c_id < a->parent->child.size(); c_id++)
            {
                if(a == a->parent->child.at(c_id)) break;
            }  
            a->layer = a->parent->child_layer.at(a->layer).at(c_id);
            demand[a->row][a->col][a->layer]++;
            la_Wire *w = new la_Wire;
            w->_make_wire(a->parent, a);
            net_3d->wires.push_back(w);
        }
        r->nets.push_back(net_3d);
    }

    // print route
    _route_print(r, p);
}

