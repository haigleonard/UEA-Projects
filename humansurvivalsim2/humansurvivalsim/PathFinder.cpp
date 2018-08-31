#include "Model/PathFinder.h"
#include <math.h>
#include "Model/Map.h"



PathFinder::PathFinder()
{
}


PathFinder::~PathFinder()
{
}

double PathFinder::manhattan_distance(int x1, int y1, int x2, int y2)
{
	double val1 = std::abs(x1 - x2);
	double val2 = std::abs(y1 - y2);
	return val1 + val2;
}


std::vector<Node> PathFinder::path(Node start, Node goal, Map m) {

	std::vector<Node> optimal_path;
	std::unordered_map<Node, Node, Node::NodeHash> cameFrom;
	std::vector<Node*> open;
	std::vector<Node*> closed;
	Node current;

	m.getNode(start.getX(), start.getY())->setGScore(0.0);

	m.getNode(start.getX(), start.getY())->setFScore(sqrt(pow(start.getX() - goal.getX(), 2) + pow(start.getY() - goal.getY(), 2)));

	open.push_back(m.getNode(start.getX(), start.getY()));

	while (!open.empty()) {

		std::sort(open.begin(), open.end(), Node::asc_sort);

		Node* curr = new Node(open[0]);
		
		if (curr->getX() == goal.getX() && curr->getY() == goal.getY()) {			
			optimal_path = PathFinder::reconstruct_path(cameFrom,  goal);		
			break;
		}

		open.erase((begin(open) + 0));

		closed.push_back(curr);

		for (int x = -1; x < 2; x++) {
			for (int y = -1; y < 2; y++) {

				bool check = (curr->getX() + x >= 0 && curr->getY() + y >= 0) && (curr->getX() + x < m.getNumCols() && curr->getY() + y < m.getNumCols());
				
				if (check) {

					Node visiting = Node(m.getNode(curr->getX() + x, curr->getY() + y));

					
						bool valid = PathFinder::is_valid_neighbour(visiting);

						if (valid) {

							if (findNode(&visiting, closed)) { //element in closed list																
								continue;
							}

							if (!findNode(&visiting, open)) { //element not in open list. Must evaluate															
								open.push_back(m.getNode(visiting.getX(), visiting.getY()));
							}


							double tentative_gScore = m.getNode(curr->getX(), curr->getY())->getGScore() + manhattan_distance(curr->getX(), curr->getY(), visiting.getX(), visiting.getY());
							

							double vistingGScore = m.getNode(visiting.getX(), visiting.getY())->getGScore();
							if (tentative_gScore >= vistingGScore) {
								continue;
							}
							else {
								if ((curr->getX() - visiting.getX() > 0 && curr->getY() - visiting.getY() == 0) ||
									(curr->getX() - visiting.getX() < 0 && curr->getY() - visiting.getY() == 0) ||
									(curr->getX() - visiting.getX() == 0 && curr->getY() - visiting.getY() > 0) ||
									(curr->getX() - visiting.getX() == 0 && curr->getY() - visiting.getY() < 0)) {
									current.setX(curr->getX());
									current.setY(curr->getY());
									cameFrom[visiting] = current;
									m.getNode(visiting.getX(), visiting.getY())->setGScore(tentative_gScore);
									m.getNode(visiting.getX(), visiting.getY())->setFScore(manhattan_distance(visiting.getX(), visiting.getY(), goal.getX(), goal.getY()));
								}
																								
							}
						}
					}
				
				else {
					continue;
				}
			}

		}
	}
	
	return optimal_path;

}

std::vector<Node> PathFinder::reconstruct_path(std::unordered_map<Node, Node, Node::NodeHash> cameFrom, Node current)
{
	std::vector<Node> optimal_path;
	optimal_path.push_back(current);

	std::unordered_map<Node, Node, Node::NodeHash>::iterator it;

	while ((it = cameFrom.find(current)) != cameFrom.end()) {		
		current = cameFrom[current];
		optimal_path.push_back(current);

	}

	return optimal_path;
}

bool PathFinder::is_valid_neighbour(Node visiting) {
	return visiting.getValue() == 1 || visiting.getValue() == -4;
}

bool PathFinder::findNode(Node * node, std::vector<Node*> nodes)
{
	for (int i = 0; i < nodes.size(); i++) {

		Node * temp = nodes[i];

		if (temp->getX() == node->getX() && temp->getY() == node->getY()) return true;

	}
	return false;
}

