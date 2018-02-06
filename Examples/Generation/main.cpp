#include <iostream>

#include "test.hpp"

#include <MetaCPP/Storage.hpp>
#include <MetaCPP/Runtime.hpp>
#include <MetaCPP/TypeInfo.hpp>
#include <MetaCPP/JsonSerializer.hpp>

int main() {
	metacpp::generated::Load(metacpp::Runtime::GetStorage());

	metacpp::Storage* storage = metacpp::Runtime::GetStorage();
	//storage->dump();

	Entity e;
	e.name = "Tank";
	e.health = 199;

	e.m_Position.x = 3.3;
	e.m_Position.y = 4.4;
	e.m_Velocity.x = -4;
	e.m_Velocity.y = -2;

	Target* t = new Target();
	t->attack = true;
	t->targetPosition.x = -99;
	t->targetPosition.y = 99;

	t->vics = { {1,2},{3,4},{5,6} };
	t->tgts = { t,t,t };

	e.m_TargetA = t;
	e.m_TargetB = t;

	e.m_TargetA->target = t;

	e.m_B = new Base();
	e.m_D = new Derived();

	e.list = { 5, 6, 7 };
	e.two_dim = { {0,1,2},{3,4,5},{6,7,8} };

	metacpp::JsonSerializer serializer = metacpp::JsonSerializer(storage, true);

	std::string json = serializer.Serialize(&e, true);
	std::cout << json.length() << std::endl;
	std::cout << json << std::endl;

	std::cout << "---------------------------------------------" << std::endl;

	Entity* e2 = serializer.DeSerialize<Entity>(json);
	std::cout << serializer.Serialize(e2, true) << std::endl;

	system("pause");

	return 0;
}