// By DanielTronic Labs  8/12/2022
// Force single coil S,Y,M,T,C
// outputs DVP14SS2 Y0-Y6 module extern 1 Y20-Y27 module extern 2 Y30-Y37
// M0-M255  S0-S255 T0-255 C0-C255 
//
// dvp.writeSingleCoil('S',0,ON); // S0 ON
// dvp.writeSingleCoil('S',0,OFF);// S0 OFF
// dvp.writeSingleCoil('Y',1,ON); // Y1 ON
// dvp.writeSingleCoil('Y',1,OFF);// Y1 OFF
// dvp.writeSingleCoil('M',2,ON); // M2 ON
// dvp.writeSingleCoil('M',2,OFF);// M2 OFF
// dvp.writeSingleCoil('T',0,ON); // T0 ON
// dvp.writeSingleCoil('T',0,OFF);// T0 OFF
// dvp.writeSingleCoil('C',C,ON); // C3 ON
// dvp.writeSingleCoil('C',C,OFF);// Y3 OFF


#include <DVP14SS2.h>
#define ON  1
#define OFF 0

DVP14SS2 dvp;// instantiate DVP14SS2 object

void setup()
{
  Serial.begin(9600);  // use Serial (port 0); initialize Modbus communication baud rate
  dvp.begin(1, Serial); // communicate with Modbus slave ID 1 over Serial (port 2)
}


void loop() {
dvp.writeSingleCoil('Y',0,ON);// Y0 ON
delay(1000);
dvp.writeSingleCoil('Y',0,OFF);// Y0 OFF
delay(1000);
}
