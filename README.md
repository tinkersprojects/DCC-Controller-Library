# DCC-Controller-Library

[https://tinkersprojects.com/](https://tinkersprojects.com/)

UNTESTED


## Class Functions
### SETUP
#### Dcc(uint8_t _OutputA, uint8_t _OutputB);

### Funtions
#### void begin();
#### size_t set(uint8_t address, uint8_t data);
#### void Send();
#### void SendAll();
#### void flush();




## Example
### Example 1:

```c++
#include <Dcc.h>

Dcc Controller(4,5);

void setup() 
{
  Controller.begin();

}

void loop() 
{
  Controller.set(30, 30);
  Controller.SendAll();
  
  delay(500);
}
```

