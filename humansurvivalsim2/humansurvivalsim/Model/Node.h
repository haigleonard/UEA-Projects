#pragma once
#include<iostream>

class Node
{
public:
	Node();
	Node(const Node* a) : fScore(a->fScore), gScore(a->gScore), x(a->x), y(a->y), value(a->value) {};
	Node(int, int);
	~Node();
	double getGScore();
	double getFScore();
	virtual int getX();
	virtual int getY();
	int getValue();
	void setValue(int);
	void setFScore(double);
	void setGScore(double);
	virtual void setX(int);
	virtual void setY(int);
	void heatTile();
	float getHeatR();
	float getHeatG();
	float getHeatB();
	void reduceHeat();
	bool operator<(const Node &a) const;
	bool operator==(const Node &a) const;
	static bool asc_sort(Node a, Node b) { return (a < b); };

	struct NodeHash
	{
		std::size_t operator()(const Node& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			return ((hash<int>()(k.x)
				^ (hash<int>()(k.y) << 1)) >> 1);
		}
	};

	friend	std::ostream &operator<<(std::ostream &output, const Node &n) {
		output << n.value;
		return output;
	}

protected:
	double gScore;
	double fScore;
	int x;
	int y;
	int value;
	float heatR;
	float heatG;
	float heatB;
};

