#pragma once
class CollisionDetection
{
public:
	CollisionDetection();
	~CollisionDetection();
	bool Collision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2, float area);
private:
};

