#ifndef LAYERASSIGNMENT_H
#define LAYERASSIGNMENT_H

#include "problem.h"
#include <bits/stdc++.h>
#define _INF 1<<24
using namespace std;

// 2D 格子點
typedef struct vertex2d la_Vertex2d;
// edge
typedef struct edge la_Edge;
// 2D net from previous process
typedef struct net2d la_Net2d;
// 3D wire
typedef struct wire la_Wire;
// 3D net for output
typedef struct net3d la_Net3d;
// final route
typedef struct route la_Route;

struct vertex2d
{ 
    int row, col, layer, color, pin_layer; 
    bool is_pin; 
    vector<la_Vertex2d*> adjlist;
    la_Vertex2d *parent;
    vector<la_Vertex2d*> child;
    la_Edge *parent_edge;
    vector<vector<int>> child_layer;

    la_Vertex2d operator=(la_Vertex2d a)
    {
        row = a.row; col = a.col; layer = a.layer; color = a.color; 
        pin_layer = a.pin_layer; is_pin = a.is_pin;
        adjlist = a.adjlist; parent = a.parent; child = a.child; 
        parent_edge = a.parent_edge; child_layer = a.child_layer;
        return a;
    }

    bool operator==(la_Vertex2d a)
    {
        if(row == a.row && col == a.col && layer == a.layer && color == a.color && 
        pin_layer == a.pin_layer && is_pin == a.is_pin) return true;
        return false;
    }
};

struct edge
{ 
    la_Vertex2d *fir, *sec; 

    char _dir(){ return (fir->row == sec->row)?'H':'V'; }
    void _make_edge(la_Vertex2d *x, la_Vertex2d *y){ fir = x; sec = y; }
    la_Edge operator=(la_Edge a){ fir = a.fir; sec = a.sec; return a; }
};

struct net2d
{
    int id;
    vector<la_Vertex2d*> vertex; 
    vector<la_Edge> edge; 
    double weight;
    int minRouteLayConst;

    double _score()
    {
        int pinnum = 0;
        for(auto a : vertex) if(a->is_pin) pinnum++;
        return 1/(double)edge.size()+pinnum;
    }
};

struct wire
{ 
    int s_row, s_col, s_layer, e_row, e_col, e_layer; 
    void _make_wire(la_Vertex2d x, int x_layer, la_Vertex2d y, int y_layer)
    {
        s_row = x.row; s_col = x.col; s_layer = x_layer;
        e_row = y.row; e_col = y.col; e_layer = y_layer;
    }
};
struct net3d{ vector<la_Wire> wires; };
struct route{ vector<la_Net3d> nets; };

void _set_vertex(la_Vertex2d *v, int r, int c, int p, bool i, Problem *pro)
{
    v->row = r; v->col = c; v->layer = -1; v->color = 0; v->pin_layer = p; v->is_pin = i;
    v->parent = new la_Vertex2d; v->parent_edge = new la_Edge; v->child_layer.resize(pro->NumLayer+1);
}

int _find_id_in_vector(vector<Pos> v, Pos t)
{
    int ret = -1;
    for(int i = 0; i < v.size(); i++) if(v.at(i).row == t.row && v.at(i).col == t.col) ret = i;
    return ret;
}

la_Vertex2d *_find_vertex(vector<la_Vertex2d*> vs, int row, int col)
{
    for(auto v : vs) if(v->row == row && v->col == col) return v;
    return NULL;
}

vector<la_Net2d> _transform(vector<Net2D> net, Problem *p)
{
    vector<la_Net2d> la_net;
    for(auto a : net)
    {
        la_Net2d la_net2d;
        la_Vertex2d *find = new la_Vertex2d;
        a.name.erase(0,1);
        la_net2d.id = stoi(a.name, nullptr, 10);
        la_net2d.minRouteLayConst = a.minRouteLay;
        la_net2d.weight = a.weight;
        bool first = true;
        for(auto r : a.route2Ds)
        {
            int layer;           
            if(first)
            {
                layer = _find_id_in_vector(a.pin2Ds, r.sIdx);
                la_Vertex2d *v = new la_Vertex2d;
                if(layer == -1) _set_vertex(v, r.sIdx.row, r.sIdx.col, -1, false, p); 
                else _set_vertex(v, r.sIdx.row, r.sIdx.col, a.pinWithLay.at(layer).lay, true, p); 
                la_net2d.vertex.push_back(v);
                first = false;
            }
            if(r.sIdx.row == r.eIdx.row)
            {
                layer = _find_id_in_vector(a.pin2Ds, r.sIdx);
                la_Vertex2d *v_pre = new la_Vertex2d; 
                find = _find_vertex(la_net2d.vertex, r.sIdx.row, r.sIdx.col);
                if(find == NULL)
                {
                    if(layer == -1) _set_vertex(v_pre, r.sIdx.row, r.sIdx.col, -1, false, p); 
                    else _set_vertex(v_pre, r.sIdx.row, r.sIdx.col, a.pinWithLay.at(layer).lay, true, p); 
                    la_net2d.vertex.push_back(v_pre);
                }
                else v_pre = find;
                if(r.sIdx.col < r.eIdx.col)
                {
                    for(int i = r.sIdx.col+1; i < r.eIdx.col; i++) 
                    {
                        la_Vertex2d *v = new la_Vertex2d;
                        find = _find_vertex(la_net2d.vertex, r.sIdx.row, i);
                        if(find == NULL)
                        {
                            _set_vertex(v, r.sIdx.row, i, -1, false, p);
                            la_net2d.vertex.push_back(v);
                        }  
                        else v = find;
                        la_Edge e;
                        e._make_edge(v_pre, v);
                        la_net2d.edge.push_back(e);
                        v_pre = v;
                    }
                }
                else
                {
                    for(int i = r.sIdx.col-1; i > r.eIdx.col; i--) 
                    {
                        la_Vertex2d *v = new la_Vertex2d;
                        find = _find_vertex(la_net2d.vertex, r.sIdx.row, i);
                        if(find == NULL)
                        {
                            _set_vertex(v, r.sIdx.row, i, -1, false, p);
                            la_net2d.vertex.push_back(v);
                        }  
                        else v = find;
                        la_Edge e;
                        e._make_edge(v_pre, v);
                        la_net2d.edge.push_back(e);
                        v_pre = v;
                    }
                }
                layer = _find_id_in_vector(a.pin2Ds, r.eIdx);
                la_Vertex2d *v = new la_Vertex2d;
                find = _find_vertex(la_net2d.vertex, r.eIdx.row, r.eIdx.col);
                if(find == NULL)
                {
                    if(layer == -1) _set_vertex(v, r.eIdx.row, r.eIdx.col, -1, false, p); 
                    else _set_vertex(v, r.eIdx.row, r.eIdx.col, a.pinWithLay.at(layer).lay, true, p);
                    la_net2d.vertex.push_back(v); 
                }
                else v = find;
                la_Edge e;
                e._make_edge(v_pre, v);
                la_net2d.edge.push_back(e);
            }
            else
            {
                layer = _find_id_in_vector(a.pin2Ds, r.sIdx);
                la_Vertex2d *v_pre = new la_Vertex2d; 
                find = _find_vertex(la_net2d.vertex, r.sIdx.row, r.sIdx.col);
                if(find == NULL)
                {
                    if(layer == -1) _set_vertex(v_pre, r.sIdx.row, r.sIdx.col, -1, false, p); 
                    else _set_vertex(v_pre, r.sIdx.row, r.sIdx.col, a.pinWithLay.at(layer).lay, true, p);
                    la_net2d.vertex.push_back(v_pre); 
                }
                else v_pre = find;
                if(r.sIdx.row < r.eIdx.row)
                {
                    for(int i = r.sIdx.row+1; i < r.eIdx.row; i++) 
                    {
                        la_Vertex2d *v = new la_Vertex2d;
                        find = _find_vertex(la_net2d.vertex, i, r.sIdx.col);
                        if(find == NULL)
                        {
                            _set_vertex(v, i, r.sIdx.col, -1, false, p);
                            la_net2d.vertex.push_back(v);
                        }   
                        else v = find;                        
                        la_Edge e;
                        e._make_edge(v_pre, v);
                        la_net2d.edge.push_back(e);
                        v_pre = v;
                    }
                }
                else
                {
                    for(int i = r.sIdx.row-1; i > r.eIdx.row; i--) 
                    {
                        la_Vertex2d *v = new la_Vertex2d;
                        find = _find_vertex(la_net2d.vertex, i, r.sIdx.col);
                        if(find == NULL)
                        {
                            _set_vertex(v, i, r.sIdx.col, -1, false, p); 
                            la_net2d.vertex.push_back(v);
                        }  
                        else v = find;                         
                        la_Edge e;
                        e._make_edge(v_pre, v);
                        la_net2d.edge.push_back(e);
                        v_pre = v;
                    }
                }
                layer = _find_id_in_vector(a.pin2Ds, r.eIdx);
                la_Vertex2d *v = new la_Vertex2d;
                find = _find_vertex(la_net2d.vertex, r.eIdx.row, r.eIdx.col);
                if(find == NULL)
                {
                    if(layer == -1) _set_vertex(v, r.eIdx.row, r.eIdx.col, -1, false, p); 
                    else _set_vertex(v, r.eIdx.row, r.eIdx.col, a.pinWithLay.at(layer).lay, true, p); 
                    la_net2d.vertex.push_back(v); 
                }
                else v = find;
                la_Edge e;
                e._make_edge(v_pre, v);
                la_net2d.edge.push_back(e);
            }
        }
        la_net.push_back(la_net2d);
    }
    return la_net;
}

void _quicksort(vector<la_Net2d> &net, vector<double> &score, int l, int r)
{
    if(l >= r) return;
    int i = l-1;
    for (int j = l; j < r; j++) 
    {
        if (net.at(j).weight > net.at(r).weight || 
           (net.at(j).weight == net.at(r).weight && score.at(j) > score.at(r))) 
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
void _net_ordering(vector<la_Net2d> &net2d)
{
    vector<double> score;
    for(auto a : net2d) score.push_back(a._score());
    _quicksort(net2d, score, 0, net2d.size()-1);
}

void _DFS_visit(la_Vertex2d *u, vector<la_Vertex2d*> &dft)
{
    u->color = 1;
    dft.push_back(u);
    for(auto v : u->adjlist)
    {
        if(v->color == 0)
        {
            v->parent = u;
            v->parent_edge->_make_edge(u,v);
            u->child.push_back(v);
            _DFS_visit(v,dft);
        }
    }
}

//transform the topology of a net into a tree
vector<la_Vertex2d*> _net_preprocessing(la_Net2d net)
{
    //make adjacency list
    for(auto a : net.edge)
    {
        a.fir->adjlist.push_back(a.sec);
        a.sec->adjlist.push_back(a.fir);
    }

    //DFS
    vector<la_Vertex2d*> dft;
    for(auto a : net.vertex) if(a->is_pin){ _DFS_visit(a, dft); break; } 
    return dft;
}

float _minimum_via_cost(la_Vertex2d *v, int layer, int minLay, Problem *p, 
char *dir, vector<vector<vector<float>>> supply, vector<vector<vector<float>>> demand)
{
    if(layer < minLay) return _INF;
    if(supply.at(v->row).at(v->col).at(layer) == demand.at(v->row).at(v->col).at(layer)) return _INF;
    if(v->child.size() == 0) return 1;
    float mvc = _INF, c;
    vector<int> vc, child_layer;
    for(int i = 0; i < pow(p->NumLayer, v->child.size()); i++)
    {
        c = 0.0; vc.clear(); child_layer.clear();
        vc.push_back(v->pin_layer);
        for(int j = 0; j < v->child.size(); j++)
        {
            int l = i/pow(v->child.size(), j);
            if(v->child.at(j)->parent_edge->_dir() != dir[l]){ c = -1; break; }
            c += _minimum_via_cost(v->child.at(j), l+1, minLay, p, dir, supply, demand)*p->layers.at(l).powerfac;
            vc.push_back(l+1);
            child_layer.push_back(l+1);
        }
        if(c == -1) continue; 
        c += *max_element(vc.begin(), vc.end())-*min_element(vc.begin(), vc.end());
        if(c < mvc){ mvc = c; v->child_layer.at(layer) = child_layer; }
    }
    return mvc;
}

void _route_print(la_Route r, Problem *p)
{
    int n_route = 0;
    for(auto n : r.nets) n_route += n.wires.size();
    printf("NumMovedCellInst 0\nNumRoutes %d\n", n_route);
    for(int i = 0; i < r.nets.size(); i++)
    {
        for(auto w : r.nets.at(i).wires)
        {
            printf("%d %d %d %d %d %d N%d\n", w.s_row+1, w.s_col+1, w.s_layer+1, 
            w.e_row+1, w.e_col+1, w.e_layer+1, i+1);
        }
    }
}

void _layer_assignment_and_print_route(Problem *p, vector<Net2D> &net)
{
    vector<la_Net2d> net2d;
    net2d = _transform(net, p);
    _net_ordering(net2d);

    //count supply and demand for each grid
    int numrow = p->GGridBD[2]-p->GGridBD[0]+1, numcol = p->GGridBD[3]-p->GGridBD[1]+1;
    vector<vector<vector<float>>> supply(numrow+1, vector<vector<float>>(numcol+1, vector<float>(p->NumLayer+1)));
    vector<vector<vector<float>>> demand(numrow+1, vector<vector<float>>(numcol+1, vector<float>(p->NumLayer+1)));
    for(int i = 0; i <= numrow; i++)
    {
        for(int j = 0; j <= numcol; j++)
        {
            for(int k = 0; k <= p->NumLayer; k++) supply.at(i).at(j).at(k) = 0.0;
        }
    }
    for(auto a : p->layers)
    {
        for(int i = 0; i <= numrow; i++)
        {
            for(int j = 0; j <= numcol; j++) supply.at(i).at(j).at(a.Idx) += (float)a.defaultsupply;
        }
    }
    for(auto a : p->NonDefaultSupplies) supply.at(a.rowIdx).at(a.colIdx).at(a.LayIdx) += (float)a.relatedValue;
    for(auto a : p->masterCells)
    {
        for(int k = 0; k < a.blockageCount; k++)
        {
            for(int i = 0; i <= numrow; i++)
            {
                for(int j = 0; j <= numcol; j++) supply.at(i).at(j).at(a.blkgs.at(k).layer_id) -= (float)a.blkgs.at(k).demand;
            }
        }
    } 

    
    // direction of layer
    char *dir = new char[p->NumLayer];
    for(int i = 0; i < p->NumLayer; i++) dir[i] = p->layers.at(i).direction;

    //assignment
    la_Route r;
    vector<la_Vertex2d*> dft;
    for(auto a : net2d)
    {
        dft = _net_preprocessing(a);

        //assignment root pin 
        int mvc = _INF;
        la_Net3d net_3d;
        la_Vertex2d *root = dft.at(0);
        root->layer = a.minRouteLayConst;
        for(int i = a.minRouteLayConst; i <= p->NumLayer; i++)
        {
            int _mvc = _minimum_via_cost(root, i, a.minRouteLayConst, p, dir, supply, demand);
            if(mvc > _mvc){ mvc = _mvc; root->layer = i; }
        }
        demand.at(root->row).at(root->col).at(root->layer) += 1.0; 
        dft.erase(dft.begin());
        
        //assignment other vertex
        for(auto a : dft)
        {
            int c_id;
            for(c_id = 0; c_id < a->parent->child.size(); c_id++)
            {
                if(a == a->parent->child.at(c_id)) break;
            }  
            a->layer = a->parent->child_layer.at(a->parent->layer).at(c_id);
            demand.at(a->row).at(a->col).at(a->layer) += 1.0;
            if(a->parent->layer == a->layer)
            {
                la_Wire w;
                w._make_wire(*a->parent, a->parent->layer, *a, a->layer);
                net_3d.wires.push_back(w);
            }
            else
            {
                la_Wire via, w;
                via._make_wire(*a->parent, a->parent->layer, *a->parent, a->layer);
                w._make_wire(*a->parent, a->layer, *a, a->layer);
                net_3d.wires.push_back(via);
                net_3d.wires.push_back(w);
            }
        }
        r.nets.push_back(net_3d);
    }

    // print route
    _route_print(r, p);
}

#endif

