#ifndef LAYERASSIGNMENT_H
#define LAYERASSIGNMENT_H

#include "problem.h"
#include "structures.h"
#include <bits/stdc++.h>
#define _INF 1<<24
using namespace std;

typedef struct vertex2d la_Vertex2d;
typedef struct edge la_Edge;
typedef struct net2d la_Net2d;
typedef struct wire la_Wire;
typedef struct net3d la_Net3d;
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

    void _print(){ printf("(%d,%d) ", row, col); }
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
struct net3d{ vector<la_Wire> wires; int id; };
struct route{ vector<la_Net3d> nets; };

void _set_vertex(la_Vertex2d *v, int r, int c, int p, bool i, Problem *pro);
int _find_id_in_vector(vector<Pos> v, Pos t);
la_Vertex2d *_find_vertex(vector<la_Vertex2d*> vs, int row, int col);

vector<la_Net2d> _transform(vector<Net2D> net, Problem *p);

void _quicksort(vector<la_Net2d> &net, vector<double> &score, int l, int r);
void _net_ordering(vector<la_Net2d> &net2d);

void _DFS_visit(la_Vertex2d *u, vector<la_Vertex2d*> &dft);
vector<la_Vertex2d*> _net_preprocessing(la_Net2d net);

int _pow(int x, int y);
float _minimum_via_cost(la_Vertex2d *v, int layer, int minLay, Problem *p, 
char *dir, vector<vector<vector<float>>> supply, vector<vector<vector<float>>> demand);

void _quicksort_reordering(vector<la_Net2d> &net, int l, int r);
void _route_print(la_Route r, Problem *p);
void _layer_assignment_and_print_route(Problem *p, vector<Net2D> &net);

#endif

