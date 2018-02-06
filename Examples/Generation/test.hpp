// This is a dumb file

#include <string>
#include <vector>

template<typename T>
struct Vector {
	T x, y;
};

typedef Vector<int> Vectori;
typedef Vector<float> Vectorf;

struct Target {
public:
	Target() {

	}

	bool attack;
	Vectorf targetPosition;

	Target* target;
	std::vector<Vectori> vics;
	std::vector<Target*> tgts;
};

class Base {
public:
	virtual ~Base() {};

	int base_int = 3;
};

class Derived : public Base {
public:
	int derived_int = 4;
};

struct Entity {
	std::string name;
	int health;

	Vectorf m_Position;
	Vectori m_Velocity;

	std::vector<int> list;
	std::vector<std::vector<int>> two_dim;

	Target* m_TargetA;
	Target* m_TargetB;
	Target** m_Targets;

	Base* m_B;
	Base* m_D;
};