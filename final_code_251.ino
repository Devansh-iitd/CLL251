#include <Wire.h>
#include <LiquidCrystal_I2C.h>
int ThermistorPin = A7;
int Vo;
float R1 = 10000; // value of R1 on board
float logR2, R2, T;
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; //steinhart-hart coeficients for thermistor

#define MOTOR_PIN_ENA 9  // Enable pin for motor driver (PWM pin)
#define MOTOR_PIN_IN1 10  // Input pin 1 for motor driver
#define MOTOR_PIN_IN2 11  // Input pin 2 for motor driver

#define TEMPERATURE_THRESHOLD 29 
#define TEMPERATURE_THRESHOLD1 32 // Temperature threshold to adjust motor speed

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust the I2C address if necessary

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
}

void loop() {
  Vo = analogRead(ThermistorPin);
  R2 = R1*Vo/(1023-Vo); //calculate resistance on thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // temperature in Kelvin
  T = T - 273.15;

  Serial.print("Temperature: ");
  Serial.print(T);
  Serial.println(" °C");

  lcd.setCursor(0, 1); // Set cursor to the second line
  lcd.print("                "); // Clear the previous motor speed

  // Adjust motor speed based on temperature
  if (T > TEMPERATURE_THRESHOLD1) {
    // Increase motor speed
    analogWrite(MOTOR_PIN_ENA, 200);    // Set the motor speed to maximum (255)
    digitalWrite(MOTOR_PIN_IN1, HIGH);  // Set motor direction (forward)
    digitalWrite(MOTOR_PIN_IN2, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Motor Speed: Max");
  }
  else if (T > TEMPERATURE_THRESHOLD) {
    // Increase motor speed
    analogWrite(MOTOR_PIN_ENA, 100);    // Set the motor speed to a value (150)
    digitalWrite(MOTOR_PIN_IN1, HIGH);  // Set motor direction (forward)
    digitalWrite(MOTOR_PIN_IN2, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Motor Speed: Med");
  }
  else {
    // Decrease motor speed
    analogWrite(MOTOR_PIN_ENA, 45);    // Set the motor speed to a lower value (150)
    digitalWrite(MOTOR_PIN_IN1, HIGH);  // Set motor direction (forward)
    digitalWrite(MOTOR_PIN_IN2, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Motor Speed: Low");
  }

  lcd.setCursor(12, 0); // Set cursor to the temperature position on the first line
  lcd.print(T); // Print temperature

  delay(500); // Delay between readings (adjust as needed)
}
