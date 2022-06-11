//#define BLUETOOTH
#define SERVO
#define UNO
#define DEBUG
//#define RASPBERRY
//#define WIFI
#define MATHMODEL
#define PI 3.14159
//variables of the chess board and arm in mm
#define CHESSX 270
#define CHESSY 270
#define DELTAY 72
#define ARML 125.0
#define BOXLENGTH 33.5625


//Initialization of the Servos
#ifdef SERVO
  #include <Servo.h> // Include the servo library:
  #define ServoNumber 6 //defines numbers of servos used in the application
  byte ServoPins[6] = {3,5,6,9,10,11}; //PWM pins for the servo ; here maximum is 6
  Servo servos[ServoNumber]; //objects creation of the Servos
  int number; //function argument
  int pos; //function argument
  byte last_position[ServoNumber] = {90,90,90,90,90,90};//initial Servo position
#endif

//initialization of the Bluetooth ; we have used HC-05
#ifdef BLUETOOTH
  #include <NeoSWSerial.h>
  NeoSWSerial bt(2,4); //Arduino : RX,TX
  String dataIn = "";
#endif

//Initialization of the WIFI module we have used the ESP-8266
#ifdef WIFI
  #include <NeoSWSerial.h>
  SoftwareSerial wifi(2,4); //Arduino : RX,TX
  String dataIn = "";
#endif


void setup() 
{
  //Assignes the pin to the Motors
  #ifdef SERVO
  for(byte i = 0; i <ServoNumber; i++)
  {
    servos[i].attach(ServoPins[i]);
    delay(100);
    movement(i,last_position[i]); //go to initial position
  }
  #endif

  //Initialization of Bluetooth Software serial
  #ifdef BLUETOOTH
  bt.begin(9600);
  bt.setTimeout(1);
  delay(20);
  #endif

  //Initialization of the WIFI Software serial
  #ifdef WIFI
  wifi.begin(9600);
  wifi.setTimeout(1);
  delay(20);
  #endif

  #ifdef DEBUG
  Serial.begin(9600);
  delay(100);
  Serial.println("Program Started");
  #endif


}

void loop() 
{
//check whether bluetooth data is available
 #ifdef BLUETOOTH
 if(bt.available()  > 0)checkData(); //if serial data available call checkdata function
 delay(10);
 #endif

//checkData of wifi
 #ifdef WIFI
 if(wifi.available() > 0)checkData();
 #endif
 //float radians = asin(0.707);
 //float degrees = (radians*180)/PI; //converts the value to the degrees
#ifdef DEBUG
positionCalculator(83.9,115.9);
#endif


}

void movement(byte number, byte degree)
{
  Serial.print("Required pos : ");
  Serial.println(degree);
  Serial.print("Last position: ");
  Serial.println(last_position[number]);
  if (last_position[number] > degree)
  {
    Serial.print("Decreasing POS");
    for(int i = last_position[number]; i>=degree; i--)
    {
      servos[number].write(i);
      delay(20);
      Serial.println(i);
    }
  }
  if (last_position[number] < degree)
  {
    Serial.print("Increasing POS");
    for(int i = last_position[number];i<degree;i++)
    {
      servos[number].write(i);
      delay(20);
      Serial.println(i);
    }
  }
  last_position[number] = degree;
  delay(10);
  Serial.println("****************************");

}
void checkData()
{
  //works for data in format of ":servo_num=position;"
  #ifdef BLUETOOTH
  bt.find(':');
  dataIn = bt.readStringUntil(';');
  #endif
  #ifdef WIFI
  wifi.find(':');
  dataIn = wifi.readStringUntil(';');
  #endif
  #ifdef WIFI || BLUETOOTH
  number = dataIn.substring(0,dataIn.indexOf('=')).toInt();
  pos = dataIn.substring(dataIn.indexOf('=')+1,dataIn.length()).toInt();
  #endif
  #ifdef SERVO
  if (number<ServoNumber && pos<=180);
  movement(number,pos);
  #endif
}


/*
calcultes the inverse of the tan,sin and cos in radians
*/

float asin(float c)
{

float out;

out= ((c+(c*c*c)/6+(3*c*c*c*c*c)/40+(5*c*c*c*c*c*c*c)/112 +

(35*c*c*c*c*c*c*c*c*c)/1152 +(c*c*c*c*c*c*c*c*c*c*c*0.022) +

(c*c*c*c*c*c*c*c*c*c*c*c*c*.0173)+(c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*.0139) +

(c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*0.0115)+(c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*0.01)

));

//asin

if(c>=.96 && c<.97){out=1.287+(3.82*(c-.96)); }
if(c>=.97 && c<.98){out=(1.325+4.5*(c-.97)); } // arcsin

if(c>=.98 && c<.99){out=(1.37+6*(c-.98)); }

if(c>=.99 && c<=1){out=(1.43+14*(c-.99)); }

return out;
}

float acos(float c)
{
  float out;
  out=asin(sqrt(1-c*c));
  return out;
}

float atan(float c)
{
  float out;
  out=asin(c/(sqrt(1+c*c)));
  return out;
}

void positionCalculator(float x, float y)
{
  float theta = asin((y+DELTAY)/sqrt((y+DELTAY)*(y+DELTAY) + (x*x))); //in radians
  theta = (theta*180)/PI;

  float alpha = acos(sqrt((y+DELTAY)*(y+DELTAY) + (x*x))/(2*ARML));
  alpha = (alpha*180)/PI;

  float beta = 180 - (2*alpha);



  #ifdef SERVO
  //movement(0,theta);
  #endif
  #ifdef DEBUG
  Serial.print("Theta : ");
  Serial.print(round(theta));
  Serial.print(" ; ");
  Serial.print("alpha : ");
  Serial.print(round(alpha));
  Serial.print(" ; ");
  Serial.print("beta : ");
  Serial.print(round(beta));
  Serial.println(" ; ");
  #endif
}