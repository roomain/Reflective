#Reflective
Reflective is a tiny library to deserialize json data into c++ structure.
The json file describe structures for specific profiles. 
A profile can herits and overload from another one.
```json
{
  "$shema" : "not_yet_rated",
  "title" : "reflective",
  "include": [],
  "ClassArray": [
    {
      "Id": {
        "name": "SimpleReflective",
        "parent": null
      },
      "Simple": {
        "m_bool": true,
        "m_int": 5,
        "m_string": "Hello world",
        "m_floatVector": [ 5, 6.3, 8.1, 9 ]
      }
    },
    {
      "Id": {
        "name": "OverloadAndHeritage",
        "parent": "SimpleReflective"
      },
      "Simple": {
        "m_bool": false,
        "m_int": 8,
        "m_floatVector": [ 4.5, 3 ]
      }
    }
  ]
}
``` 
## Features
- script to generate data used for deserialisation
- loading json file and choosing profile

## Basic Usage
Reflective usage is easy to use:
- First create your structure with specific macro.
Your file must include the future generated file named <your_filename>.generated.h
```cpp
#pragma once
#include <string>
#include <vector>
#include "Reflective_macros.h"              // for reflective macros
#include "Simple.generated.h"     // generated file

REFLECT_CLASS
struct Simple
{
	bool m_bool;
	int m_int;
	std::string m_string;
	std::vector<float> m_floatVector;

	REFLECT_DEFINE(Simple)

};
REFLECT_IMPL(Simple)
```

- Second: run the script with the folder containing your files as parameter
```bat
cd C:\Projects\Reflective\script
python reflective.py C:\Project\MyReflective\
```
The script will generate *.generated.h files.

- To deserialize you have to load json file via singleton 'Reflective':
```cpp
Reflective::instance().loadFile(R"(..\..\Test_data\reflective.json)");
```

- Next you must set your profile.
```cpp
Reflective::instance().setProfile("SimpleReflective");
```
You can check the presence of profile in json file via
```cpp
Reflective::instance().hasProfile("SimpleReflective");
```

- Then just instanciate your structure.


## Limitation
- Reflective can't deserialize data for std::variant.