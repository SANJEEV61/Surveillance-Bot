#include <SoftwareSerial.h>
SoftwareSerial SIM900(11,12);
#include <Servo.h>
#include <TinyGPS++.h>
 
#define echoPin 6
#define trigPin 7
long duration;
int distance;
 
char t;
int leftUSval;
int rightUSval;


#define IN_1 2
#define IN_2 3
#define IN_3 4
#define IN_4 5

#define pump 8
#define buzzer A0
#define irsensor A1

int L;
int R;

String long_lat;

String Link;
String SMS;

float latitude=13.1220;
float longitude=77.6105;

TinyGPSPlus gps;

static const uint32_t GPSBaud = 9600;

//float latitude;
//float longitude;
Servo servo;

int get_location(String message){
  //    delay(500);
    
  SIM900.print("ATD +919740824200;\r");
  delay(1000);

  SIM900.print("AT+CMGF=1\r");     // AT command to set SIM900 to SMS mode
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r");       // Set module to send SMS data to serial out upon receipt
  delay(100);
    
  SIM900.println("AT+CMGF=1"); // Replace x with mobile number
  delay(1000);
  SIM900.println("AT+CMGS= \"+919740824200\"\r"); // Replace * with mobile number  sim number - 8861273413
  delay(1000); 
  SIM900.println(message);// The SMS text you want to send
  delay(100);
  SIM900.println((char)26);// ASCII code of CTRL+Z
  
} 

void GPS()
{
if (gps.charsProcessed() < 10)
  {
    //Serial.println("No GPS detected: check wiring.");
     // Blynk.virtualWrite(V4, "GPS ERROR");         // Value Display widget  on V4 if GPS not detected
  }
}
void displaygpsInfo()
{ 

  if (gps.location.isValid() ) 
  {
    
     latitude = (gps.location.lat());      //Storing the Lat. and Lon. 
     longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);                // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6); 

      }
}
 

void locate()
{
  while (Serial.available() > 0) 
    {
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(Serial.read()))
        displaygpsInfo();
  }
}


void goForward(){ 
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
  }

void goBack(){ 
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
  }

void goRight(){ 
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
  }

void goLeft(){
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
  }

void hold(){  
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
 }

 

int ultrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;
 
  return distance;
}

void setup() {
  Serial.begin(9600);  
  SIM900.begin(9600);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 pinMode(pump, OUTPUT);
 pinMode(buzzer, OUTPUT);
   pinMode(irsensor, INPUT);
  servo.attach(9);
}

void loop() {
//  locate();
  int val=digitalRead(irsensor);
  BTcontrol();////////////////////BLUETOOTH CONTROL
 distance = ultrasonic();/////ULTRASONIC CODE
 Serial.println(distance);
 distance = duration * 0.034 / 2;
  if (distance==7) 
  {
     hold();
     servo.write(90);
     digitalWrite(pump,HIGH);-
     delay(5000);
     digitalWrite(pump,LOW);    
     servo.write(180);
   
   long_lat = String(float(latitude))+","+String(float(longitude));
   Link = "https://www.google.com/maps/search/?api=1&query="+String(long_lat);
   SMS = "Alert: potholes detected "+ long_lat + " " + Link;//////////////////////////////SMS message
   get_location(SMS);
   Serial.println(long_lat);
   } 
  
  else 
  {
     digitalWrite(pump,LOW);    
     servo.write(180);
  }
  
  if(val==1)   ///IR SENSOR
  {
  digitalWrite(buzzer,LOW);  
  }
  else
  {
    digitalWrite(buzzer,HIGH);  
    }
     
//  Obstacle();
//  if(digitalRead(mode)==0){
//  Obstacle(); 
//  }
//  else{
//    automatic();
//  }
}

void BTcontrol() {
if(Serial.available()){
  t = Serial.read();
  Serial.println(t);
}
 
if(t == 'F'){            //move forward(all motors rotate in forward direction)
  goForward();
}
 
else if(t == 'B'){      //move reverse (all motors rotate in reverse direction)
  goBack();
}
 
else if(t == 'L'){      //turn right (left side motors rotate in forward direction, right side motors doesn't rotate)
  goLeft();
}
 
else if(t == 'R'){      //turn left (right side motors rotate in forward direction, left side motors doesn't rotate)
  goRight();
}

else if(t == 'S'){      //STOP (all motors stop)
  hold();
}
}


 
