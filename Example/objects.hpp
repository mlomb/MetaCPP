#include <string>
#include <vector>

template<typename T>
struct Vector {
	T x, y;
};

class Entity {
public:
	virtual ~Entity() {};

	int health;
	Vector<float> position;
	Vector<float> velocity;
};

class Player : public Entity {
public:
	std::string name;
};

class Monster : public Entity {
public:
	float scary_factor;
};

class Map {
public:
	std::vector<int> magic_numbers;
	std::vector<std::vector<int>> map;
	std::vector<Entity*> entities;
};