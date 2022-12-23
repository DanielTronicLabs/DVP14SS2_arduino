#include "Arduino.h"
#include "DVP14SS2.h"
 
DVP14SS2::DVP14SS2()
{
 
}

void DVP14SS2::begin(int Slave, Stream &serial)
{
  _Slave = Slave;
  _serial = &serial;
   SlaveAddress(_Slave);
}
 
void DVP14SS2::SlaveAddress(int Slave)
{
 int num1=Slave/10;
 int num2=Slave%10;
 _RegWr[1]=DecChar(num1);// direccion sclavo ADR1
 _RegWr[2]=DecChar(num2);// direccion sclavo ADR0
 _SumCrc[0]=Slave;
}

void DVP14SS2::writeSingleCoil(char device,int range,int state)
{
 _RegWr[3]='0';// codigo de comando CMD1
 _RegWr[4]='5';// codigo de comando CMD0 5 force single coil
 _SumCrc[1]=5;
 _range=range;
 Device(device,_range);
 StateOnOff(state);
 CalculateLRC();
 _serial->print(_RegWr); 
}
void DVP14SS2::writeSingleRegister(char device,int range,int value)
{
 _RegWr[3]='0';// codigo de comando CMD1
 _RegWr[4]='6';// codigo de comando CMD0 5 force single coil
 _SumCrc[1]=6;
 _range=range;
 Device(device,_range);

  _RegWr[9]='0';   // numero de datos Hi1
  _RegWr[10]='0';  // numero de datos Hi2
  _RegWr[11]='F';   // numero de datos Lo1
  _RegWr[12]='F';  // numero de datos Lo2
  _SumCrc[4]=0;
  _SumCrc[5]=255;

 CalculateLRC();
 _serial->print(_RegWr); 
}

void DVP14SS2::Device(char opt,int range) //Selecciona el tipo de dispositivo S,X,Y,T,M,C,D
{
switch(opt){
case 'S':      // S0-S255
 _RegWr[5]='0';// inicio direccion Hi1
 _RegWr[6]='0';// inicio direccion Hi2
 _SumCrc[2]=0;
 RangeN(range);
break;
case 'X':      // X0-X255
 _RegWr[5]='0';// inicio direccion Hi1
 _RegWr[6]='4';// inicio direccion Hi2
 _SumCrc[2]=4;
 RangeN(range);
break;
case 'Y':      // Y0-Y255
 _RegWr[5]='0';// inicio direccion Hi1
 _RegWr[6]='5';// inicio direccion Hi2
 _SumCrc[2]=5;
 RangeXY(range);
break;
case 'T':      // T0-T255
 _RegWr[5]='0';// inicio direccion Hi1
 _RegWr[6]='6';// inicio direccion Hi2
 _SumCrc[2]=6;
 RangeN(range);
break;
case 'M':      // M0-M255
 _RegWr[5]='0';// inicio direccion Hi1
 _RegWr[6]='8';// inicio direccion Hi2
 _SumCrc[2]=8;
 RangeN(range);
break;
case 'C':      // C0-C255
 _RegWr[5]='0';// inicio direccion Hi1
 _RegWr[6]='E';// inicio direccion Hi2
 _SumCrc[2]=14;
 RangeN(range);
break;
case 'D':      // D0-D255
 _RegWr[5]='1';// inicio direccion Hi1
 _RegWr[6]='0';// inicio direccion Hi2
 _SumCrc[2]=16;
 RangeN(range);
break;
}
}

void DVP14SS2::RangeXY(int range){ //calculo para entradas X y salidas Y  
  int num1=range/10;
  int num2=range%10;
  int result=0;
  if(num1>1){
   result=num1-1;
   }
  _RegWr[7]=DecChar(result);// inicio direccion Lo1
  _RegWr[8]=DecChar(num2);  // inicio direccion Lo2
  _SumCrc[3]=result*16+num2;// hex a dec
}

void DVP14SS2::RangeN(int range){ //calculo para entradas X y salidas Y  
 
   int num1=range/16;
   int num2=range%16;
   if(range>15){
     _RegWr[7]=DecChar(num1);// inicio direccion Lo1
     _RegWr[8]=DecChar(num2);  // inicio direccion Lo2
   }
   else{
     _RegWr[7]=DecChar(0);// inicio direccion Lo1
     _RegWr[8]=DecChar(num2);  // inicio direccion Lo2
   }
  _SumCrc[3]=range;
}

void DVP14SS2::StateOnOff(int opt){// control ON/OFF 
if(opt==1){
  _RegWr[9]='F';   // numero de datos Hi1
  _RegWr[10]='F';  // numero de datos Hi2
  _RegWr[11]='0';   // numero de datos Lo1
  _RegWr[12]='0';  // numero de datos Lo2
  _SumCrc[4]=255;
  _SumCrc[5]=0;
}
else{
  _RegWr[9]='0';   // numero de datos Hi1
  _RegWr[10]='0';  // numero de datos Hi2
  _RegWr[11]='0';   // numero de datos Lo1
  _RegWr[12]='0';  // numero de datos Lo2
  _SumCrc[4]=0;
  _SumCrc[5]=0;
}
}

void DVP14SS2::CalculateLRC()// calcula el complemento 2 del LRC CHK check sum
{
 int i=0;
 int SumLrc=0;
 int complement2=0;
 int val=0;
 int cont=0;
 int exp=0;
 int num1=0;
 int num2=0;
 for(i=0;i<6;i++){
  SumLrc=SumLrc+_SumCrc[i];
 }
 val=SumLrc;
 while(val>=1){
  val=val/2;
  cont++;
 } 
 if(cont>8){
  exp=pow(2,cont)+1;
  exp=512;
  complement2=exp-SumLrc;
 }
 else{
  complement2=256-SumLrc;
 }
 num1=complement2/16;
 num2=complement2%16;
 _RegWr[13]=DecChar(num1);   // LRC1
 _RegWr[14]=DecChar(num2);   // LRC2

 //_serial->print(complement2); 
}

char DVP14SS2::DecChar(int num)
{
char numero=0;
switch(num){
case 0:
numero='0';
break;
case 1:
numero='1';
break;
case 2:
numero='2';
break;
case 3:
numero='3';
break;
case 4:
numero='4';
break;
case 5:
numero='5';
break;
case 6:
numero='6';
break;
case 7:
numero='7';
break;
case 8:
numero='8';
break;
case 9:
numero='9';
break;
case 10:
numero='A';
break;
case 11:
numero='B';
break;
case 12:
numero='C';
break;
case 13:
numero='D';
break;
case 14:
numero='E';
break;
case 15:
numero='F';
break;
}
return numero;
}

