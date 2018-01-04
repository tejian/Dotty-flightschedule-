#include "flownetwork.h"
#include "fordfulkerson.h"
#include "flight.h"

#include <iostream>
#include <vector>
#include <set>


void printPath (int curr, Graph& g, int supplyPilot, std::vector <bool> visited);

int main()
{
	// --------------------------------------INPUT------------------------------------------
	// take input (flight) and sort it based on landing time
	std::multiset <Flight, ComapareFlight> flights;
	int to, from, takeOffTime, landingTime;
	int id = 1;
	while (std::cin >> from >> to >> takeOffTime >> landingTime)
	{
		Flight f {id, from, to, takeOffTime, landingTime};
		flights.insert (f);
		++id;
	}

	// --------------------------------------GRAPH MODELING------------------------------------------
	// create flownetwork with demands and check if there is a valid cirtulation with certain supply
	// each flight has 2 nodes and we have 4 extra nodes;
	int graphSize = flights.size () * 2 + 4;
	// we will give each extra nodes one its relative index
	int supplyPilot = graphSize - 4;
	int demandPilot = graphSize - 3;
	int mainSource  = graphSize - 2;
	int mainSink    = graphSize - 1;

	// create flow network needed size
	// save necessary datas
	FlowNetwork fn (graphSize);
	std::vector <Edge*> flightEdges;
	// counter to index nodes in graph
	int nodeId = 0;
	for (auto a = flights.begin(); a != flights.end(); ++a)
	{
		// a edge from supply pilot
		fn.addEdge (supplyPilot, nodeId, flights.size (), 0);
		// a edge to demandpilont
		fn.addEdge (nodeId + 1, demandPilot, flights.size (), 0);
		// reducing lower bound edge to demand on vertex
		// for each need flight add edge with lower bound 1
		// send id as representation too
		Edge* e = fn.addEdge (nodeId, nodeId + 1, flights.size (), 0, a -> ID);
		flightEdges.push_back (e);
		// reducing demand on vertex to normal flow network
		// an edge from supersource to flight destination
		fn.addEdge (mainSource, nodeId + 1, 1, 0);
		// an edge from flight source to supersink
		fn.addEdge (nodeId, mainSink, 1, 0);

		int nodeId2 = nodeId;
		for (auto b = a; b != flights.end(); ++b)
		{
			// if a landing and takeoff location of two flight are same
			// and takeoff time is greater than landing by 15 minutes time we
			// create an edge 
			if (a -> to == b -> from && b -> takeOffTime >= a -> landingTime + 15)
			{
				fn.addEdge (nodeId + 1, nodeId2, 1, 0);
			}
			nodeId2 += 2;
		}
		// increase the indx for create next flight
		nodeId += 2;
	}

	// --------------------------------------RUN ALGORITHM------------------------------------------
	// we have 1 demand for each flight as the lower bound of 1 gets reduced to this
	int demand = flights.size();
	// maximum demand of pilots is the number of flights itself
	int k = flights.size();
	// reduce it to normal flow graph 
	Edge* edgeSourceToSupply = fn.addEdge (mainSource, supplyPilot, k, 0);
	Edge* edgeDemandToSink   = fn.addEdge (demandPilot, mainSink  , k, 0);
	// in case for excess pilots
	Edge* edgeSupplyToDemand = fn.addEdge (supplyPilot, demandPilot, k, 0);

	// use binary search to find the minimum number of pilots needed
	int s = 0;
	int l = k;
	int answer = -1;
	while (s < l)
	{
		// demand circulation is valid only if total demands is equal to maxflow
		int m = (s + l) / 2; 
		edgeSourceToSupply -> setCapacity (m);
		edgeDemandToSink   -> setCapacity (m);
		edgeSupplyToDemand -> setCapacity (m);
		if (m + demand != FordFulkerson::computeMaxFlow (mainSource, mainSink, fn))
		{
			m += 1;
			s = m;
			edgeSourceToSupply -> setCapacity (m);
			edgeDemandToSink   -> setCapacity (m);
			edgeSupplyToDemand -> setCapacity (m);
			if (m + demand == FordFulkerson::computeMaxFlow (mainSource, mainSink, fn))
			{
				answer = m;
				break;
			}
		}
		else
		{
			m -= 1;
			l = m;
			edgeSourceToSupply -> setCapacity (m);
			edgeDemandToSink   -> setCapacity (m);
			edgeSupplyToDemand -> setCapacity (m);
			if (m + demand != FordFulkerson::computeMaxFlow (mainSource, mainSink, fn))
			{
				answer = m + 1;
				break;
			}
		}
	}

	// -------------------------------PRINT SOLUTION-------------------------------------
	std::cout << answer << std::endl;
	
	// generate graph with best solution
	edgeSourceToSupply -> setCapacity (answer);
	edgeDemandToSink   -> setCapacity (answer);
	edgeSupplyToDemand -> setCapacity (answer);
	FordFulkerson::computeMaxFlow (mainSource, mainSink, fn);
	// modify graph so its suitable to print solution
	// restore flow on flights
	for (auto a : flightEdges)
	{
		a -> setFlow (a -> getFlow() + 1 );
		//a -> setCapacity (1);
	}
	Graph& g = fn.getGraph();
	// make sure theres no path to supersink
	for (auto a : g[mainSink])
	{
		a -> setFlow (0);
		a -> setCapacity (0);
	}

	// print unique paths from supply to demand printing only flight ids
	std::vector <bool> visited (flights.size() + 1, false);
	for (int i = 0; i < g[supplyPilot].size(); ++i)
	{
		if (g[supplyPilot][i]-> getFrom() != supplyPilot) 
			continue;
		if (g[supplyPilot][i] -> getFlow() < 1)
			continue;

		// print pant and reduce flow
		printPath (g[supplyPilot][i] -> getTo(), g, demandPilot, visited);
		g[supplyPilot][i] -> setFlow (g[supplyPilot][i] -> getFlow() - 1);
	}
}

void printPath (int curr, Graph& g, int demandPilot, std::vector <bool> visited)
{	
	// if we reached supply pilot we return
	if (curr == demandPilot)
	{
		std::cout << "\n";
		return;
	}

	// find a good edge to take
	for (int i = 0; i < g[curr].size(); ++i)
	{
		if (g[curr][i] -> getFrom() == curr && g[curr][i] -> getFlow() > 0)
		{
			// reduce flow so it cant be traversed next time
			g[curr][i] -> setFlow (g[curr][i] -> getFlow() - 1);
			// print result if edge represents a flight
			if (g[curr][i] -> getID() > 0 && !visited[g[curr][i] -> getID()])
			{
				std::cout << g[curr][i] -> getID() << " ";
				visited[g[curr][i] -> getID()] = true;
			}
			// follow along
			printPath (g[curr][i] -> getTo(), g, demandPilot, visited);

			return;
		}
	}
}


