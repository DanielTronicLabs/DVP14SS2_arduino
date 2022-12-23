#ifndef DVP14SS2_h
#define DVP14SS2_h
 
#include "Arduino.h"
 
class DVP14SS2
{
  public:
    DVP14SS2();
    void begin(int, Stream &serial);
    void SlaveAddress(int Slave);
    void writeSingleCoil(char device,int range,int state);
    void writeSingleRegister(char device,int range,int value);
    void Device(char opt,int range);                               //Selecciona el tipo de dispositivo S,X,Y,T,M,C,D
    void RangeXY(int range);
    void RangeN(int range);
    void StateOnOff(int opt);
    void CalculateLRC();
    char DecChar(int num);
  private:
    int _pin;
    Stream* _serial;                                      ///< reference to serial port object
    int  _Slave;                                         ///< Modbus slave (1..32) initialized in begin()
    String _RegWr=":000000000000RR\r\n";                 // trama de datos con los registros a leer/escribir
    int _SumCrc[6];
    int _SumLrc=0;
    int _range;
};
 
#endif
