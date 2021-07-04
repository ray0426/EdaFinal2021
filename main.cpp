#include <iostream>
//#include <fstream>
#include "compress.h"
#include "kruskal.h"
#include "problem.h"
#include "rsmtAware.h"
#include "layerassignment.h"
#include <math.h>
#include <string>
#include <vector>
using namespace std;

void testH(Problem *pro);
float evaluate(vector<vector<GridSupply>> &graph, vector<TwoPinNets> &netSet);
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

    vector<vector<GridSupply>> gSupGraph = GenerateGridSupplyGraph(pro);
    vector<vector<GridSupply>> testGSupGraph = gSupGraph;

    //cout << "gridSupply(v,h)" << endl;
    //for (auto r : gSupGraph)
    //{
    //for (auto g : r)
    //{
    //PrintGridSupply(g);
    //}
    //cout << endl;
    //}

    vector<TwoPinNets> netSets;
    vector<vector<TwoPinNets>> ans;
    // vector<TwoPinNets> ans;
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


    vector<TwoPinNets> origin = netSets;
    int ite = 1;
    bool needBLMR = true;
    int nowScore = evaluate(gSupGraph, netSets);
    float score;
    vector<float> scoreRecord;
    vector<TwoPinRoute2D> reRouted;
    scoreRecord.push_back(nowScore);
    ans.push_back(netSets);
    int reRouteIdx;
    float reRouteRatio = 0.5;
    float BLMRRatio = 0.5;

    while (ite < 30 && queue.size() != 0)
    {
        // cout << "reRoute" << endl;
        // cout << queue.size() << endl;

        reRouteIdx = RerouteNet(queue, reRouted);
        reRouted.push_back(queue[reRouteIdx]);
        if (ite > queue.size()*reRouteRatio)
        {
            reRouted.erase(reRouted.begin());
        }
        // cout << reRouteIdx << endl;
        // cout << reRouted.size() << endl;
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
        // queue.pop_back();               
        // cout << "BLMR" << endl;
        for (int j = 0; j < queue.size(); j++)
        {
            needBLMR = true;
            for (int k = 0; k < reRouted.size()*BLMRRatio; k++)
            {
                if (isPosSame(reRouted[k].sPin, queue[j].sPin) && isPosSame(reRouted[k].ePin, queue[j].ePin) && reRouted[k].name == queue[j].name)
                {
                    needBLMR = false;
                    break;
                }
            }

            if (!needBLMR)
            {
                continue;
            }

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
        // cout << ite << endl;
        score = evaluate(gSupGraph, netSets);
        scoreRecord.push_back(score);
        if (nowScore > score)
        {
            ans.clear();
            ans.push_back(netSets);
            nowScore = score;
        }
        else if (nowScore == score)
        {
            ans.push_back(netSets);
        }
        ite++;
    }


    ans.push_back(origin);
    
    // for (auto s : scoreRecord)
    // {
    //   cout << s << "\n";
    // }
    // cout << endl;

    vector<vector<Net2D>> flattenNetAnss;
    vector<Net2D> flattenNetAns;
    Net2D bufferNet2D;
    vector<Route2D> flattenRoute;
    for (auto possibleAns : ans)
    {
        flattenNetAns.clear();
        for (auto nets : possibleAns)
        {
            flattenRoute.clear();

            bufferNet2D.name = nets.name;
            for (auto ref : flattenNet)
            {
                if (ref.name == bufferNet2D.name)
                {
                    bufferNet2D = ref;
                }
            }

            for (auto net : nets.net)
            {
                for (auto r : net.route)
                {
                    flattenRoute.push_back(r);
                }
            }

            // cout << nets.name;
            // for (auto a: nets.usage) {
            //     for (auto b: a) {
            //         PrintGridSupply(b);
            //     }
            //     cout << endl;
            // }

            MergeNet(flattenRoute);
            // cout << endl;
            // for (auto line:flattenRoute) {
            //     PrintRoute2D(line);
            // }
            bufferNet2D.route2Ds = flattenRoute;

            flattenNetAns.push_back(bufferNet2D);
        }
        flattenNetAnss.push_back(flattenNetAns);
    }

    // cout << "find two pin" << endl;
    vector<Net2D> twoPin;
    for (auto n : flattenNetAnss[0])
    {
        if (n.pin2Ds.size() == 2)
        {
            twoPin.push_back(n);
        }
    }

    // cout << "store ans" << endl;

    vector<Net2D> ansBuffer;
    Route2D line;
    int j;
    for (int k = 0; k < pow(2.0, twoPin.size()); k++) {
        ansBuffer.clear();
        j = k;
        for (int i = 0; i < twoPin.size(); i++) {
            twoPin[i].route2Ds.clear();
            if (j % 2 == 0) {
                line.sIdx = twoPin[i].pin2Ds[0];
                line.eIdx.row = twoPin[i].pin2Ds[1].row;
                line.eIdx.col = twoPin[i].pin2Ds[0].col;
                twoPin[i].route2Ds.push_back(line);

                line.eIdx = twoPin[i].pin2Ds[1];
                line.sIdx.row = twoPin[i].pin2Ds[1].row;
                line.sIdx.col = twoPin[i].pin2Ds[0].col;
                twoPin[i].route2Ds.push_back(line);
            } else {
                line.sIdx = twoPin[i].pin2Ds[0];
                line.eIdx.row = twoPin[i].pin2Ds[0].row;
                line.eIdx.col = twoPin[i].pin2Ds[1].col;
                twoPin[i].route2Ds.push_back(line);

                line.eIdx = twoPin[i].pin2Ds[1];
                line.sIdx.row = twoPin[i].pin2Ds[0].row;
                line.sIdx.col = twoPin[i].pin2Ds[1].col;
                twoPin[i].route2Ds.push_back(line);
            }
            j = j / 2;
        }
        for (auto n: flattenNetAnss[0]) {
            if (n.pin2Ds.size() != 2) {
                ansBuffer.push_back(n);
            } else {
                for (auto m: twoPin) {
                    if (m.name == n.name) {
                        ansBuffer.push_back(m);
                        break;
                    }
                }
            }
        }
        flattenNetAnss.push_back(ansBuffer);
    }

    // go through a for
    double min = -1.0; la_Route *min_r = new la_Route;
    for(int i = 0; i < flattenNetAnss.size(); i++)
    {
        la_Route *r = _layer_assignment(pro, flattenNetAnss.at(i));
        double score = _route_score(pro, r);
        if(min == -1.0 || min > score)
        {
            min = score;
            min_r = r;
        }
    }

    printf("-----------------------\n");
    _route_print(min_r, pro);
    printf("score:%llf\n", min);

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
float evaluate(vector<vector<GridSupply>> &graph, vector<TwoPinNets> &netSet)
{
    vector<Route2D> multiPinNet;
    float score = 0.0;
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
                score += float(lineLen(line.sIdx, line.eIdx)) * n.weight;
            }
        }
    }
    // cout << "score is :" << score << endl;
    // cout << endl;
    return isOverflowed ? 10000.0 : score;
}
