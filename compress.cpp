#include <iostream>
#include <vector>
#include <string>
#include "problem.h"
#include "compress.h"
#include "rsmtAware.h"
#include <algorithm>
#include <math.h>
#include <cstdlib> 
#include <ctime>   
using namespace std;

void PrintPos(Pos a);
void PrintRoute2D(Route2D a);
void PrintNet2D(Net2D a);
void PrintTwoPinNet(TwoPinRoute2D a);
void PrintEdgeSupply(EdgeSupply a);
void PrintGridSupply(GridSupply a);
void PrintBLMRgrid(BLMRgrid a);

bool isPosSame (Pos& a, Pos& b);
bool isPininRoute(Pos& start, Pos& end, Pos& Pin);
bool isLineVertical(Pos& start, Pos& end);
bool isLineConnect(Route2D& a, Route2D& b);

vector<Net2D> Three2Two (Problem* pro);
Route2D mergeLine(Route2D& a, Route2D& b);
void MergeNet (vector<Route2D>& routes);

TwoPinNets CreateNetSet(Problem* pro, string& name);
void changeTwoPinNet(TwoPinNets &netSet, TwoPinRoute2D &newNet);
void changeUsage(vector<vector<GridSupply>>& usage,TwoPinRoute2D &net, bool add = true);

vector<vector<GridSupply>> GenerateGridSupplyGraph(Problem *pro);
vector<vector<EdgeSupply>> Grid2EdgeSupply(vector<vector<GridSupply>>& graph);
void SupplyChange(vector<vector<GridSupply>>& graph, Net2D oldNet, Net2D newNet);
void afterRouting (vector<vector<GridSupply>>& graph, TwoPinRoute2D& net, TwoPinNets& netSet);

TwoPinRoute2D Multi2TwoPinRoute(Net2D& net, Pos sPin, Pos ePin);
vector<Route2D> FindRoute(vector<Route2D> &ref, Pos sPin, Pos ePin);
void FindRouteRaw(vector<Route2D> *route, vector<Route2D> &ref, Pos sPin, Pos ePin);

void SortTaskQueue(vector<TwoPinRoute2D> &twoPinNets, vector<vector<GridSupply>> &graph, int chosen);
int score(TwoPinRoute2D twoPinNet, vector<vector<GridSupply>> *graph);
int lineLen (Pos start, Pos end);
int RerouteNet(vector<TwoPinRoute2D>& twoPinNets, vector<TwoPinRoute2D>& reRouted);
int bounding(TwoPinRoute2D net);

void BLMR(vector<vector<GridSupply>> &graph, TwoPinNets &netSet, TwoPinRoute2D &net, int &ite);
int findPath(vector<Pos> &list, vector<vector<BLMRgrid>> &map);
bool isBLCobtained(int &historyLen, Pos &grid, int &gridLen, Pos &start, Pos &end, int &ite);
int costOfEdge(Route2D& line, vector<vector<EdgeSupply>>& graph, vector<vector<GridSupply>>& usage, int& ite, vector<vector<EdgeS>>& skeleton);
bool needChange(BLMRgrid stepGrid, int nowCost, int nowLen, bool isBLC);
void Monotonic(vector<vector<GridSupply>> &graph, TwoPinNets &netSet, TwoPinRoute2D &net, int &ite);




void PrintPos(Pos a) {
    cout << "(" << a.row << "," << a.col << ") ";
}
void PrintRoute2D(Route2D a) {
    cout << "s: ";
    PrintPos(a.sIdx);
    cout << "/e: ";
    PrintPos(a.eIdx);
    cout << endl;
}
void PrintNet2D(Net2D a) {
    cout << a.name << " " << a.weight << endl;
    cout << "pin" << endl;
    for (auto pin: a.pinWithLay) {
        PrintPos(pin.locate);
        cout << "lay: " << pin.lay << endl;
    }
    cout << "route" << endl;
    for (auto route: a.route2Ds) {
        PrintRoute2D(route);
    }
    cout << "min route Lay:" << a.minRouteLay << endl;
    cout << endl;
}
void PrintTwoPinNet(TwoPinRoute2D a) {
    cout << "twoPin " << a.name << " " << a.weight << endl;
    cout << "s: ";
    PrintPos(a.sPin);
    cout << "/e: ";
    PrintPos(a.ePin);
    cout << endl << "route"<<endl;
    for (auto route: a.route) {
        PrintRoute2D(route);
    }
}
void PrintEdgeSupply(EdgeSupply a) {
    cout << "(" << a.row << "," << a.col << ") ";
}
void PrintGridSupply(GridSupply a) {
    cout << "(" << a.v << "," << a.h << ") ";
}
void PrintBLMRgrid(BLMRgrid a) {
    cout << a.cost << " ";
    // PrintPos(a.further);
}

bool isPosSame (Pos& a, Pos& b) {
    return (a.col == b.col && a.row == b.row);
}
bool isPininRoute(Pos& start, Pos& end, Pos& Pin) {
    if (end.col == start.col && end.col == Pin.col) {
        if (min(end.row,start.row) <= Pin.row && Pin.row <= max(end.row,start.row))
            return true;     //line is vertical
    } else if (start.row == end.row && end.row == Pin.row) {
        if (min(end.col,start.col) <= Pin.col && Pin.col <= max(end.col,start.col))
            return true;     //line is horrizontal
    }
    return false;
}
bool isLineVertical (Pos& start, Pos& end) {
    if (end.col == start.col)
        return true;
    else
        return false;
}
bool isLineConnect (Route2D& a, Route2D& b) {
    if (isLineVertical(a.eIdx,a.sIdx)^isLineVertical(b.eIdx,b.sIdx))
        return false;
    else {
        if ((isPininRoute(a.sIdx,a.eIdx,b.sIdx) || isPininRoute(a.sIdx,a.eIdx,b.eIdx))
                && (isPininRoute(b.sIdx,b.eIdx,a.eIdx) || isPininRoute(b.sIdx,b.eIdx,a.sIdx)))
            return true;
        else
            return false;
    }
}

vector<Net2D> Three2Two (Problem* pro) {
    vector<Net2D> flatenNets;   //return

    CellInst currentCellinst;
    string instName;            
    Net CurrentNet;
    Route CurrentRoute;         //now using

    Pos pin;
    Pin pinWithLay;
    vector<Pos> pins;
    vector<Pin> pinsWithLay;
    vector<Route2D> routes;
    Route2D flatenRoute;
    Net2D flatenNet;            //buffer

    //pin of Net2D,this part of Net2D are loss of routing
    for (int i = 0; i < pro->NumNet; i++) {
        CurrentNet = pro->nets[i];

        //reseting
        flatenNet.pin2Ds.clear();
        flatenNet.route2Ds.clear();

        //seting name and weight
        flatenNet.name = CurrentNet.name;
        flatenNet.weight = CurrentNet.weight;

        flatenNet.minRouteLay = 1;
        for (auto lay: pro->layers) {
            if (CurrentNet.minRouteLayConst == lay.name) {
                flatenNet.minRouteLay = lay.Idx;
                break;
            }
        }

        pins.clear();
        pinsWithLay.clear();
        //finding flatenNet.pin2Ds
        for (int j = 0; j < CurrentNet.NumPins; j++) {
            instName = CurrentNet.pins[j].instName;
            // string masterPinName = CurrentNet.pins[j].masterPinName;
            for (auto currentCellinst : pro->cellinsts) {
                if (instName == currentCellinst.name) {
                    pin.col = currentCellinst.colIdx;
                    pin.row = currentCellinst.rowIdx;
                    pinWithLay.locate = pin;
                    
                    for (auto currentMC : pro->masterCells) {
                        if (currentCellinst.MasterCellName == currentMC.name) {
                            for (auto p: currentMC.pins) {
                                if (CurrentNet.pins[j].masterPinName == p.name) {
                                    for (auto lay: pro->layers) {
                                        if (p.layer == lay.name) {
                                            pinWithLay.lay = lay.Idx;
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                    }

                    pins.push_back(pin);
                    pinsWithLay.push_back(pinWithLay);
                    break;
                }
            }
        }
        flatenNet.pin2Ds = pins;
        flatenNet.pinWithLay = pinsWithLay;
        flatenNets.push_back(flatenNet);
    }

    //now we put routing into Net2Ds
    for (int i = 0; i < flatenNets.size(); i++) {
        routes.clear();
        for (auto CurrentRoute : pro->routes) {
            if (CurrentRoute.elayIdx != CurrentRoute.slayIdx) continue;
            if (flatenNets[i].name == CurrentRoute.name) {
                flatenRoute.eIdx.row = CurrentRoute.erowIdx;
                flatenRoute.eIdx.col = CurrentRoute.ecolIdx;
                flatenRoute.sIdx.row = CurrentRoute.srowIdx;
                flatenRoute.sIdx.col = CurrentRoute.scolIdx;
                routes.push_back(flatenRoute);
            }
        }
        flatenNets[i].route2Ds = routes;
    }
    // PrintNet2D(flatenNets[0]);

    //deal with redundant way of writing route
    for (int k = 0; k < flatenNets.size(); k++) {
        MergeNet(flatenNets[k].route2Ds);
    }
    return flatenNets;
}
Route2D mergeLine (Route2D& a, Route2D& b) {
    Route2D c;
    if (isLineVertical(a.sIdx,a.eIdx)) {
        c.sIdx.row = min(min(a.sIdx.row, b.sIdx.row), min(a.eIdx.row, b.eIdx.row));
        c.sIdx.col = a.sIdx.col;
        c.eIdx.row = max(max(a.sIdx.row, b.sIdx.row), max(a.eIdx.row, b.eIdx.row));
        c.eIdx.col = a.sIdx.col;
    } else {
        c.sIdx.row = a.sIdx.row;
        c.sIdx.col = min(min(a.sIdx.col, b.sIdx.col), min(a.eIdx.col, b.eIdx.col));
        c.eIdx.row = a.sIdx.row;
        c.eIdx.col = max(max(a.sIdx.col, b.sIdx.col), max(a.eIdx.col, b.eIdx.col));
    }
    
    return c;
}
void MergeNet (vector<Route2D>& routes) {
    vector<int> merged = {};
    bool isMerged;

    for (int i = 0; i < routes.size(); i++) {
        for (int j = i + 1; j < routes.size(); j++) {
            if (isLineConnect(routes[i],routes[j])) {
                isMerged = false;
                for (auto p: merged) {
                    if (p == i || p == j) {
                        isMerged = true;
                    }
                }
                if (isMerged)
                    continue;
                else {
                    merged.push_back(i);
                    merged.push_back(j);
                    routes.push_back(mergeLine(routes[i],routes[j]));
                }
            }
        }
    }
    sort(merged.begin(), merged.end(), [](int a, int b)
         { return a > b; }); //decreasing

    for (auto p: merged) {
        routes.erase(routes.begin() + p);
    }
    return;
}

TwoPinNets CreateNetSet(Problem* pro, string& name) {
    TwoPinNets a;
    int rowBound = pro->GGridBD[2];
    int colBound = pro->GGridBD[3];
    a.name = name;
    GridSupply g;
    g.h = 0;
    g.v = 0;
    vector<GridSupply> vec;
    for (int i = 0; i < rowBound; i++) {
        vec.clear();
        for (int j = 0; j < colBound; j++){
            vec.push_back(g);
        }
        a.usage.push_back(vec);
    }
    return a;
}
void changeTwoPinNet(TwoPinNets &netSet, TwoPinRoute2D &newNet) {

    for (int i = 0; i < netSet.net.size(); i++) {
        if (isPosSame(netSet.net[i].ePin,newNet.ePin) && isPosSame(netSet.net[i].sPin,newNet.sPin)) {
            

            changeUsage(netSet.usage, netSet.net[i], false);
            netSet.net.erase(netSet.net.begin() + i);
            break;
        }
    }

    changeUsage(netSet.usage, newNet);

    netSet.net.push_back(newNet);
}
void changeUsage(vector<vector<GridSupply>>& usage,TwoPinRoute2D &net, bool add) {
    int col;
    int row;
    Pos start;
    Pos end;

    if (add) {
        for (auto line:net.route){
            start = line.sIdx;
            end = line.eIdx;
            if (!isLineVertical(start,end)) {
                row = start.row;
                for (col = min(start.col, end.col); col <= max(start.col, end.col); col++) {
                    usage[row-1][col-1].h++;
                }
            } else {
                col = start.col;
                for (row = min(start.row, end.row); row <= max(start.row, end.row); row++) {
                    usage[row-1][col-1].v++;
                }
            }
        }
    } else {
        for (auto line:net.route){
            start = line.sIdx;
            end = line.eIdx;
            if (!isLineVertical(start,end)) {
                row = start.row;
                for (col = min(start.col, end.col); col <= max(start.col, end.col); col++)
                {
                    usage[row-1][col-1].h--;
                    if (usage[row-1][col-1].h < 0)
                        cout << "error for usagemap h";
                }
            } else {
                col = start.col;
                for (row = min(start.row, end.row); row <= max(start.row, end.row); row++) {
                    usage[row-1][col-1].v--;
                    if (usage[row-1][col-1].v < 0)
                        cout << "error for usagemap v";
                }
            }
        }
    }
}

vector<vector<GridSupply>> GenerateGridSupplyGraph (Problem* pro) {
    int hSupply = 0;
    int vSupply = 0;
    int rowBound = pro->GGridBD[2];
    int colBound = pro->GGridBD[3];

    vector<vector<GridSupply>> graph;
    vector<GridSupply> rowGraph;
    GridSupply grid;

    int rId, cId, delta;
    Layer layer;

    for (Layer lay: pro->layers) {
        if (lay.direction == 'H')
            hSupply += lay.defaultsupply;
        else if (lay.direction == 'V')
            vSupply += lay.defaultsupply;
    }

    //assert defaultsupply to grids
    for (int i = 0; i < rowBound; i++) {
        rowGraph.clear();
        for (int j = 0; j < colBound; j++) {
            grid.v = vSupply;
            grid.h = hSupply;
            rowGraph.push_back(grid);
        }
        graph.push_back(rowGraph);
    }
    //change grids of nondefaultsupply
    for (auto nd: pro->NonDefaultSupplies) {
        rId = nd.rowIdx;
        cId = nd.colIdx;
        delta = nd.relatedValue;
        layer = pro->layers[nd.LayIdx - 1];

        if (layer.direction == 'H') graph.at(rId-1)[cId-1].h += delta;
        else if (layer.direction == 'V') graph.at(rId-1)[cId-1].v += delta;
    }

    //blockage also have demand     
    for (auto cell: pro->cellinsts) {
        for (auto mc: pro->masterCells) {
            if (cell.MasterCellName == mc.name) {
                for (auto blkg: mc.blkgs) {
                    for (auto lay: pro->layers) {
                        if (lay.name == blkg.layer) {   
                            if (lay.direction == 'H') {
                                graph[cell.rowIdx-1][cell.colIdx-1].h -= blkg.demand;
                            } else {
                                graph[cell.rowIdx-1][cell.colIdx-1].v -= blkg.demand;
                            }
                        }
                    }
                }
            }
        }
    }


    return graph;
}
vector<vector<EdgeSupply>> Grid2EdgeSupply (vector<vector<GridSupply>>& graph) {
    vector<vector<EdgeSupply>> Egraph;
    vector<EdgeSupply> rowGraph;
    EdgeSupply corGrid;
    int rowBound = graph.size();
    int colBound = graph[0].size();

    for (int i = 0; i < rowBound; i++) {
        rowGraph.clear();
        for (int j = 0; j < colBound; j++) {
            corGrid.row = (i == 0) ? 0 : min(graph[i][j].h, graph[i-1][j].h);
            corGrid.col = (j == 0) ? 0 : min(graph[i][j].v, graph[i][j - 1].v);
            rowGraph.push_back(corGrid);
        }
        Egraph.push_back(rowGraph);
    }
    return Egraph;
}
void SupplyChange (vector<vector<GridSupply>>& graph, Net2D newNet, Net2D oldNet){
    vector<Route2D> oldRoute = oldNet.route2Ds;
    vector<Route2D> newRoute = newNet.route2Ds;

    Pos start;
    Pos end;
    int rId;
    int cId;

    for (auto oldLine: oldRoute) {
        start = oldLine.sIdx;
        end = oldLine.eIdx;
        if (isLineVertical(start,end)){
            cId = start.col;
            for (rId = min(start.row, end.row); rId <= max(start.row, end.row); rId++) {
                graph.at(rId - 1)[cId - 1].v++;
            }
        } else {
            rId = start.row;
            for (cId = min(start.col, end.col); cId <= max(start.col, end.col); cId++) {
                graph.at(rId - 1)[cId - 1].h++;
            }
        }
    }

    for (auto newLine: newRoute) {
        start = newLine.sIdx;
        end = newLine.eIdx;
        if (isLineVertical(start,end)){
            cId = start.col;
            for (rId = min(start.row, end.row); rId <= max(start.row, end.row); rId++) {
                graph.at(rId - 1)[cId - 1].v--;
            }
        } else {
            rId = start.row;
            for (cId = min(start.col, end.col); cId <= max(start.col, end.col); cId++) {
                graph.at(rId - 1)[cId - 1].h--;
            }
        }
    }
    return;
}
void afterRouting (vector<vector<GridSupply>>& graph, TwoPinRoute2D& net, TwoPinNets& netSet) {
    vector<vector<GridSupply>> oldUsage = netSet.usage;
    changeTwoPinNet(netSet, net);
    vector<vector<GridSupply>> newUsage = netSet.usage;

    for (int row = 0; row < graph.size(); row++) {
        for (int col = 0; col < graph[0].size(); col++) {
            if (oldUsage[row][col].h == 0 && newUsage[row][col].h != 0) {
                graph[row][col].h--;
            } else if(oldUsage[row][col].h != 0 && newUsage[row][col].h == 0) {
                graph[row][col].h++;
            }

            if (oldUsage[row][col].v == 0 && newUsage[row][col].v != 0) {
                graph[row][col].v--;
            } else if(oldUsage[row][col].v != 0 && newUsage[row][col].v == 0) {
                graph[row][col].v++;
            }
        }
    }
}

TwoPinRoute2D Multi2TwoPinRoute (Net2D& net, Pos sPin, Pos ePin) {
    TwoPinRoute2D TwoPinNet;    //result
    TwoPinNet.name = net.name;
    TwoPinNet.weight = net.weight;
    TwoPinNet.ePin = ePin;
    TwoPinNet.sPin = sPin;      //setting
    Route2D line;               //buffer

    //return with ePin to sPin
    TwoPinNet.route = FindRoute((net.route2Ds),sPin,ePin);  
    
    return TwoPinNet;
}
vector<Route2D> FindRoute (vector<Route2D>& ref, Pos sPin, Pos ePin) {
    vector<Route2D> *rawRoute = new vector<Route2D>;
    vector<Route2D> route;
    Route2D line;
    FindRouteRaw(rawRoute,ref,sPin,ePin);

    if (rawRoute->size() == 1) {
        if (isPosSame(sPin,ePin)) {
            line.eIdx = ePin;
            line.sIdx = sPin;
            route.push_back(line);
            return route;
        }
    }

    for (int i = 0; i < rawRoute->size() - 1; i++) {
        line.sIdx = i == 0 ? sPin : route.back().eIdx; //rawRoute->at(i).sIdx;
        line.eIdx = isPininRoute(rawRoute->at(i).sIdx,rawRoute->at(i).eIdx,rawRoute->at(i+1).sIdx) ? rawRoute->at(i + 1).sIdx : rawRoute->at(i + 1).eIdx;
        route.push_back(line);
    }
        // for (auto n: route) {
        //     PrintRoute2D(n);
        // }
        // cout << endl;
    return route;
}
void FindRouteRaw (vector<Route2D>* route, vector<Route2D>& ref, Pos sPin, Pos ePin) {
    //return with routing [(sPin,eP0),(sP1,eP1),....,(sPn,ePn),(ePin,ePin)]

    Pos end;
    Pos start;
    Pos Pin;
    Route2D line;
    char achivable;
    bool isLineUsed;

    if (isPosSame(sPin,ePin)) {
        line.sIdx = sPin;
        line.eIdx = ePin;
        route->push_back(line);
        return;
    }

    for (int j = 0; j < ref.size(); j++) {
        isLineUsed = false;

        line = ref.at(j);
        end = line.eIdx;
        start = line.sIdx;

        // test whether line is in route already
        for (auto usedline: *route) {
            if ((isPosSame(end,usedline.eIdx) && isPosSame(start,usedline.sIdx)) || (isPosSame(end,usedline.sIdx) && isPosSame(start,usedline.eIdx))) {
                isLineUsed = true;
            }
        }
        if (isLineUsed)
            continue;

        //line not used, then store it first
        route->push_back(line);

        //whether sPin in this line
        if (isPininRoute(start, end, sPin)) {
            if (isLineVertical(start,end)) {
                Pin.col = start.col;
                for (int i = min(end.row, start.row); i <= max(end.row, start.row); i++){
                    Pin.row = i;

                    FindRouteRaw(route, ref, Pin, ePin);
                    if (isPosSame(route->back().eIdx,ePin) && isPosSame(route->back().sIdx,ePin)) {
                        return;
                    }
                }
            } else {
                Pin.row = start.row;
                for (int i = min(end.col, start.col); i <= max(end.col, start.col); i++){
                    Pin.col = i;

                    FindRouteRaw(route, ref, Pin, ePin);
                    if (isPosSame(route->back().eIdx,ePin) && isPosSame(route->back().sIdx,ePin)) {
                        return;
                    }
                }
            }
        };
        //if in the line, then find next line
        
        //if line can't lead to ePin -> the line is not solution
        route->pop_back();
    }
    return;
}

//used only after finish a inter of Net rerouting
void SortTaskQueue (vector<TwoPinRoute2D>& twoPinNets, vector<vector<GridSupply>>& graph, int  chosen) {
    //not sure whether go wrong
    vector<vector<GridSupply>> *ref = &graph;
    if (chosen != -1) {
        TwoPinRoute2D chosenNet = twoPinNets.at(chosen);
        twoPinNets.erase(twoPinNets.begin() + chosen);
        sort(twoPinNets.begin(), twoPinNets.end(), [ref](TwoPinRoute2D a, TwoPinRoute2D b) {
                return score(a, ref) > score(b, ref);
            });
        twoPinNets.push_back(chosenNet);
    } else {
        sort(twoPinNets.begin(), twoPinNets.end(), [ref](TwoPinRoute2D a, TwoPinRoute2D b) {
            return score(a, ref) >= score(b, ref);
        });
    }
}
int score(TwoPinRoute2D twoPinNet, vector<vector<GridSupply>>* graph) {
    int coefOver = 30;
    int coefLen = 1;
    int score = 0;
    Pos start;
    Pos end;
    for (auto line: twoPinNet.route) {
        start = line.sIdx;
        end = line.eIdx;
        score += coefLen * twoPinNet.weight * lineLen(start, end);
        if (isLineVertical(start,end)){
            for (int r = min(start.row, end.row); r <= max(start.row, end.row); r++) {
                if (graph->at(r - 1)[start.col - 1].v < 0) {
                    score += coefOver;
                }
            }
        } else {
            for (int c = min(start.col, end.col); c <= max(start.col, end.col); c++) {
                if (graph->at(start.row - 1)[c - 1].h < 0) {
                    score += coefOver;
                }
            }
        }
    }

    return score;
}
int lineLen (Pos start, Pos end) {
    return abs(start.row - end.row) + abs(start.col - end.col);
}
int RerouteNet (vector<TwoPinRoute2D>& twoPinNets, vector<TwoPinRoute2D>& reRouted) {
    //return the data location of most needing rerouting net
    int index;
    int currentBounding = -1;
    bool isReRouted;
    for (int i = 0; i < twoPinNets.size(); i++) {
        isReRouted = false;
        if (currentBounding == -1 || bounding(twoPinNets.at(i)) < currentBounding) {
            for (auto r: reRouted) {
                if (isPosSame(r.sPin, twoPinNets[i].sPin) && isPosSame(r.ePin, twoPinNets[i].ePin) && r.name == twoPinNets[i].name) {
                    isReRouted = true;
                    break;
                }
            }
            if (isReRouted) {
                continue;
            }
            currentBounding = bounding(twoPinNets.at(i));
            index = i;
        }
    }
    return index;
}
int bounding (TwoPinRoute2D net) {
    int len = 0;
    for (auto line: net.route) {
        len += net.weight * lineLen(line.sIdx, line.eIdx);
    }
    return len;
}

void BLMR(vector<vector<GridSupply>>& graph, TwoPinNets& netSet, TwoPinRoute2D& net, int& ite) {
    //copy data
    vector<vector<EdgeS>> skeleton = netSet.skeleton;
    Pos start = net.sPin;
    Pos end = net.ePin;
    int oldLen = 0;
    for (auto p: net.route) {
        oldLen += lineLen(p.sIdx, p.eIdx);
    }

    //record
    // cout << "record" << endl;
    vector<vector<BLMRgrid>> BLMRmap;
    vector<BLMRgrid> bufferVec;
    BLMRgrid buffer;
    buffer.cost = std::numeric_limits<int>::max();
    buffer.isFixed = false;
    for (int i = 0; i < graph.size(); i++) {
        bufferVec.clear();
        for (int j = 0; j < graph[0].size(); j++) {
            bufferVec.push_back(buffer);
        }
        BLMRmap.push_back(bufferVec);
    }
    
    BLMRmap[start.row - 1][start.col - 1].further = start;
    BLMRmap[start.row - 1][start.col - 1].cost = 0;
    BLMRmap[start.row - 1][start.col - 1].isBLC = true;
    BLMRmap[start.row - 1][start.col - 1].len = 0;

    vector<Pos> need;
    need.push_back(start);
    vector<Route2D> route;

    //buffer
    // cout << "buffer" << endl;
    BLMRgrid grid;
    BLMRgrid stepGrid;
    Pos mapIdx;
    int rId;
    int cId;
    int needIdx;
    int lineSupply;
    Route2D line;
    Pos lineEnd;
    bool isBLC;
    Pos nextPos;

    // cout << "clear now routed net && get operated var" << endl;
    TwoPinRoute2D newNet = net;
    newNet.route.clear();
    afterRouting(graph, newNet, netSet);
    vector<vector<GridSupply>> usage = netSet.usage;
    vector<vector<EdgeSupply>> supplyG = Grid2EdgeSupply(graph);

    //find end
    while (true) { //do until mapIdx == end
        //find grid with less cost
        // cout << "s" << endl;
        for (auto k: BLMRmap) {
            for (auto q: k) {
                PrintBLMRgrid(q);
            }
            // cout << endl;
        }
        // cout << endl;
        needIdx = findPath(need,BLMRmap);
        // cout << need.size() << endl;
        mapIdx = need[needIdx];

        if (isPosSame(mapIdx,end))
            break;
        // cout << "e" << endl;
        need.erase(need.begin() + needIdx);
        // cout << "a" << endl;
        line.sIdx = mapIdx;
        BLMRmap[mapIdx.row - 1][mapIdx.col - 1].isFixed = true;
        grid = BLMRmap[mapIdx.row - 1][mapIdx.col - 1];
        isBLC = isBLCobtained(oldLen, mapIdx, grid.len, start, end, ite);
        PrintPos(mapIdx);
        // cout << supplyG.size() << endl;
        // cout << supplyG[0].size() << endl;
        // cout << "upper" << endl;
        if (mapIdx.row != 1) { // upper
            lineEnd.row = mapIdx.row - 1;
            lineEnd.col = mapIdx.col;
            line.eIdx = lineEnd;

            lineSupply = costOfEdge(line, supplyG, usage, ite, skeleton);
            // cout << lineSupply << endl;
            stepGrid = BLMRmap[lineEnd.row - 1][lineEnd.col - 1];
            if (needChange(stepGrid, grid.cost + lineSupply, grid.len + 1, isBLC)) {
                stepGrid.cost = grid.cost + lineSupply;
                stepGrid.len = grid.len + 1;
                stepGrid.further = mapIdx;
                stepGrid.isBLC = isBLC;
                BLMRmap[lineEnd.row - 1][lineEnd.col - 1] = stepGrid;
                need.push_back(lineEnd);
            }
        }
        // cout << "lower" << endl;
        if (mapIdx.row != supplyG.size()) { // lower
            
            lineEnd.row = mapIdx.row + 1;
            lineEnd.col = mapIdx.col;
            line.eIdx = lineEnd;

            lineSupply = costOfEdge(line, supplyG, usage, ite, skeleton);
            // cout << lineSupply << endl;
            stepGrid = BLMRmap[lineEnd.row - 1][lineEnd.col - 1];
            if (needChange(stepGrid, grid.cost + lineSupply, grid.len + 1, isBLC)) {
                stepGrid.cost = grid.cost + lineSupply;
                stepGrid.len = grid.len + 1;
                stepGrid.further = mapIdx;
                stepGrid.isBLC = isBLC;
                BLMRmap[lineEnd.row - 1][lineEnd.col - 1] = stepGrid;
                need.push_back(lineEnd);
            }
        }
        // cout << "left" << endl;
        if (mapIdx.col != 1) { // left
            
            lineEnd.row = mapIdx.row;
            lineEnd.col = mapIdx.col - 1;
            line.eIdx = lineEnd;

            lineSupply = costOfEdge(line, supplyG, usage, ite, skeleton);
            // cout << lineSupply << endl;
            stepGrid = BLMRmap[lineEnd.row - 1][lineEnd.col - 1];
            if (needChange(stepGrid, grid.cost + lineSupply, grid.len + 1, isBLC)) {
                stepGrid.cost = grid.cost + lineSupply;
                stepGrid.len = grid.len + 1;
                stepGrid.further = mapIdx;
                stepGrid.isBLC = isBLC;
                BLMRmap[lineEnd.row - 1][lineEnd.col - 1] = stepGrid;
                need.push_back(lineEnd);
            }
        }
        // cout << "right" << endl;
        if (mapIdx.col != supplyG.size()) { // right
            //cout << "a" << endl;
            lineEnd.row = mapIdx.row;
            lineEnd.col = mapIdx.col + 1;
            line.eIdx = lineEnd;

            lineSupply = costOfEdge(line, supplyG, usage, ite, skeleton);
            // cout << lineSupply << endl;
            stepGrid = BLMRmap[lineEnd.row - 1][lineEnd.col - 1];
            //cout << "b" << endl;
            if (needChange(stepGrid, grid.cost + lineSupply, grid.len + 1, isBLC)) {
                stepGrid.cost = grid.cost + lineSupply;
                stepGrid.len = grid.len + 1;
                stepGrid.further = mapIdx;
                stepGrid.isBLC = isBLC;
                BLMRmap[lineEnd.row - 1][lineEnd.col - 1] = stepGrid;
                need.push_back(lineEnd);
            }
        }
    }
    // cout << "futher" << endl;
    line.eIdx = end;
    line.sIdx = BLMRmap[end.row - 1][end.col - 1].further;
    while (!isPosSame(line.sIdx, start)) {
        nextPos = BLMRmap[line.sIdx.row - 1][line.sIdx.col - 1].further;
        if (isPininRoute(nextPos,line.eIdx,line.sIdx)) {
            line.sIdx = nextPos;
        } else {
            route.push_back(line);
            line.eIdx = line.sIdx;
            line.sIdx = nextPos;
        }
    }
    route.push_back(line);

    // cout << "refine" << endl;
    newNet.route = route;
    afterRouting(graph,  newNet, netSet);
    net = newNet;
    return;
}
int findPath (vector<Pos>& list, vector<vector<BLMRgrid>>& map) {
    int ans = 0;
    int nowCost = -100;
    for (int i = 0; i < list.size(); i++) {
        if (nowCost == -100 || nowCost > map[list[i].row - 1][list[i].col - 1].cost) {
            nowCost = map[list[i].row - 1][list[i].col - 1].cost;
            ans = i;
        }
    }
    return ans;
}
bool isBLCobtained(int &historyLen, Pos &grid, int &gridLen,Pos &start, Pos &end, int& ite) {
    int a = 9;
    float b = 1.5;
    float BLC = float(lineLen(start, end)) * (1 - atan(ite - a) + b);
    float ew = float(historyLen) * float(lineLen(grid, end)) / float(lineLen(start, end));
    // cout << "BLC,ew" << BLC << " " << ew << endl;
    return (BLC > ew + float(gridLen));
}
//consider skeleton edge
int costOfEdge(Route2D& line, vector<vector<EdgeSupply>>& graph, vector<vector<GridSupply>>& usage, int& ite, vector<vector<EdgeS>>& skeleton){
    float eCost;
    float isSkeleton;
    float kappa = 100000;
    float w = 5.0;
    float eSupply = 0.0;
    float C3 = 150.0;
    float C4 = 0.3;
    // float C5 = 30;
    float C6 = 200.0;
    if (isLineVertical(line.sIdx,line.eIdx)) {
        eSupply = float(graph[max(line.sIdx.row, line.eIdx.row) - 1][line.sIdx.col - 1].row);
        isSkeleton = float(skeleton[min(line.eIdx.row,line.sIdx.row) - 1][line.sIdx.col - 1].col);
        if (eSupply > 0 && usage[line.sIdx.row - 1][line.sIdx.col - 1].v > 0 && usage[line.eIdx.row - 1][line.eIdx.col - 1].v > 0) {
            return 0;
        }
    } else {
        eSupply = float(graph[line.sIdx.row - 1][max(line.sIdx.col, line.eIdx.col) - 1].col);
        isSkeleton = float(skeleton[line.sIdx.row - 1][min(line.sIdx.col, line.eIdx.col) - 1].row);
        if (eSupply > 0 && usage[line.sIdx.row - 1][line.sIdx.col - 1].h > 0 && usage[line.eIdx.row - 1][line.eIdx.col - 1].h > 0) {
            return 0;
        }
    }

    eCost = (10 + C3 / exp(C4 * float(eSupply)) + C6 / (pow(2.0, float(ite))) - isSkeleton * w);
    if (eSupply <= 0) {
        PrintRoute2D(line);
        PrintEdgeSupply(graph[line.sIdx.row - 1][max(line.sIdx.col, line.eIdx.col) - 1]);
        return int(kappa * eCost);
    } else {
        return int(100 * eCost);
    }
}
bool needChange(BLMRgrid stepGrid, int nowCost, int nowLen, bool isBLC) {
    if (stepGrid.isFixed) {
        return false;
    } else if (stepGrid.cost == std::numeric_limits<int>::max()) {
        return true;
    } else if (isBLC) {
        if (!stepGrid.isBLC || stepGrid.cost > nowCost) {
            return true;
        }
    } else {
        if (!stepGrid.isBLC && stepGrid.len > nowLen) {
            return true;
        }
    }
    return false;
}
void Monotonic(vector<vector<GridSupply>> &graph, TwoPinNets &netSet, TwoPinRoute2D &net, int &ite) {
    int tester = 2;
    vector<Route2D> route;
    Route2D line;
    int x;
    int dis;

    Pos s = net.sPin;
    if (tester == 1) {
        while (!isPosSame(s,net.ePin)) {
            srand( time(NULL) );
            x = rand();
            dis = lineLen(s, net.ePin);
            if (x % dis < abs(s.row - net.ePin.row)) {
                if (s.row < net.ePin.row) {
                    line.sIdx = s;
                    line.eIdx.col = s.col;
                    line.eIdx.row = s.row + 1;
                    route.push_back(line);
                } else {
                    line.sIdx = s;
                    line.eIdx.col = s.col;
                    line.eIdx.row = s.row - 1;
                    route.push_back(line);
                }
            } else {
                if (s.col < net.ePin.col) {
                    line.sIdx = s;
                    line.eIdx.col = s.col + 1;
                    line.eIdx.row = s.row;
                    route.push_back(line);
                } else {
                    line.sIdx = s;
                    line.eIdx.col = s.col - 1;
                    line.eIdx.row = s.row;
                    route.push_back(line);
                }
            }
            s = line.eIdx;
        }
        MergeNet(route);
    }

    // Lshape
    if (tester == 2) {
        line.sIdx = net.sPin;
        line.eIdx.row = net.ePin.row;
        line.eIdx.col = net.sPin.col;
        route.push_back(line);
    
        line.eIdx = net.ePin;
        line.sIdx.row = net.ePin.row;
        line.sIdx.col = net.sPin.col;
        route.push_back(line);
    }


    net.route = route;
    afterRouting(graph, net, netSet);
}