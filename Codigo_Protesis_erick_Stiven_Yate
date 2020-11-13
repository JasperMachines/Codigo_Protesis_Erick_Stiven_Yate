


//Versión 1.0 por Nicoll Cáceres - Practicante Universidad El Bosque -  Noviembre 2020

#include <Servo.h>

// Declaramos las variables para controlar el servo
Servo servoMotor1;   //Dedo pulgar
Servo servoMotor2;   //Dedo corazón
Servo servoMotor3;   //Dedo indice
Servo servoMotor4;   //Dedo anular y meñique
Servo servoMotor5;   //Muñeca
int AnalogPin = 0;   // Sensor conectado a Analog 0
int angulo = 0;      
int Activacion;     
const int BOTON = 3;
int val = 0; 
int state = 0;
int old_val = 0; // almacena el antiguo valor de val

void setup() {
  Serial.begin(9600);
 
  servoMotor1.attach(5);
  servoMotor2.attach(6);
  servoMotor3.attach(6);
  servoMotor4.attach(8);
  servoMotor5.attach(10);
}

void loop() {
  val= digitalRead(BOTON); // lee el estado del Botón
    if ((val == HIGH) && (old_val == LOW)){
      state=1-state;
    delay(10);
    }
  old_val = val; // valor del antiguo estado
    if (state==1){
     Activacion = analogRead(AnalogPin); // La Resistencia es igual a la lectura del sensor (Analog 0)
      Serial.print("Lectura Analogica = ");
      Serial.println(Activacion);
  if (Activacion>150){
        servoMotor5.write(angulo);
          delay(10);
          angulo++;
         if(angulo>180) angulo = 0;     
     }}
else{
  Activacion = analogRead(AnalogPin); // La Resistencia es igual a la lectura del sensor (Analog 0)
      Serial.print("Lectura Analogica = ");
      Serial.println(Activacion);
      
  if (Activacion>150){
servoMotor1.write(angulo);
servoMotor2.write(angulo);
servoMotor3.write(angulo);
servoMotor4.write(angulo);

     delay(10);
    angulo++;
   if(angulo>180) angulo = 0;
  }
  } 
 }
