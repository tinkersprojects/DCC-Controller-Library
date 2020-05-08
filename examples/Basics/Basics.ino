
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