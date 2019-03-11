#include <Servo.h>
#include <Wire.h>
#define SLAVE_ADDRESS 0x18

Servo myservo;  // create servo object to control a servo

int BOOST = A0;  // analog pin for reading boost pressure sensor
int MOTPOWER = A1;  // analog pin for motfueling
int MAXPOWER = A2;  // analog pin for maxfueling
int BOOSTVAL = 0;    // variable to read the value from boost pressure sensor
int BOOSTPROS = 0;
int FUELVAL = 0;  // variable for fuel quanity
int MOTVAL = 0;
int MAXVAL = 0; 
int number = 0;
                     //BAR 0             1,25           2,75                4                   5,25                6,5                 7,75                9                   10,2
                         //0             5              10                  15                  20                  25                  30                  35                  40
int MULTIPLIERMAX []    = {5,10,15,20,25,30,35,45,60,80,100,120,140,160,170,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180};

                     //BAR 0          1,25           2,75           4              5,25           6,5            7,75           9                   10,2
                         //0          5              10             15             20             25             30             35                  40
int MULTIPLIERMOT []    = {1,3,6,9,11,14,17,20,23,26,29,32,35,38,41,44,47,50,53,56,59,61,64,67,70,73,76,79,81,84,87,90,93,96,99,102,105,108,111,114,117};

                     //BAR 0           1,25           2,75                4                   5,25                6,5                 7,75                9                   10,2
                         //0           5              10                  15                  20                  25                  30                  35                  40
int MULTIPLIERCRUISE [] = {2,4,8,16,24,34,44,60,74,88,102,115,127,136,143,148,154,160,164,168,172,176,179,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180};


void setup() {
Serial.begin(9600);
// initialize i2c as slave
Wire.begin(SLAVE_ADDRESS);

// define callbacks for i2c communication
Wire.onReceive(receiveData);
Wire.onRequest(sendData);

Serial.println("Ready!"); 
myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
BOOSTVAL = analogRead(BOOST);    	
MOTVAL = digitalRead (MOTPOWER);
MAXVAL = digitalRead (MAXPOWER);
	
if (MOTVAL == HIGH){
  BOOSTPROS = map(BOOSTVAL, 102.3, 920.7, 0, 40);
	FUELVAL = MULTIPLIERMOT [BOOSTPROS];
	myservo.write(FUELVAL);
	}       
	
if (MAXVAL == HIGH){
	BOOSTPROS = map(BOOSTVAL, 102.3, 920.7, 0, 40);
	FUELVAL = MULTIPLIERMAX [BOOSTPROS];
	myservo.write(FUELVAL);
	}
  
else {
  BOOSTPROS = map(BOOSTVAL, 102.3, 920.7, 0, 40);
  FUELVAL = MULTIPLIERCRUISE [BOOSTPROS];
  myservo.write(FUELVAL); 
  }
}
// callback for received data
  void receiveData(int byteCount){
    while(Wire.available()) {
      
    number = Wire.read();
     }
  }

// callback for sending data
void sendData(){

if (number == 1){
    Wire.write(FUELVAL);
}


else{
    Wire.write(0);

}
}
