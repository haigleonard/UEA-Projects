#include "Model/Node.h"



Node::Node(int x, int y)
{
	Node::fScore = 10000;
	Node::gScore = 10000;
	Node::x = x;
	Node::y = y;
	Node::heatR = 0.0;
	Node::heatG = 0.0;
	Node::heatB = 0.0;
}

Node::Node() {}

Node::~Node() {}


double Node::getGScore()
{
	return Node::gScore;
}

double Node::getFScore()
{
	return Node::fScore;
}

int Node::getX()
{
	return Node::x;
}

int Node::getY()
{
	return Node::y;
}

int Node::getValue()
{
	return Node::value;
}

void Node::setValue(int newVal)
{
	Node::value = newVal;
}

void Node::setFScore(double newFScore)
{
	Node::fScore = newFScore;
}

void Node::setGScore(double newGScore)
{
	Node::gScore = newGScore;
}

void Node::setX(int newX)
{
	Node::x = newX;
}

void Node::setY(int newY)
{
	Node::y = newY;
}

bool Node::operator<(const Node & a) const
{
	return fScore < a.fScore;
}

bool Node::operator==(const Node & a) const
{
	return x == a.x && y == a.y;
}


void Node::heatTile() {
		Node::heatR += 0.05;
		Node::heatB += 0.01;
		Node::heatG += 0.01;
}

float Node::getHeatR() {
	return Node::heatR;
}
float Node::getHeatG() {
	return Node::heatG;
}
float Node::getHeatB() {
	return Node::heatB;
}

void Node::reduceHeat() {
	if (heatR>0.05) {
		Node::heatR /= 5;
	}
	if (heatB>0.01) {
		Node::heatB /= 10;
	}
	if (heatG>0.01) {
		Node::heatG /= 10;
	}
}







