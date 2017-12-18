#include "flownetwork.h"
#include "fordfulkerson.h"

#include <iostream>
#include <vector>
#include <set>

class Flight
{
	public:
		int from;
		int to;
		int takeOffTime;
		int landingTime;
};

struct ComapareFlight
{
	bool operator() (const Flight& a, const Flight& b)
	{
		return a.landingTime < b.landingTime;
	}
};

int main()
{
	// take input and sort it based on landing time
	std::multiset <Flight, ComapareFlight> flights;

	int to, from, takeOffTime, landingTime;
	while (std::cin >> from >> to >> takeOffTime >> landingTime)
	{
		Flight f {from, to, takeOffTime, landingTime};
		flights.insert (f);
	}

	// create flownetwork with demands and check if there is a valid cirtulation with certain supply
	int mainSource, mainSink, supplyPilot, demandPilot;
	// each flight has 2 nodes and we have 4 extra nodes;
	int graphSize = flights.size () * 2 + 4;
	// we will give each one its relative index
	supplyPilot = graphSize - 4;
	demandPilot = graphSize - 3;
	mainSource  = graphSize - 2;
	mainSink    = graphSize - 1;

	// create flow network needed size
	FlowNetwork fn (graphSize);
	// counter to index flight
	int i = 0;
	for (auto a = flights.begin(); a != flights.end(); ++a)
	{
		// a edge from supply pilot
		fn.addEdge (supplyPilot, i, 1, 0);
		// a edge to demandpilont
		fn.addEdge (i + 1, demandPilot, 1, 0);
		// reducing lower bound edge to demand on vertex
		// for each need flight add edge with lower bound 1
		fn.addEdge (i, i + 1, 0, 0);
		// reducing demand on vertex to normal flow network
		// an edge from supersource to flight destination
		fn.addEdge (mainSource, i + 1, 1, 0);
		// an edge from flight source to supersink
		fn.addEdge (i, mainSink, 1, 0);

		int y = i;
		for (auto b = a; b != flights.end(); ++b)
		{
			// if a landing and takeoff location of two flight are same
			// and takeoff time is greater than landing by 15 minutes time we
			// create an edge 
			if (a -> to == b -> from && b -> takeOffTime >= a -> landingTime + 15)
			{
				fn.addEdge (i + 1, y, 1, 0);
			}
			y += 2;
		}
		// increase the indx for create next flight
		i += 2;
	}

	// we have 1 demand for each flight as the lower bound of 1 gets reduced to this
	int demand = flights.size();
	// k represents the number of pilot we need
	// i.e demand of pilots 
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
	std::cout << "minimum needed pilots " << answer << std::endl;
}


