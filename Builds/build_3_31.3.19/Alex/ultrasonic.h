/**************************************************************************************************
 * ultrasonic.h
 * Setup ultrasonic sensor
 * Includes both bare-metal and non-bare-metal code
 * 
 * Functions:
 * void setupUS - call to setup US sensor
 * int getDistUS - returns distance of anything in front
**************************************************************************************************/

/*=================================================================================================
  Constants:
  #define USTRIG 12 //Ultrasonic Sensor Trigger Pin
  #define USECHO 13 //Echo Pin
=================================================================================================*/

//Global Variables
long duration;
int distance;

void setupUS() {
  DDRB |= 0b00010000;
  DDRB &= 0b11011111;
}

//bare-metal code
int getDistUS() { //returns the length of an object in front
  PORTB &= 0b11101111; //Clear the trigPin
  _delay_ms(2);
  
  PORTB |= 0b00010000; //Sets the trigPin on HIGH state for 10 micro seconds
  _delay_ms(10);
  PORTB &= 0b11101111; //Clear
  
  duration = pulseIn(USECHO, HIGH); //Reads the echoPin, returns the sound wave travel time in microseconds
  
  distance= duration*0.034/2; //Calculate the distance
  /*if (distance != 0) { //print
    Serial.print("Distance: ");
    Serial.println(distance);
  }*/
  return distance;
}

//non-bare-metal code
/*void setupUS() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

int getDistUS() {
PORTB &= 0b11101111; //Clear the trigPin
  digitalWrite(USTRIG, LOW);
  _delay_ms(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  PORTB |= 0b00010000;
  _delay_ms(10);

  digitalWrite(USECHO, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(USECHO, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  if (distance != 0) {
    Serial.print("Distance: ");
    Serial.println(distance);
  }
  return distance;
}*/
