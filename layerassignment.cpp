#include "layerassignment.h"
#include <bits/stdc++.h>
using namespace std;

void _set_vertex(la_Vertex2d *v, int r, int c, vector<int> p, Problem *pro) {
  v->row = r;
  v->col = c;
  v->layer = -1;
  v->color = 0;
  v->pin_layer = p;
  v->parent = new la_Vertex2d;
  v->parent_edge = new la_Edge;
  v->child_layer.resize(pro->NumLayer + 1);
  if (p.size() == 0)
    v->is_pin = false;
  else
    v->is_pin = true;
}

vector<int> _find_id_in_vector(vector<Pin> p, int r, int c) {
  vector<int> ret;
  for (auto v : p) {
    if (v.locate.row == r && v.locate.col == c)
      ret.push_back(v.lay);
  }
  return ret;
}

la_Vertex2d *_find_vertex(vector<la_Vertex2d *> vs, int row, int col) {
  for (auto v : vs)
    if (v->row == row && v->col == col)
      return v;
  return NULL;
}

vector<la_Net2d> _transform(vector<Net2D> net, Problem *p) {
  vector<la_Net2d> la_net;
  for (auto a : net) {
    la_Net2d la_net2d;
    la_Vertex2d *find = new la_Vertex2d;
    vector<int> layer;
    a.name.erase(0, 1);
    la_net2d.id = stoi(a.name, nullptr, 10);
    la_net2d.minRouteLayConst = a.minRouteLay;
    la_net2d.weight = a.weight;
    bool first = true;
    for (auto r : a.route2Ds) {
      if (r.sIdx.row == r.eIdx.row) {
        la_Vertex2d *v_pre = new la_Vertex2d;
        layer = _find_id_in_vector(a.pinWithLay, r.sIdx.row, r.sIdx.col);
        find = _find_vertex(la_net2d.vertex, r.sIdx.row, r.sIdx.col);
        if (find == NULL) {
          _set_vertex(v_pre, r.sIdx.row, r.sIdx.col, layer, p);
          la_net2d.vertex.push_back(v_pre);
        } else
          v_pre = find;
        if (r.sIdx.col < r.eIdx.col) {
          for (int i = r.sIdx.col + 1; i < r.eIdx.col; i++) {
            la_Vertex2d *v = new la_Vertex2d;
            layer = _find_id_in_vector(a.pinWithLay, r.sIdx.row, i);
            find = _find_vertex(la_net2d.vertex, r.sIdx.row, i);
            if (find == NULL) {
              _set_vertex(v, r.sIdx.row, i, layer, p);
              la_net2d.vertex.push_back(v);
            } else
              v = find;
            la_Edge e;
            e._make_edge(v_pre, v);
            la_net2d.edge.push_back(e);
            v_pre = v;
          }
        } else {
          for (int i = r.sIdx.col - 1; i > r.eIdx.col; i--) {
            la_Vertex2d *v = new la_Vertex2d;
            layer = _find_id_in_vector(a.pinWithLay, r.sIdx.row, i);
            find = _find_vertex(la_net2d.vertex, r.sIdx.row, i);
            if (find == NULL) {
              _set_vertex(v, r.sIdx.row, i, layer, p);
              la_net2d.vertex.push_back(v);
            } else
              v = find;
            la_Edge e;
            e._make_edge(v_pre, v);
            la_net2d.edge.push_back(e);
            v_pre = v;
          }
        }
        la_Vertex2d *v = new la_Vertex2d;
        layer = _find_id_in_vector(a.pinWithLay, r.eIdx.row, r.eIdx.col);
        find = _find_vertex(la_net2d.vertex, r.eIdx.row, r.eIdx.col);
        if (find == NULL) {
          _set_vertex(v, r.eIdx.row, r.eIdx.col, layer, p);
          la_net2d.vertex.push_back(v);
        } else
          v = find;
        la_Edge e;
        e._make_edge(v_pre, v);
        la_net2d.edge.push_back(e);
      } else {
        la_Vertex2d *v_pre = new la_Vertex2d;
        layer = _find_id_in_vector(a.pinWithLay, r.sIdx.row, r.sIdx.col);
        find = _find_vertex(la_net2d.vertex, r.sIdx.row, r.sIdx.col);
        if (find == NULL) {
          _set_vertex(v_pre, r.sIdx.row, r.sIdx.col, layer, p);
          la_net2d.vertex.push_back(v_pre);
        } else
          v_pre = find;
        if (r.sIdx.row < r.eIdx.row) {
          for (int i = r.sIdx.row + 1; i < r.eIdx.row; i++) {
            la_Vertex2d *v = new la_Vertex2d;
            layer = _find_id_in_vector(a.pinWithLay, i, r.sIdx.col);
            find = _find_vertex(la_net2d.vertex, i, r.sIdx.col);
            if (find == NULL) {
              _set_vertex(v, i, r.sIdx.col, layer, p);
              la_net2d.vertex.push_back(v);
            } else
              v = find;
            la_Edge e;
            e._make_edge(v_pre, v);
            la_net2d.edge.push_back(e);
            v_pre = v;
          }
        } else {
          for (int i = r.sIdx.row - 1; i > r.eIdx.row; i--) {
            la_Vertex2d *v = new la_Vertex2d;
            layer = _find_id_in_vector(a.pinWithLay, i, r.sIdx.col);
            find = _find_vertex(la_net2d.vertex, i, r.sIdx.col);
            if (find == NULL) {
              _set_vertex(v, i, r.sIdx.col, layer, p);
              la_net2d.vertex.push_back(v);
            } else
              v = find;
            la_Edge e;
            e._make_edge(v_pre, v);
            la_net2d.edge.push_back(e);
            v_pre = v;
          }
        }
        la_Vertex2d *v = new la_Vertex2d;
        layer = _find_id_in_vector(a.pinWithLay, r.eIdx.row, r.eIdx.col);
        find = _find_vertex(la_net2d.vertex, r.eIdx.row, r.eIdx.col);
        if (find == NULL) {
          _set_vertex(v, r.eIdx.row, r.eIdx.col, layer, p);
          la_net2d.vertex.push_back(v);
        } else
          v = find;
        la_Edge e;
        e._make_edge(v_pre, v);
        la_net2d.edge.push_back(e);
      }
    }
    la_net.push_back(la_net2d);
  }
  return la_net;
}

void _quicksort(vector<la_Net2d> &net, vector<double> &score, int l, int r) {
  if (l >= r)
    return;
  int i = l - 1;
  for (int j = l; j < r; j++) {
    if (net.at(j).weight > net.at(r).weight ||
        (net.at(j).weight == net.at(r).weight && score.at(j) > score.at(r))) {
      i++;
      swap(score.at(i), score.at(j));
      swap(net.at(i), net.at(j));
    }
  }
  i++;
  swap(score.at(i), score.at(r));
  swap(net.at(i), net.at(r));
  _quicksort(net, score, l, i - 1);
  _quicksort(net, score, i + 1, r);
}

// determine the net order
void _net_ordering(vector<la_Net2d> &net2d) {
  vector<double> score;
  for (auto a : net2d)
    score.push_back(a._score());
  _quicksort(net2d, score, 0, net2d.size() - 1);
}

void _DFS_visit(la_Vertex2d *u, vector<la_Vertex2d *> &dft) {
  u->color = 1;
  dft.push_back(u);
  for (auto v : u->adjlist) {
    if (v->color == 0) {
      v->parent = u;
      v->parent_edge->_make_edge(u, v);
      u->child.push_back(v);
      _DFS_visit(v, dft);
    }
  }
}

// transform the topology of a net into a tree
vector<la_Vertex2d *> _net_preprocessing(la_Net2d net) {
  // make adjacency list
  for (auto a : net.edge) {
    a.fir->adjlist.push_back(a.sec);
    a.sec->adjlist.push_back(a.fir);
  }
  // DFS
  vector<la_Vertex2d *> dft;
  for (auto a : net.vertex)
    if (a->is_pin) {
      _DFS_visit(a, dft);
      break;
    }
  return dft;
}

int _pow(int x, int y) {
  if (y == 0)
    return 1;
  else
    return x * _pow(x, y - 1);
}

float _minimum_via_cost(la_Vertex2d *v, int layer, int minLay, Problem *p,
                        char *dir, vector<vector<vector<float>>> supply,
                        vector<vector<vector<float>>> demand) {
  // v->_print();printf("\n");
  if (layer < minLay)
    return _INF;
  if (supply.at(v->row).at(v->col).at(layer) ==
      demand.at(v->row).at(v->col).at(layer))
    return _INF;
  if (v->child.size() == 0)
    return 1;
  float mvc = _INF, c;
  vector<int> vc, child_layer;
  for (int i = 0; i < pow(p->NumLayer, v->child.size()); i++) {
    c = 0.0;
    vc.clear();
    child_layer.clear();
    vc = v->pin_layer;
    vc.push_back(layer);
    for (int j = 0; j < v->child.size(); j++) {
      int l = (i % _pow(p->NumLayer, j + 1)) / _pow(p->NumLayer, j);
      if (v->child.at(j)->parent_edge->_dir() != dir[l]) {
        c = -1;
        break;
      }
      c += _minimum_via_cost(v->child.at(j), l + 1, minLay, p, dir, supply,
                             demand) *
           p->layers.at(l).powerfac;
      vc.push_back(l + 1);
      child_layer.push_back(l + 1);
    }
    if (c == -1)
      continue;
    c += (*max_element(vc.begin(), vc.end()) -
          *min_element(vc.begin(), vc.end())) *
         2.0;
    if (c < mvc) {
      mvc = c;
      v->child_layer.at(layer) = child_layer;
    }
  }
  return mvc;
}

void _quicksort_id(vector<la_Net3d> &net, int l, int r) {
  if (l >= r)
    return;
  int i = l - 1;
  for (int j = l; j < r; j++) {
    if (net.at(j).id < net.at(r).id) {
      i++;
      swap(net.at(i), net.at(j));
    }
  }
  i++;
  swap(net.at(i), net.at(r));
  _quicksort_id(net, l, i - 1);
  _quicksort_id(net, i + 1, r);
}

void _route_print(la_Route *r, Problem *p) {
  ofstream outputfile;
  outputfile.open("output.txt");
  int n_route = 0;
  for (auto n : r->nets)
    n_route += n.wires.size();
  printf("NumMovedCellInst 0\nNumRoutes %d\n", n_route);
  outputfile << "NumMovedCellInst 0\nNumRoutes " << n_route << "\n";
  for (auto n : r->nets) {
    for (auto w : n.wires) {
      w._print();
      printf("N%d\n", n.id);
      outputfile << w.s_row << " " << w.s_col << " " << w.s_layer << " "
                 << w.e_row << " " << w.e_col << " " << w.e_layer << " ";
      outputfile << "N" << n.id << "\n";
    }
  }
  outputfile.close();
}

bool _is_overlap(la_Wire a, la_Wire b) {
  if (a.s_col == b.s_col && a.e_col == b.e_col && a.s_layer == b.s_layer &&
      a.e_layer == b.e_layer) {
    return (a.s_row <= b.s_row && a.s_row <= b.e_row && a.e_row >= b.s_row &&
            a.e_row >= b.e_row) ||
           (a.e_row <= b.s_row && a.e_row <= b.e_row && a.s_row >= b.s_row &&
            a.s_row >= b.e_row);
  }
  if (a.s_row == b.s_row && a.e_row == b.e_row && a.s_layer == b.s_layer &&
      a.e_layer == b.e_layer) {
    return (a.s_col <= b.s_col && a.s_col <= b.e_col && a.e_col >= b.s_col &&
            a.e_col >= b.e_col) ||
           (a.e_col <= b.s_col && a.e_col <= b.e_col && a.s_col >= b.s_col &&
            a.s_col >= b.e_col);
  }
  if (a.s_row == b.s_row && a.e_row == b.e_row && a.s_col == b.s_col &&
      a.e_col == b.e_col) {
    return (a.s_layer <= b.s_layer && a.s_layer <= b.e_layer &&
            a.e_layer >= b.s_layer && a.e_layer >= b.e_layer) ||
           (a.e_layer <= b.s_layer && a.e_layer <= b.e_layer &&
            a.s_layer >= b.s_layer && a.s_layer >= b.e_layer);
  }
  if (a.s_col == b.e_col && a.e_col == b.s_col && a.s_layer == b.e_layer &&
      a.e_layer == b.s_layer) {
    return (a.s_row <= b.s_row && a.s_row <= b.e_row && a.e_row >= b.s_row &&
            a.e_row >= b.e_row) ||
           (a.e_row <= b.s_row && a.e_row <= b.e_row && a.s_row >= b.s_row &&
            a.s_row >= b.e_row);
  }
  if (a.s_row == b.e_row && a.e_row == b.s_row && a.s_layer == b.e_layer &&
      a.e_layer == b.s_layer) {
    return (a.s_col <= b.s_col && a.s_col <= b.e_col && a.e_col >= b.s_col &&
            a.e_col >= b.e_col) ||
           (a.e_col <= b.s_col && a.e_col <= b.e_col && a.s_col >= b.s_col &&
            a.s_col >= b.e_col);
  }
  if (a.s_row == b.e_row && a.e_row == b.s_row && a.s_col == b.e_col &&
      a.e_col == b.s_col) {
    return (a.s_layer <= b.s_layer && a.s_layer <= b.e_layer &&
            a.e_layer >= b.s_layer && a.e_layer >= b.e_layer) ||
           (a.e_layer <= b.s_layer && a.e_layer <= b.e_layer &&
            a.s_layer >= b.s_layer && a.s_layer >= b.e_layer);
  }
  return false;
}

void _wire_compress(vector<la_Wire> &w) {
  for (int i = 0; i < w.size() - 1; i++) {
    for (int j = i + 1; j < w.size(); j++) {
      if (_is_overlap(w.at(i), w.at(j)))
        w.erase(w.begin() + j);
      else if (_is_overlap(w.at(j), w.at(i)))
        w.erase(w.begin() + i);
    }
  }
}

la_Route *_layer_assignment(Problem *p, vector<Net2D> &net) {
  vector<la_Net2d> net2d;
  net2d = _transform(net, p);
  _net_ordering(net2d);

  // count supply and demand for each grid
  int numrow = p->GGridBD[2] - p->GGridBD[0] + 1,
      numcol = p->GGridBD[3] - p->GGridBD[1] + 1;
  vector<vector<vector<float>>> supply(
      numrow + 1,
      vector<vector<float>>(numcol + 1, vector<float>(p->NumLayer + 1)));
  vector<vector<vector<float>>> demand(
      numrow + 1,
      vector<vector<float>>(numcol + 1, vector<float>(p->NumLayer + 1)));
  for (int i = 0; i <= numrow; i++) {
    for (int j = 0; j <= numcol; j++) {
      for (int k = 0; k <= p->NumLayer; k++)
        supply.at(i).at(j).at(k) = 0.0;
    }
  }
  for (auto a : p->layers) {
    for (int i = 0; i <= numrow; i++) {
      for (int j = 0; j <= numcol; j++)
        supply.at(i).at(j).at(a.Idx) += (float)a.defaultsupply;
    }
  }
  for (auto a : p->NonDefaultSupplies)
    supply.at(a.rowIdx).at(a.colIdx).at(a.LayIdx) += (float)a.relatedValue;
  for (auto a : p->masterCells) {
    for (int k = 0; k < a.blockageCount; k++) {
      for (int i = 0; i <= numrow; i++) {
        for (int j = 0; j <= numcol; j++)
          supply.at(i).at(j).at(a.blkgs.at(k).layer_id) -=
              (float)a.blkgs.at(k).demand;
      }
    }
  }

  // direction of layer
  char *dir = new char[p->NumLayer];
  for (int i = 0; i < p->NumLayer; i++)
    dir[i] = p->layers.at(i).direction;

  // assignment
  la_Route *r = new la_Route;
  vector<la_Vertex2d *> dft;
  for (auto a : net2d) {
    dft = _net_preprocessing(a);
    // assignment root pin
    int mvc = _INF;
    la_Net3d net_3d;
    net_3d.id = a.id;
    la_Vertex2d *root = dft.at(0);
    root->layer = a.minRouteLayConst;
    for (int i = a.minRouteLayConst; i <= p->NumLayer; i++) {
      int _mvc = _minimum_via_cost(root, i, a.minRouteLayConst, p, dir, supply,
                                   demand);
      if (mvc > _mvc) {
        mvc = _mvc;
        root->layer = i;
      }
    }
    demand.at(root->row).at(root->col).at(root->layer) += 1.0;
    if (root->is_pin && root->pin_layer.size() == 1 &&
        root->layer != root->pin_layer.at(0)) {
      la_Wire w;
      w._make_wire(*root, root->layer, *root, root->pin_layer.at(0));
      net_3d.wires.push_back(w);
      if (root->layer < root->pin_layer.at(0)) {
        for (int i = root->layer + 1; i <= root->pin_layer.at(0); i++) {
          demand.at(root->row).at(root->col).at(i) += 1.0;
        }
      } else {
        for (int i = root->layer - 1; i >= root->pin_layer.at(0); i--) {
          demand.at(root->row).at(root->col).at(i) += 1.0;
        }
      }
    }
    if (root->is_pin && root->pin_layer.size() > 1) {
      int max = *max_element(root->pin_layer.begin(), root->pin_layer.end());
      int min = *min_element(root->pin_layer.begin(), root->pin_layer.end());
      if (max != root->layer) {
        la_Wire w;
        w._make_wire(*root, root->layer, *root, max);
        net_3d.wires.push_back(w);
        if (root->layer < max) {
          for (int i = root->layer + 1; i <= max; i++) {
            demand.at(root->row).at(root->col).at(i) += 1.0;
          }
        } else {
          for (int i = root->layer - 1; i >= max; i--) {
            demand.at(root->row).at(root->col).at(i) += 1.0;
          }
        }
      }
      if (min != root->layer) {
        la_Wire w;
        w._make_wire(*root, root->layer, *root, min);
        net_3d.wires.push_back(w);
        if (root->layer < min) {
          for (int i = root->layer + 1; i <= min; i++) {
            demand.at(root->row).at(root->col).at(i) += 1.0;
          }
        } else {
          for (int i = root->layer - 1; i >= min; i--) {
            demand.at(root->row).at(root->col).at(i) += 1.0;
          }
        }
      }
    }
    dft.erase(dft.begin());

    // assignment other vertex
    for (auto a : dft) {
      int c_id;
      for (c_id = 0; c_id < a->parent->child.size(); c_id++) {
        if (a == a->parent->child.at(c_id))
          break;
      }
      a->layer = a->parent->child_layer.at(a->parent->layer).at(c_id);
      demand.at(a->row).at(a->col).at(a->layer) += 1.0;
      if (a->parent->layer == a->layer) {
        la_Wire w;
        w._make_wire(*a->parent, a->parent->layer, *a, a->layer);
        net_3d.wires.push_back(w);
      } else {
        la_Wire via, w;
        via._make_wire(*a->parent, a->parent->layer, *a->parent, a->layer);
        w._make_wire(*a->parent, a->layer, *a, a->layer);
        net_3d.wires.push_back(via);
        net_3d.wires.push_back(w);
        demand.at(a->parent->row).at(a->parent->col).at(a->layer) += 1.0;
      }
      if (a->is_pin && a->pin_layer.size() == 1 &&
          a->layer != a->pin_layer.at(0)) {
        la_Wire w;
        w._make_wire(*a, a->layer, *a, a->pin_layer.at(0));
        net_3d.wires.push_back(w);
        if (a->layer < a->pin_layer.at(0)) {
          for (int i = a->layer + 1; i <= a->pin_layer.at(0); i++) {
            demand.at(a->row).at(a->col).at(i) += 1.0;
          }
        } else {
          for (int i = a->layer - 1; i >= a->pin_layer.at(0); i--) {
            demand.at(a->row).at(a->col).at(i) += 1.0;
          }
        }
      }
      if (a->is_pin && a->pin_layer.size() > 1) {
        int max = *max_element(a->pin_layer.begin(), a->pin_layer.end());
        int min = *min_element(a->pin_layer.begin(), a->pin_layer.end());
        if (max != a->layer) {
          la_Wire w;
          w._make_wire(*a, a->layer, *a, max);
          net_3d.wires.push_back(w);
          if (a->layer < max) {
            for (int i = a->layer + 1; i <= max; i++) {
              demand.at(a->row).at(a->col).at(i) += 1.0;
            }
          } else {
            for (int i = a->layer - 1; i >= max; i--) {
              demand.at(a->row).at(a->col).at(i) += 1.0;
            }
          }
        }
        if (min != a->layer) {
          la_Wire w;
          w._make_wire(*a, a->layer, *a, min);
          net_3d.wires.push_back(w);
          if (a->layer < min) {
            for (int i = a->layer + 1; i <= min; i++) {
              demand.at(a->row).at(a->col).at(i) += 1.0;
            }
          } else {
            for (int i = a->layer - 1; i >= min; i--) {
              demand.at(a->row).at(a->col).at(i) += 1.0;
            }
          }
        }
      }
    }
    _wire_compress(net_3d.wires);
    r->nets.push_back(net_3d);
  }
  _quicksort_id(r->nets, 0, r->nets.size() - 1);
  return r;
}

double _route_score(Problem *p, la_Route *r) {
  double score = 0.0;
  int numrow = p->GGridBD[2] - p->GGridBD[0] + 1,
      numcol = p->GGridBD[3] - p->GGridBD[1] + 1;
  vector<vector<vector<bool>>> is_use(
      numrow + 1,
      vector<vector<bool>>(numcol + 1, vector<bool>(p->NumLayer + 1)));
  for (auto n : r->nets) {
    for (int i = 1; i <= numrow; i++) {
      for (int j = 1; j <= numcol; j++) {
        for (int k = 1; k <= p->NumLayer; k++)
          is_use.at(i).at(j).at(k) = false;
      }
    }
    for (auto w : n.wires) {
      if (w.s_row < w.e_row) {
        for (int i = w.s_row; i <= w.e_row; i++)
          is_use.at(i).at(w.s_col).at(w.s_layer) = true;
      } else if (w.s_row > w.e_row) {
        for (int i = w.e_row; i <= w.s_row; i++)
          is_use.at(i).at(w.s_col).at(w.s_layer) = true;
      } else if (w.s_col < w.e_col) {
        for (int i = w.s_col; i <= w.e_col; i++)
          is_use.at(w.s_row).at(i).at(w.s_layer) = true;
      } else if (w.s_col > w.e_col) {
        for (int i = w.e_col; i <= w.s_col; i++)
          is_use.at(w.s_row).at(i).at(w.s_layer) = true;
      } else if (w.s_layer < w.e_layer) {
        for (int i = w.s_layer; i <= w.e_layer; i++)
          is_use.at(w.s_row).at(w.s_col).at(i) = true;
      } else if (w.s_layer > w.e_layer) {
        for (int i = w.e_layer; i <= w.s_layer; i++)
          is_use.at(w.s_row).at(w.s_col).at(i) = true;
      }
    }
    for (int k = 1; k <= p->NumLayer; k++) {
      for (int i = 1; i <= numrow; i++) {
        for (int j = 1; j <= numcol; j++) {
          if (is_use.at(i).at(j).at(k))
            score += (double)p->layers.at(k - 1).powerfac *
                     (double)p->nets.at(n.id - 1).weight;
        }
      }
    }
  }
  return score;
}
