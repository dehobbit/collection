//Using the TM4C123G Launchpad Board

const int joystick_x_pin = A7;  
const int joystick_y_pin = A6;
bool d1 = HIGH;
bool d2 = LOW;

//Declaring pin outputs
void setup() {pinMode(19, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(38, OUTPUT);
  Serial.begin(9600); /* Define baud rate for serial communication */
}

/*Set loop where it will constantly read the input from joystick and based on
those reading move the treads*/
void loop() {
  int x_adc_val, y_adc_val; 
  int motorSpeedA, motorSpeedB;
  float x_volt, y_volt;
  x_adc_val = analogRead(joystick_x_pin);
  y_adc_val = analogRead(joystick_y_pin);
  x_volt = ( ( x_adc_val * 3.3 ) / 1023 );  /*Convert digital value to voltage */
  y_volt = ( ( y_adc_val * 3.3 ) / 1023 );  /*Convert digital value to voltage */
  Serial.print("X_Voltage = ");
  Serial.print(x_volt);
  Serial.print("\t");
  Serial.print("Y_Voltage = ");
  Serial.println(y_volt);

  //Forward
  if (y_volt < 6 && x_volt > 6 && x_volt < 7) {
    //Motor A forwards
    digitalWrite(31, d1);
    digitalWrite(40, d2);
    //Motor B backwards
    digitalWrite(39, d1);
    digitalWrite(38, d2);
    motorSpeedA = y_volt; 
    motorSpeedB = y_volt;
  }

  //Backwards
  else if (y_volt >7 && x_volt > 6 && x_volt < 7) {
    //Motor A backwards
    digitalWrite(31, d2);
    digitalWrite(40, d1);
    //Motor B forwards
    digitalWrite(39, d2);
    digitalWrite(38, d1);
    motorSpeedA = y_volt; 
    motorSpeedB = y_volt;
  }

  //Right Turn
  else if (x_volt > 7 && y_volt > 6 && y_volt < 7) {
    //Motor A forwards
    digitalWrite(31, d1);
    digitalWrite(40, d2);
    //Motor B forwards
    digitalWrite(39, d2);
    digitalWrite(38, d1);
    motorSpeedA = x_volt; 
    motorSpeedB = x_volt;
  }

  //Left Turn
  else if (x_volt < 6 && y_volt > 6 && y_volt < 7) {
    //Motor A backwards
    digitalWrite(31, d2);
    digitalWrite(40, d1);
    //Motor B backwards
    digitalWrite(39, d1);
    digitalWrite(38, d2);
    motorSpeedA = x_volt; 
    motorSpeedB = x_volt;
  }

  //Joystick in middle position so no movement
  else {
    digitalWrite(31, d2);
    digitalWrite(40, d2);
    digitalWrite(39, d2);
    digitalWrite(38, d2);
    motorSpeedA = 0;
    motorSpeedB = 0;
  }
  analogWrite(19, motorSpeedA);
  analogWrite(37, motorSpeedB);
}
