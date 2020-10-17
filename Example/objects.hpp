#include <string>
#include <vector>
#include <array>

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

#pragma pack(push, 1)
class Map {
public:
    //std::vector<int> magic_numbers;
	char magic_numbers[3];
	std::vector<std::vector<int>> map;
	//std::vector<Entity*> entities;
	std::array<Entity*, 16> entities;
};
#pragma pack(pop)