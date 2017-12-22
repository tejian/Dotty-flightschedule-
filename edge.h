#ifndef EDGE_H
#define EDGE_H

// functions for both graph and its residual

class Edge
{
	public:
		Edge (int p_from, int p_to, int p_capacity, int p_flow, int p_id = -1);

		int setFlow (int p_flow);
		int setCapacity (int p_capacity);

		int getID() const;

		int getTo() const;
		int getFrom() const;

		int getFlow() const;
		int getCapacity() const;

		int getResidualCapacity (int p_from) const;
		
		int addFlow (int p_flow, int p_from);

	private:
		int m_to, m_from;
		int m_curFlow;
		int m_capacity;

		// used as representation
		// is -1 one if we dont need this
		int m_id;
};

#endif
