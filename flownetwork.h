#ifndef FLOWNETWORK_H
#define FLOWNETWORK_H

#include <vector>

#include "edge.h"

using Graph = std::vector < std::vector <Edge*>>;

class FlowNetwork
{
	public:
		FlowNetwork(int size)
		{
			m_graph = Graph(size);
		}

		~FlowNetwork ()
		{
			clear();
		}

		Edge* addEdge (int p_from, int p_to, int p_capacity, int p_flow, int p_id = -1)
		{
			Edge* e = new Edge (p_from, p_to, p_capacity, p_flow, p_id);

			m_graph[p_from].push_back (e);
			m_graph[p_to  ].push_back (e);

			m_edges.push_back (e);

			return e;
		}

		void clear()
		{
			for (int i = 0; i < m_edges.size(); ++i)
				delete m_edges[i];

			m_edges.clear();
			m_graph.clear();
		}

		void initializeFlowToZeroOnAllEdges()
		{
			for (int i = 0; i < m_edges.size(); ++i)
				m_edges[i] -> setFlow (0);
		}

		Graph& getGraph()
		{
			return m_graph;
		}

	private:
		std::vector <Edge*> m_edges;
		Graph m_graph;
};

#endif
