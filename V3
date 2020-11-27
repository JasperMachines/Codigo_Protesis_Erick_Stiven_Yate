#include <Servo.h>

//////////////// DECLARACION DE ESTRUCTURA PARA POSICIONES
struct Mano{

  int muneca;
  int posMUN; 
  int pulgar;
  int indice;
  int medio;
  int anular_menique;
  
} ManoIzquierda;


//////////////// DECLARACION DE ACTUADORES/MOTORES 

Servo muneca;
Servo pulgar;
Servo indice;                  //DECLARACION DE SERVOS PARA MOVIMIENTO DE DEDOS Y MUÑECA
Servo medio;
Servo anular_menique;

/////////////// ASIGNACION DE PINES PARA LED RGB INDICADOR

int redPin = 9;
int greenPin = 11;                           
int bluePin = 10;  

/////////////// VARIABLES DE SENSOR DE ACTIVIDAD MUSCULAR Y PARA FILTRADO DE SEÑAL (PROMEDIO DE X MUESTRAS)

int sensor = 0;
int input=0;
int i;
const int numReadings = 20;     //numerode lecturas a promediar

int readings[numReadings];      // array de lecturas
int readIndex = 0;              // indice de lectura actual
int total = 0;                  // total de lecturas

//////////////// DECLARACION DE FUNCIONES AUXILIARES DE POSICION E INDICADOR LED
void RelaxPosition();
void TripodPosition();

void RGBLED (int red, int green, int blue );

int SensorRead();

int SensorRead(){

 total = total - readings[readIndex];        
  readings[readIndex] = analogRead(A3);
  total = total + readings[readIndex];        // SE TOMA EL PROMEDIO DE 10 MUESTRAS DE LA SEÑAL PARA TENER UNA LECTURA ESTABLE           
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
    
  }

  sensor = map((total / numReadings), 0, 1020, 10000, 0);   // SE AMPLIFICA LA SEÑAL 10 VECES PARA HACER MAS FACIL EL PROCESADO 
  
    Serial.println(sensor);   

    return sensor;

  
}
void RelaxPosition(){

    for (i = 180; i > 0; i--) { 
   // servo1.write(abs(i-180));  
    ManoIzquierda.pulgar = abs(i);
     ManoIzquierda.indice = abs(i);
      ManoIzquierda.medio = abs(i);
      RGBLED(255,230,255);  // verde
     
      Serial.println("NIVEL 1 - TRIPODE");       //REPORTE DE ESTADO POR PUERTO SERIAL
      delay(10);
      return;
}
}

void TripodPosition(){

    for (i = 0; i < 180; i++) {  
    ManoIzquierda.pulgar = abs(i);
     ManoIzquierda.indice = abs(i);
      ManoIzquierda.medio = abs(i);   
      RGBLED(255,240,240);
     
      Serial.println("NIVEL 1 - TRIPODE");       //REPORTE DE ESTADO POR PUERTO SERIAL
      delay(10);
      return;
}


return;
}




void RGBLED (int red, int green, int blue ){

    analogWrite(redPin, red);
    analogWrite(greenPin, green);                   
    analogWrite(bluePin, blue);

  return;
}


/////////////// SECUENCIA PRELIMINAR DE ARDUINO

void setup() {
  
//  pinMode(2, OUTPUT);
//  pinMode(3, OUTPUT);                   //ASIGNACION DE PINES PARA LED RGB INDICADOR
//  pinMode(4, OUTPUT);
//  pinMode(5, OUTPUT);                   //ASIGNACION DE PINES PARA LED RGB INDICADOR
//  pinMode(6, OUTPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);                   //ASIGNACION DE PINES PARA LED RGB INDICADOR
  pinMode(bluePin, OUTPUT);

  muneca.attach(2);
  pulgar.attach(12); 
  indice.attach(6);                            //ASIGNACION DE PINES DE SALIDA PARA SERVOS
  medio.attach(5);
  anular_menique.attach(3);
  
  Serial.begin(9600);                          //COMUNICACION SERIAL PARA DEBUG

}

/////////////// EJECUCION DE PROGRAMA PRINCIPAL

void loop() {

  input = SensorRead();

                               // REPORTA EL PROMEDIO CALCULADO POR SERIAL
    
                           //POSICIONAMIENTO DE MUÑECA A LA ULTIMA REGISTRADA EN LOS NIVELES DE PRESION 3 Y 4
  
/////////////// REPOSO - DEDOS EXTENDIDOS - STANDBY - LED VERDE     

  if (input < 1000 ) {                        //CUANDO LA LECTURA DEL SENSOR ES MENOR A 500 LOS DEDOS SE EXTIENDEN EN POSICIÓN DE REPOSO 
    RelaxPosition();  
  }                                                                          
                                              //Y LA MUÑECA QUEDA EN LA ULTIMA POSICIÓN REGISTRADA
//      POSITION(ManoIzquierda.posMUN,0,0,0,180);                                           
//      
//     
//      Serial.println("REPOSO - STANDBY");       //REPORTE DE ESTADO POR PUERTO SERIAL
//      delay(10);
//      
 // }
  
/////////////// NIVEL 1 DE PRESION - POSICIÓN DE AGARRE TIPO TRIPODE - LED EN COLOR AZUL ARTICO

  if (sensor > 1500 && sensor < 3000) {         //CUANDO LA LECTURA DEL SENSOR ESTA ENTRE 500 Y 3000 


TripodPosition();

     // POSITION(ManoIzquierda.posMUN,180,180,180,180);                                           


  }

/////////////// NIVEL 2 DE PRESION - POSICIÓN DE AGARRE TIPO CYLINDRICAL/SPHERICAL - LED EN COLOR AZUL PERSIA

  if (sensor > 3500 && sensor < 4500) {        //CUANDO LA LECTURA DEL SENSOR ESTA ENTRE 3500 Y 6500 

   

  }
  
  /////////////// NIVEL 3 DE PRESION - MOVIMIENTO DE FLEXION DE MUÑECA - LED EN COLOR AMARILLO
  
 

  if (sensor > 5000 && sensor < 7500) {        //CUANDO LA LECTURA DEL SENSOR ESTA ENTRE 8000 Y 9000 SE CONSERVA LA POSICION DE LOS DEDOS
                                               //Y LA MUÑECA VA FLEXIONANDO LENTAMENTE
                                               //AL DETECTAR UNA LECTURA MENOR SE EJECUTA EL NIVEL ANTERIOR Y LA MUÑECA SE DETIENE
                                               //AL PERSISTIR EL NIVEL ACTUAL LA MUÑECA CONTINUA FLEXIONANDO HASTA SU POSICION MAXIMA Y SE DETIENE
                                               //AL DETECTAR UNA LECTURA MAYOR SE EJECUTA EL NIVEL SIGUIENTE Y LA MUÑECA SE EXTIENDE
     
      POSITION(ManoIzquierda.posMUN--,ManoIzquierda.pulgar,ManoIzquierda.indice,
               ManoIzquierda.medio,ManoIzquierda.anular_menique);                                           
      RGBLED(100,120,255);
      Serial.println("NIVEL 3 - FLEXION DE MUNECA");     //REPORTE DEL ESTADO POR SERIAL
      delay(10);
      
  }
  
  /////////////// NIVEL 4 DE PRESION - MOVIMIENTO DE EXTENSION DE MUÑECA - LED EN COLOR ROJO
  
  if (sensor > 8000 ) {    
                                              //CUANDO LA LECTURA DEL SENSOR ES SUPERIOR A 9000 SE CONSERVA LA POSICION DE LOS DEDOS
                                              //Y LA MUÑECA VA EXTENDIENDOSE LENTAMENTE 
                                              //AL DETECTAR UNA LECTURA MENOR SE EJECUTA EL NIVEL ANTERIOR Y LA MUÑECA SE FLEXIONA
                                              //AL PERSISTIR EL NIVEL MAXIMO, LA MUÑECA LLEGA A SU PUNTO MAXIMO Y SE DETIENE
           POSITION(ManoIzquierda.posMUN++,ManoIzquierda.pulgar,ManoIzquierda.indice,
               ManoIzquierda.medio,ManoIzquierda.anular_menique);                                          
      RGBLED(50,255,255);
      Serial.println("NIVEL 4 - EXTENSION DE MUÑECA");     //REPORTE DEL ESTADO POR SERIAL
      delay(10);

  }

  
pulgar.write(ManoIzquierda.pulgar);
 delay(10);
    indice.write(ManoIzquierda.indice);     
     delay(10);//POSICIONAMIENTO DE LOS DEDOS 
    medio.write(ManoIzquierda.medio); 
     delay(10);
    anular_menique.write(ManoIzquierda.anular_menique); 
     delay(10);
    muneca.write(ManoIzquierda.muneca);
 // delay(20);
}
