#ifndef FLIGHT_H
#define FLIGHT_H


class Flight
{
	public:
		int ID;
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

#endif
