#include <iostream>
//#include <fstream>
#include "compress.h"
#include "kruskal.h"
#include "problem.h"
#include "rsmtAware.h"
#include "layerassignment.h"
#include <string>
#include <vector>
using namespace std;

void testH(Problem *pro);
int evaluate(vector<vector<GridSupply>> &graph, vector<TwoPinNets> &netSet);
int main(int argc, char **argv)
{
    vector<vector<EdgeS>> edgeSkeleton;
    if (!argv[1])
    {
        cout << "Please input case file" << endl;
        return -1;
    }
    Problem *pro = new Problem;
    pro->readCase(argv[1]);

    // testH(pro);

    vector<Net2D> flattenNet = Three2Two(pro);
    /*
    vector<vector<GridSupply>> gSupGraph = GenerateGridSupplyGraph(pro);
    vector<vector<GridSupply>> testGSupGraph = gSupGraph;
    // vector<vector<EdgeSupply>> eSupGraph = Grid2EdgeSupply(gSupGraph);
    
    cout << "gridSupply(v,h)" << endl;
    for (auto r : gSupGraph)
    {
        for (auto g : r)
        {
            PrintGridSupply(g);
        }
        cout << endl;
    }
    
    // print(flattenNet[1].pin2Ds);
    // edgeSkeleton = rsmtAware(flattenNet[1].pin2Ds, pro->GGridBD[0],
    //                          pro->GGridBD[2], pro->GGridBD[1],
    //                          pro->GGridBD[3]);
    vector<TwoPinNets> netSets;
    vector<TwoPinNets> ans;
    TwoPinNets netSet;
    vector<TwoPinRoute2D> queue;
    vector<Route2D> netPins;
    TwoPinRoute2D netRoute;

    for (auto net : flattenNet)
    {
        // cout << net.name << endl;

        netSet = CreateNetSet(pro, net.name);
        // cout << "netSetCreate" << endl;

        netSet.skeleton = rsmtAware(net.pin2Ds, pro->GGridBD[0], pro->GGridBD[2],
                                    pro->GGridBD[1], pro->GGridBD[3]);
        // cout << "skeleton created" << endl;
        // make two Pins, gen route, put into queue
        // cout << "rsmt ";
        netPins = decomposition(net.pin2Ds);
        // cout << "decompose" << endl;
        for (auto netPin : netPins)
        {
            netRoute = Multi2TwoPinRoute(net, netPin.sIdx, netPin.eIdx);
            // cout << "get Route: ";
            // PrintRoute2D(netPin);
            afterRouting(gSupGraph, netRoute, netSet);
            // cout << "gridSupply(v,h)" << endl;
            // for (auto r : gSupGraph)
            // {
            //     for (auto g : r)
            //     {
            //         PrintGridSupply(g);
            //     }
            //     cout << endl;
            // }
            queue.push_back(netRoute);
            // cout << "afterRouting" << endl;
        }
        netSets.push_back(netSet);
    }

    int ite = 1;
    int nowScore = evaluate(gSupGraph, netSets);
    int score;
    vector<int> scoreRecord;
    scoreRecord.push_back(nowScore);
    ans = netSets;
    int reRouteIdx;
    while (ite < 100 && queue.size() != 0)
    {
        // cout << "reRoute" << endl;
        // cout << queue.size() << endl;

        reRouteIdx = RerouteNet(queue);
        // cout << reRouteIdx << endl;
        for (int k = 0; k < netSets.size(); k++)
        {
            if (netSets[k].name == queue[reRouteIdx].name)
            {
                // Monotonic(gSupGraph, netSets[k], queue[reRouteIdx], ite);
                BLMR(gSupGraph, netSets[k], queue[reRouteIdx], ite);
                break;
            }
        }
        // cout << "sort" << endl;
        SortTaskQueue(queue, gSupGraph, reRouteIdx);
        queue.pop_back();                //if monotonic is do, don't do this
        // for (auto q: queue) {
        //     PrintTwoPinNet(q);
        //     cout << endl;
        // }
        // cout << "BLMR" << endl;
        for (int j = 0; j < queue.size(); j++)
        {
            // cout << j << endl;
            for (int k = 0; k < netSets.size(); k++)
            {
                // cout << netSets[k].name << " " << queue[j].name;

                if (netSets[k].name == queue[j].name)
                { // cout << "before\n";
                    // for(auto n: queue[j].route) {
                    //     PrintRoute2D(n);
                    // }

                    BLMR(gSupGraph, netSets[k], queue[j], ite);
                    // cout << "after\n";
                    // for(auto n: queue[j].route) {
                    //     PrintRoute2D(n);
                    // }
                    // cout << endl;
                    break;
                }
            }
        }
        // evaluate part
        //cout << ite << endl;
        score = evaluate(gSupGraph, netSets);
        scoreRecord.push_back(score);
        if (nowScore > score)
        {
            ans = netSets;
            nowScore = score;
        }
        ite++;
    }
    
    for (auto s : scoreRecord)
    {
        cout << s << "\n";
    }
    cout << endl;
    
    vector<Net2D> flattenNetAns;
    Net2D bufferNet2D;
    vector<Route2D> flattenRoute;
    for (auto nets: netSets) {
        flattenRoute.clear();

        bufferNet2D.name = nets.name;
        for (auto ref: flattenNet) {
            if (ref.name == bufferNet2D.name) {
                bufferNet2D = ref;
            }
        }

        for (auto net: nets.net) {
            for (auto r: net.route) {
                flattenRoute.push_back(r);
            }
        }

        // for (auto a: nets.usage) {
        //     for (auto b: a) {
        //         PrintGridSupply(b);
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        // for (auto line:flattenRoute) {
        //     PrintRoute2D(line);
        // }
        
        MergeNet(flattenRoute);
        bufferNet2D.route2Ds = flattenRoute;
        flattenNetAns.push_back(bufferNet2D);
    }
    printf("\n");
    */
   for(auto a : flattenNet)
   {
       printf("%s\n", a.name);
       for(auto r : a.route2Ds) printf("(%d,%d)<->(%d,%d)\n", r.sIdx.row, r.sIdx.col, r.eIdx.row, r.eIdx.col);
   }

    // use flattenNetAns to do 3D
    _layer_assignment_and_print_route(pro, flattenNet);

    delete pro;
    return 0;
}

void testH(Problem *pro)
{
    vector<Net2D> flattenNet = Three2Two(pro);
    vector<vector<GridSupply>> gSupGraph = GenerateGridSupplyGraph(pro);
    vector<vector<EdgeSupply>> eSupGraph = Grid2EdgeSupply(gSupGraph);

    // cout << "multiPinNets" << endl;
    // for(auto net: flattenNet){
    //    PrintNet2D(net);
    //}

    // cout << "gridSupply(v,h)" << endl;
    // for (auto r : gSupGraph)
    // {
    //     for (auto g : r)
    //     {
    //         PrintGridSupply(g);
    //     }
    //     cout << endl;
    // }

    // SupplyChange(gSupGraph,flattenNet[0]);
    // cout << "supplychange" << endl;
    // for (auto r: gSupGraph) {
    //    for (auto g: r) {
    //        PrintGridSupply(g);
    //    }
    //    cout << endl;
    //}
    //
    // cout << "edgeSupply(v,h)" << endl;
    // for (auto r: eSupGraph) {
    //    for (auto e: r) {
    //        PrintEdgeSupply(e);
    //    }
    //    cout << endl;
    //}

    TwoPinRoute2D TwoPinNet = Multi2TwoPinRoute(flattenNet[0],
                                                flattenNet[0].pin2Ds[0], flattenNet[0].pin2Ds[2]);
    PrintTwoPinNet(TwoPinNet);

    // TwoPinRoute2D Net1 = Multi2TwoPinRoute(flattenNet[1], flattenNet[1].pin2Ds[1],
    //                                        flattenNet[1].pin2Ds[2]);
    // TwoPinRoute2D Net2 = Multi2TwoPinRoute(flattenNet[1], flattenNet[1].pin2Ds[0],
    //                                        flattenNet[1].pin2Ds[3]);
    // TwoPinRoute2D Net3 = Multi2TwoPinRoute(flattenNet[1], flattenNet[1].pin2Ds[0],
    //                                        flattenNet[1].pin2Ds[1]);
    // vector<TwoPinRoute2D> Nets{Net1, Net2, Net3};
    //
    // string name = "N2";
    // TwoPinNets N2 = CreateNetSet(pro, name);
    //
    // for (auto n : Nets)
    // {
    //     // PrintTwoPinNet(n);
    //     afterRouting(gSupGraph, n, N2);
    // }
    //
    // cout << "gridSupply(v,h)" << endl;
    // for (auto r : gSupGraph)
    // {
    //     for (auto g : r)
    //     {
    //         PrintGridSupply(g);
    //     }
    //     cout << endl;
    // }

    // for (auto n: N2.net) {
    //     PrintTwoPinNet(n);
    // }
    // for (auto r: N2.usage) {
    //     for (auto g: r) {
    //         PrintGridSupply(g);
    //     }
    //     cout << endl;
    // }
    // int ite = 1;
    // BLMR(gSupGraph, N2, Net2, ite);
    // for (auto n: N2.net) {
    //     PrintTwoPinNet(n);
    // }

    // int i = RerouteNet(Nets);
    // PrintTwoPinNet(Nets[i]);
    // //
    // SortTaskQueue(Nets,i,gSupGraph);
    // for (auto n: Nets) {
    //    PrintTwoPinNet(n);
    //}
}
int evaluate(vector<vector<GridSupply>> &graph, vector<TwoPinNets> &netSet)
{
    vector<Route2D> multiPinNet;
    int score = 0;
    bool isOverflowed = false;

    for (auto r : graph)
    {
        for (auto g : r)
        {
            if (g.h < 0 || g.v < 0)
            {
                isOverflowed = true;
                cout << "overflowed ";
                PrintGridSupply(g);
                cout << endl;
            }
        }
    }

    for (auto nS : netSet)
    {
        multiPinNet.clear();
        for (auto n : nS.net)
        {
            for (auto line : n.route)
            {
                multiPinNet.push_back(line);
            }
            MergeNet(multiPinNet);
            for (auto line : multiPinNet)
            {
                score += lineLen(line.sIdx, line.eIdx) * n.weight;
            }
        }
    }
    // cout << "score is :" << score << endl;
    // cout << endl;
    return isOverflowed ? 10000 : score;
}
