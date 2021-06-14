// #include <iostream>
#include <vector>
#include <string>
#include "problem.h"
#include "compress.h"
#include <algorithm>
using namespace std;

bool isPosSame (Pos a, Pos b) {
    return (a.col == b.col && a.row == b.row);
}
bool isPininRoute(Pos start, Pos end, Pos Pin) {
    if (end.col == start.col) {
        if (min(end.row,start.row) <= Pin.row && Pin.row <= max(end.row,start.row))
            return true;     //line is vertical
    } else if (start.row == end.row) {
        if (min(end.col,start.col) <= Pin.col && Pin.col <= max(end.col,start.col))
            return true;     //line is horrizontal
    }
    return false;
}
bool isLineVertical (Pos start, Pos end) {
    if (end.col == start.col)
        return true;
    else
        return false;
}
bool isLineConnect (Route2D a, Route2D b) {
    if (isLineVertical(a.eIdx,a.sIdx)^isLineVertical(b.eIdx,b.sIdx))
        return false;
    else {
        if (isPosSame(a.eIdx,b.eIdx) || isPosSame(a.eIdx,b.sIdx) || isPosSame(a.sIdx,b.eIdx) || isPosSame(a.sIdx,b.sIdx))
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
    vector<Pos> pins;
    Route2D flatenRoute;
    Net2D flatenNet;            //buffer

    vector<int> merged;
    bool isMerged;

    //pin of Net2D,this part of Net2D are loss of routing
    for (int i = 0; i < pro->NumNet; i++) {
        CurrentNet = pro->nets[i];

        //reseting
        flatenNet.pin2Ds.clear();
        flatenNet.route2Ds.clear();

        //seting name and weight
        flatenNet.name = CurrentNet.name;
        flatenNet.weight = CurrentNet.weight;

        pins.clear();
        //finding flatenNet.pin2Ds
        for (int j = 0; j < CurrentNet.NumPins; j++) {
            instName = CurrentNet.pins[j].instName;
            // string masterPinName = CurrentNet.pins[j].masterPinName;
            for (auto currentCellinst : pro->cellinsts) {
                if (instName == currentCellinst.name) {
                    pin.col = currentCellinst.colIdx;
                    pin.row = currentCellinst.rowIdx;
                    pins.push_back(pin);
                    break;
                }
            }
        }
        flatenNet.pin2Ds = pins;
        flatenNets.push_back(flatenNet);
    }

    //now we put routing into Net2Ds
    for (int i = 0; i < pro->NumRoute; i++){
        CurrentRoute = pro->routes[i];
        
        if (CurrentRoute.elayIdx != CurrentRoute.slayIdx) continue;

        for (auto planedNet: flatenNets) {
            if (planedNet.name == CurrentRoute.name) {
                flatenRoute.eIdx.row = CurrentRoute.erowIdx;
                flatenRoute.eIdx.col = CurrentRoute.ecolIdx;
                flatenRoute.sIdx.row = CurrentRoute.srowIdx;
                flatenRoute.sIdx.col = CurrentRoute.scolIdx;
                planedNet.route2Ds.push_back(flatenRoute);
                break;
            }
        }
    }
    //deal with redundant way of writing route
    for (auto planedNet: flatenNets) {
        for (int i = 0; i < planedNet.route2Ds.size(); i++) {
            for (int j = i + 1; j < planedNet.route2Ds.size(); j++) {
                if (isLineConnect(planedNet.route2Ds[i],planedNet.route2Ds[j])) {
                    isMerged = false;
                    for (auto k: merged) {
                        if (k == i || k == j) {
                            isMerged == true;
                        }
                    }
                    if (isMerged)
                        continue;
                    else {
                        merged.push_back(i);
                        merged.push_back(j);
                        planedNet.route2Ds.push_back(mergeLine(planedNet.route2Ds[i],planedNet.route2Ds[j]));
                    }
                }
            }
        }
        sort(merged.begin(), merged.end(), [](int a, int b)
             { return a < b; }); //decreasing
        for (auto k: merged) {
            planedNet.route2Ds.erase(planedNet.route2Ds.begin() + k);
        }
    }
    return flatenNets;
}
Route2D mergeLine (Route2D a, Route2D b) {
    Route2D c;
    c.sIdx.row = min(min(a.sIdx.row, b.sIdx.row), min(a.eIdx.row, b.eIdx.row));
    c.sIdx.col = min(min(a.sIdx.col, b.sIdx.col), min(a.eIdx.col, b.eIdx.col));
    c.eIdx.row = max(max(a.sIdx.row, b.sIdx.row), max(a.eIdx.row, b.eIdx.row));
    c.eIdx.col = max(max(a.sIdx.col, b.sIdx.col), max(a.eIdx.col, b.eIdx.col));
    return c;
}

vector<vector<GridSupply>>* GenerateGridSupplyGraph (Problem* pro) {
    int hSupply;
    int vSupply;
    int rowBound = pro->GGridBD[2];
    int colBound = pro->GGridBD[3];
    vector<vector<GridSupply>>* graph;
    vector<GridSupply> rowGraph;
    GridSupply grid;

    //counting defaultsupply
    for (Layer layer: pro->layers) {
        if (layer.direction == 'H')
            hSupply += layer.defaultsupply;
        else if (layer.direction == 'V')
            vSupply += layer.defaultsupply;
    }
    //assert defaultsupply to grids
    for (int i = 0; i < rowBound; i++) {
        rowGraph.clear();
        for (int j = 0; j < colBound; j++) {
            grid.v = vSupply;
            grid.h = hSupply;
            rowGraph.push_back(grid);
        }
        graph->push_back(rowGraph);
    }
    //change grids of nondefaultsupply
    for (NonDefaultSupply nd: pro->NonDefaultSupplies) {
        int rId = nd.rowIdx;
        int cId = nd.colIdx;
        int delta = nd.relatedValue;
        Layer layer = pro->layers[nd.LayIdx];

        if (layer.direction == 'H') graph->at(rId)[cId].h += delta;
        else if (layer.direction == 'V') graph->at(rId)[cId].v += delta;
    }

    //blockage also have demand
    
    return graph;
}
vector<vector<EdgeSupply>>* Grid2EdgeSupply (vector<vector<GridSupply>>* graph) {
    vector<vector<EdgeSupply>>* Egraph;
    vector<EdgeSupply> rowGraph;
    EdgeSupply corGrid;
    int rowBound = graph->size();
    int colBound = graph->at(0).size();

    for (int i = rowBound - 1; i >= 0; i--) {
        rowGraph.clear();
        for (int j = colBound - 1; j >= 0; j--) {
            corGrid.row = (i == 0) ? 0 : min(graph->at(i)[j].h, graph->at(i - 1)[j].h);
            corGrid.col = (j == 0) ? 0 : min(graph->at(i)[j].v, graph->at(i)[j - 1].v);
            rowGraph.push_back(corGrid);
        }
        Egraph->push_back(rowGraph);
    }
    return Egraph;
}
void SupplyChange (vector<vector<GridSupply>>* graph, Net2D oldNet, Net2D newNet){
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
                graph->at(rId - 1)[cId - 1].v++;
            }
        } else {
            rId = start.row;
            for (cId = min(start.col, end.col); cId <= max(start.col, end.col); cId++) {
                graph->at(rId - 1)[cId - 1].h++;
            }
        }
    }

    for (auto newLine: newRoute) {
        start = newLine.sIdx;
        end = newLine.eIdx;
        if (isLineVertical(start,end)){
            cId = start.col;
            for (rId = min(start.row, end.row); rId <= max(start.row, end.row); rId++) {
                graph->at(rId - 1)[cId - 1].v--;
            }
        } else {
            rId = start.row;
            for (cId = min(start.col, end.col); cId <= max(start.col, end.col); cId++) {
                graph->at(rId - 1)[cId - 1].h--;
            }
        }
    }
    return;
}


TwoPinRoute2D Multi2TwoPinRoute (Net2D* net, Pos sPin, Pos ePin) {
    TwoPinRoute2D TwoPinNet;    //result
    TwoPinNet.name = net->name;
    TwoPinNet.weight = net->weight;
    TwoPinNet.ePin = ePin;
    TwoPinNet.sPin = sPin;      //setting
    Route2D line;               //buffer
    vector<Route2D>* ref;
    *ref = net->route2Ds;

    //return with ePin to sPin
    TwoPinNet.route = FindRoute(ref,sPin,ePin);  
    
    return TwoPinNet;
}
vector<Route2D> FindRoute (vector<Route2D>* ref, Pos sPin, Pos ePin) {
    vector<Route2D> *rawRoute;
    vector<Route2D> route;
    Route2D line;
    FindRouteRaw(rawRoute,ref,sPin,ePin);
    for (int i = 0; i < rawRoute->size() - 1; i++) {
        line.sIdx = rawRoute->at(i).sIdx;
        line.eIdx = rawRoute->at(i + 1).sIdx;
        route.push_back(line);
    }
    return route;
}
void FindRouteRaw (vector<Route2D>* route, vector<Route2D>* ref, Pos sPin, Pos ePin) {
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

    for (int j = 0; j < ref->size(); j++) {
        isLineUsed = false;

        line = ref->at(j);
        end = line.eIdx;
        start = line.sIdx;

        // test whether line is in route already
        for (auto usedline: *route) {
            if (isPosSame(end,usedline.eIdx) && isPosSame(start,usedline.sIdx)) {
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
                }
            } else {
                Pin.row = start.row;
                for (int i = min(end.col, start.col); i <= max(end.col, start.col); i++){
                    Pin.col = i;
                    FindRouteRaw(route, ref, Pin, ePin);
                }
            }
        };
        //if in the line, then find next line
        
        //if line can't lead to ePin -> the line is not solution
        if (!isPosSame(route->back().eIdx,ePin)) {
            route->pop_back();
        } else {
            break;
        }
    }
    return;
    // return vector<Pos>();
}


//not sure whether ref is good
//this two part can be private
// vector<Pos> FindRoute_old (vector<Route2D> ref, Pos sPin, Pos ePin) {
//     vector<Pos> route;
//     Pos end;
//     Pos start;
//     Pos Pin;
//     Route2D line;
//     vector<Route2D> nextRef = ref;
//     char achivable;
// 
//     if (sPin.col == ePin.col && sPin.row == ePin.row) {
//         route.push_back(ePin);
//         return route;
//     }
// 
//     for (int j = 0; j < ref.size(); j++) {
//         line = ref[j];
//         end = line.eIdx;
//         start = line.sIdx;
// 
//         achivable = isPininRoute(end, start, sPin);
// 
//         if (achivable == 'v') {
//             Pin.col = start.col;
//             for (int i = min(end.row, start.row); i <= max(end.row, start.row); i++){
//                 nextRef.erase(nextRef.begin() + j);
//                 Pin.row = i;
//                 route = FindRoute(nextRef, Pin, ePin);
//                 nextRef = ref;
//                 if (!route.empty()) {
//                     route.push_back(sPin);
//                     return route;
//                 }
//             }
//         } else if (achivable == 'h') {
//             Pin.row = start.row;
//             for (int i = min(end.col, start.col); i <= max(end.col, start.col); i++){
//                 nextRef.erase(nextRef.begin() + j);
//                 Pin.col = i;
//                 route = FindRoute(nextRef, Pin, ePin);
//                 nextRef = ref;
//                 if (!route.empty()) {
//                     route.push_back(sPin);
//                     return route;
//                 }
//             }
//         }
//     }
//     return vector<Pos>();
// }