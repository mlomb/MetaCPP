#include <iostream>

#include "objects.hpp"

#include <MetaCPP/Storage.hpp>
#include <MetaCPP/Runtime.hpp>
#include <MetaCPP/JsonSerializer.hpp>

int main() {
	metacpp::Storage* storage = metacpp::Runtime::GetStorage();
	metacpp::generated::Load(storage);

	Player* player = new Player();
	player->health = 255;
	player->position = {5, 5};
	player->velocity = {1, 1};
	player->name = "mlomb";

	Monster* monster = new Monster();
	monster->health = 255;
	monster->position = {10, 10};
	monster->velocity = {-1, -1};
	monster->scary_factor = 42.123;

	Map map;
	map.entities = {player, monster};

	map.magic_numbers[0][0] = 5;
	map.magic_numbers[0][1] = 7;
	map.magic_numbers[1][0] = 1;
	map.magic_numbers[1][1] = 3;

	map.map = {
			{1, 2, 3},
			{4, 5, 6},
			{7, 8, 9},
	};

	metacpp::JsonSerializer serializer = metacpp::JsonSerializer(storage);

	// serialize
	std::string json = serializer.Serialize(&map, true /* pretty print */);
	std::cout << json << std::endl;

	// deserialize
	Map* deserialized_map = serializer.DeSerialize<Map>(json);

	// serialize again and compare the jsons
	if (serializer.Serialize(deserialized_map, true) == json) {
		std::cout << "The serialization was successful!" << std::endl;
		return 0;
	}
	else {
		std::cout << "The serialization did not match" << std::endl;
		return 1;
	}
}