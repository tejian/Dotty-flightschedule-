#include "edge.h"

Edge::Edge (int p_from, int p_to, int p_capacity, int p_flow)
	: m_from (p_from), m_to (p_to), m_capacity (p_capacity),
	  m_curFlow (p_flow)
{

}

int Edge::setFlow (int p_flow)
{
	m_curFlow = p_flow;
}

int Edge::setCapacity (int p_capacity)
{
	m_capacity = p_capacity;
}

int Edge::getTo() const
{
	return m_to;
}

int Edge::getFrom() const
{
	return m_from;
}

int Edge::getFlow() const
{
	return m_curFlow;
}

int Edge::getCapacity() const
{
	return m_capacity;
}

int Edge::getResidualCapacity (int p_from) const
{
	if (p_from == m_from)
	{
		return m_capacity - m_curFlow;
	}
	else
	{
		return m_curFlow;
	}
}

int Edge::addFlow (int p_flow, int p_from)
{
	if (p_from == m_from)
	{
		m_curFlow += p_flow;	
	}
	else
	{
		m_curFlow -= p_flow;	
	}
}

