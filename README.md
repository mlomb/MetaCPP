# MetaCPP - Reflection & Serialization
The objective is to generate reflection information and enable serialization/deserialization with the least or -zero- *manual* code possible.

Note that this project is not production ready, there are many bugs and lacks proper testing.

## Example
You can find the definition of the following objects in [Example/objects.hpp](Example/objects.hpp).
```C++
// load the auto-generated metadata
metacpp::Storage* storage = metacpp::Runtime::GetStorage();
metacpp::generated::Load(storage);

Player* player = new Player();
player->health = 255;
player->position = { 5, 5 };
player->velocity = { 1, 1 };
player->name = "mlomb";

Monster* monster = new Monster();
monster->health = 255;
monster->position = { 10, 10 };
monster->velocity = { -1, -1 };
monster->scary_factor = 42.123;

Map map;
map.entities = { player, monster };

map.magic_numbers = { 4, 2 };

map.map = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 },
};

metacpp::JsonSerializer serializer = metacpp::JsonSerializer(storage);

// serialize
std::string json = serializer.Serialize(&map, true /* pretty print */);

std::cout << json << std::endl;

// deserialize
Map* deserialized_map = serializer.DeSerialize<Map>(json);
```
The code above spits out a JSON like this:
```JSON
[{
    "magic_numbers": [4, 2],
    "map": [[1, 2, 3], [4, 5, 6], [7, 8, 9]],
    "entities": [1, 2, null, ...]
}, {
    "reflection_class": "Player",
    "health": 255,
    "position": {
        "x": 5.0,
        "y": 5.0
    },
    "velocity": {
        "x": 1.0,
        "y": 1.0
    },
    "name": "mlomb"
}, {
    "reflection_class": "Monster",
    "health": 255,
    "position": {
        "x": 10.0,
        "y": 10.0
    },
    "velocity": {
        "x": -1.0,
        "y": -1.0
    },
    "scary_factor": 42.12300109863281
}]
```
You can find the full code for this example [here](Example/).

## Setup
You can set it up using CMake.
First clone the repository under your project directory.

Then, inside your `CMakeLists.txt` add the following:
```CMake
add_subdirectory(MetaCPP/MetaCPP-CLI)

...

# MetaCPP automatic reflection
include(MetaCPP/CMake/MetaPrebuild.cmake)
meta_generate(YOUR_TARGET "FileToReflect.hpp" "Generated.hpp" "Generated.cpp" "")
```
Next and last, include the following lines at the begining of your application to load all the metadata:
```C++
#include <MetaCPP/Runtime.hpp>

int main() {
  metacpp::Storage* storage = metacpp::Runtime::GetStorage();
  metacpp::generated::Load(storage);
 
  ...
}
```

## Dependencies
MetaCPP relies on Clang's LibTooling to generate all the metadata.  
These are the dependencies:
- [Clang's Libtooling](https://clang.llvm.org/docs/LibTooling.html) to gather metadata (only for CLI)
- [Mustache](https://github.com/kainjow/Mustache) to generate the metadata source code (only for CLI)
- [rapidjson](https://github.com/Tencent/rapidjson) to enable JSON serialization

Mustache and rapidjson are included as submodules so don't forget to clone with `--recursive`.

If you need to compile the CLI make sure to have installed Clang's Libtooling. In Windows you should have in `PATH` the Clang and LLVM binaries.

## Known issues

* Arrays of pointers are broken ([#pr-3](https://github.com/mlomb/MetaCPP/pull/3#issuecomment-716345878))

## License
See [LICENSE](LICENSE).
