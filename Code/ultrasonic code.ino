#define echoPin A4
#define trigPin A5

#define relay 5

#define ir 7
#define buz 2
#define red 12
#define wh 10


long duration;
float distance; 

 void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relay,OUTPUT);

  pinMode(buz, OUTPUT);
  pinMode(ir, INPUT);
  pinMode(red,OUTPUT);
   pinMode(wh,OUTPUT);

   

  digitalWrite(buz,LOW);
  
  digitalWrite(wh,HIGH);
  
}


 void loop()
  {

   int val= digitalRead(ir);

   if(val==0){

     Serial.print("Garbage detected ");

     digitalWrite(buz, HIGH);
     delay(100);
   }
   
   else{
        digitalWrite(buz, LOW);
        delay(100);
      
   }


  digitalWrite(trigPin, LOW);
    
  

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;
  
if(distance<10){



  Serial.print("Abnormal or Problematic Road Detected!");

  digitalWrite(relay,HIGH);
   digitalWrite(red,HIGH);

  

  delay(100);
}

else{


  Serial.print(" No Abnormal Roads Detected! ");
  digitalWrite(relay,LOW);
   digitalWrite(red,LOW);
  
   digitalWrite(wh,HIGH);
  delay(100);
  
}

delay(500);
  Serial.print("distance is");
  Serial.println(distance);

  delay(100);


}