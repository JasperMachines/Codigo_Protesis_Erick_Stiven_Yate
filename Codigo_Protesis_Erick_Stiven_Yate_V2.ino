

#include <Servo.h>

//////////////// DECLARACION DE ACTUADORES/MOTORES 

Servo muneca;

Servo pulgar;
Servo indice;                  //DECLARACION DE SERVOS PARA MOVIMIENTO DE DEDOS Y MUÑECA
Servo medio;
Servo anular_menique;

/////////////// ASIGNACION DE PINES PARA LED RGB INDICADOR

int redPin = 5;
int greenPin = 6;              //             
int bluePin = 9;



/////////////// LIMITES MAXIMOS / MINIMOS DE ACCION DE LOS DEDOS Y MUÑECA
   
int posMUN = 0;                //Posicion actual de rotación de la muñeca (Variable dinámica)

int posminPUL = 180;           //Posiciones minimas y maximas de servos para dedo PULGAR (Variable Fija)
int posmaxPUL = 0;

int posminIND = 180;           //Posiciones minimas y maximas de servos para dedos INDICE (Variable Fija)
int posmaxIND = 0;

int posminMED = 0;             //Posiciones minimas y maximas de servos para dedo MEDIO (Variable Fija)
int posmaxMED = 180;

int posminANU = 0;             //Posiciones minimas y maximas de servos para dedo ANULAR Y MEÑIQUE (Variable Fija)
int posmaxANU = 180;

/////////////// VARIABLES DE SENSOR DE ACTIVIDAD MUSCULAR Y PARA FILTRADO DE SEÑAL (PROMEDIO DE 10 MUESTRAS)

int sensor = 0;

const int numReadings = 10;     //numerode lecturas a promediar

int readings[numReadings];      // array de lecturas
int readIndex = 0;              // indice de lectura actual
int total = 0;                  // total de lecturas

/////////////// SECUENCIA PRELIMINAR DE ARDUINO

void setup() {
  
  pinMode(A3, INPUT_PULLUP);                   //ASIGNACION DE PIN DE ENTRADA PARA SENSOR
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);                   //ASIGNACION DE PINES PARA LED RGB INDICADOR
  pinMode(bluePin, OUTPUT);

  muneca.attach(2);
  pulgar.attach(4); 
  indice.attach(6);                            //ASIGNACION DE PINES DE SALIDA PARA SERVOS
  medio.attach(8);
  anular_menique.attach(10);
  
  Serial.begin(9600);                          //COMUNICACION SERIAL PARA DEBUG

}

/////////////// EJECUCION DE PROGRAMA PRINCIPAL

void loop() {

/////////////// SE TOMA EL PROMEDIO DE 10 MUESTRAS DE LA SEÑAL PARA TENER UNA LECTURA ESTABLE

  total = total - readings[readIndex];        
  readings[readIndex] = analogRead(A3);
  total = total + readings[readIndex];                   
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }

/////////////// SE AMPLIFICA LA SEÑAL 10 VECES PARA HACER MAS FACIL EL PROCESADO DE LA MISMA

  sensor = map((total / numReadings), 1023, 0, 0, 10000);   
  Serial.println(sensor);                                   // REPORTA EL PROMEDIO CALCULADO POR SERIAL
  
/////////////// REPOSO - DEDOS EXTENDIDOS - STANDBY - LED VERDE     

  if (sensor < 500 ) {                        //CUANDO LA LECTURA DEL SENSOR ES MENOR A 500 LOS DEDOS SE EXTIENDEN EN POSICIÓN DE REPOSO
                                              //Y LA MUÑECA QUEDA EN LA ULTIMA POSICIÓN REGISTRADA
                                            
    pulgar.write(posminPUL);
    indice.write(posminIND);                   //POSICIONAMIENTO DE LOS DEDOS 
    medio.write(posminMED); 
    anular_menique.write(posminANU);
    
    muneca.write(posMUN);                       //POSICIONAMIENTO DE MUÑECA A LA ULTIMA REGISTRADA EN LOS NIVELES DE PRESION 3 Y 4
    
    analogWrite(redPin, 0);                    //INDICADOR LED RGB EN VERDE 
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
    
     Serial.println("REPOSO - STANDBY");       //REPORTE DE ESTADO POR PUERTO SERIAL
    
    delay(50);
  }
  
/////////////// NIVEL 1 DE PRESION - POSICIÓN DE AGARRE TIPO TRIPODE - LED EN COLOR AZUL ARTICO

  if (sensor > 500 && sensor < 3000) {         //CUANDO LA LECTURA DEL SENSOR ESTA ENTRE 500 Y 3000 
                                               //SE ACCIONAN LOS SERVOS CORRESPONDIENTS A LOS DEDOS PULGAR, INDICE Y MEDIO 
    pulgar.write(posmaxPUL);
    indice.write(posmaxIND);                   //POSICIONAMIENTO DE LOS DEDOS 
    medio.write(posmaxMED); 
  //  anular_menique.write(posminANU);     
    
    muneca.write(posMUN);                       //POSICIONAMIENTO DE MUÑECA A LA ULTIMA REGISTRADA EN LOS NIVELES DE PRESION 3 Y 4
     
    analogWrite(redPin, 0);
    analogWrite(greenPin, 150);                    //INDICADOR LED RGB EN AZUL ARTICO
    analogWrite(bluePin, 255);
    
    Serial.println("NIVEL 1 - TRIPODE");              //REPORTE DE ESTADO POR PUERTO SERIAL
    
    delay(20);
  }

/////////////// NIVEL 2 DE PRESION - POSICIÓN DE AGARRE TIPO CYLINDRICAL/SPHERICAL - LED EN COLOR AZUL PERSIA

  if (sensor > 3500 && sensor < 6500) {        //CUANDO LA LECTURA DEL SENSOR ESTA ENTRE 3500 Y 6500 
                                               //SE ACCIONA EL SERVO CORRESPONDIENTE A LOS DEDOS ANULAR Y MEÑIQUE
    pulgar.write(posmaxPUL);
    indice.write(posmaxIND);                   //POSICIONAMIENTO DE LOS DEDOS 
    medio.write(posmaxMED); 
    anular_menique.write(posmaxANU); 
    
    muneca.write(posMUN);                       //POSICIONAMIENTO DE MUÑECA A LA ULTIMA REGISTRADA EN LOS NIVELES DE PRESION 3 Y 4
    
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);                   //INDICADOR LED RGB EN AZUL PERSIA
    analogWrite(bluePin, 255);
    
     Serial.println("NIVEL 2 - CYLINDRICAL_ESPHERICAL");          //REPORTE DEL ESTADO POR SERIAL
     
    delay(20);
  }
  
  /////////////// NIVEL 3 DE PRESION - MOVIMIENTO DE FLEXION DE MUÑECA - LED EN COLOR AMARILLO

  if (sensor > 8000 && sensor < 9000) {        //CUANDO LA LECTURA DEL SENSOR ESTA ENTRE 8000 Y 9000 SE CONSERVA LA POSICION DE LOS DEDOS
                                               //Y LA MUÑECA VA FLEXIONANDO LENTAMENTE
                                               //AL DETECTAR UNA LECTURA MENOR SE EJECUTA EL NIVEL ANTERIOR Y LA MUÑECA SE DETIENE
                                               //AL PERSISTIR EL NIVEL ACTUAL LA MUÑECA CONTINUA FLEXIONANDO HASTA SU POSICION MAXIMA Y SE DETIENE
                                               //AL DETECTAR UNA LECTURA MAYOR SE EJECUTA EL NIVEL SIGUIENTE Y LA MUÑECA SE EXTIENDE

    constrain(posmun, 90, 180);
    posMUN -= 5;
    
    muneca.write(posMUN);                       //POSICIONAMIENTO DE MUÑECA A LA ULTIMA REGISTRADA 
    
    analogWrite(redPin, 255);
    analogWrite(greenPin, 165);                 //INDICADOR LED RGB EN AMARILLO
    analogWrite(bluePin, 0);
    
    Serial.println("NIVEL 3 - FLEXION DE MUÑECA");     //REPORTE DEL ESTADO POR SERIAL
    
    delay(5);
  }
  
  /////////////// NIVEL 4 DE PRESION - MOVIMIENTO DE EXTENSION DE MUÑECA - LED EN COLOR ROJO
  
  if (sensor > 9000 ) {                       //CUANDO LA LECTURA DEL SENSOR ES SUPERIOR A 9000 SE CONSERVA LA POSICION DE LOS DEDOS
                                              //Y LA MUÑECA VA EXTENDIENDOSE LENTAMENTE 
                                              //AL DETECTAR UNA LECTURA MENOR SE EJECUTA EL NIVEL ANTERIOR Y LA MUÑECA SE FLEXIONA
                                              //AL PERSISTIR EL NIVEL MAXIMO, LA MUÑECA LLEGA A SU PUNTO MAXIMO Y SE DETIENE

    posMUN += 5;
    muneca.write(posMUN);
    
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);               //INDICADOR LED RGB EN ROJO
    analogWrite(bluePin, 0);
    
    Serial.println("NIVEL 3 - FLEXION DE MUÑECA");     //REPORTE DEL ESTADO POR SERIAL
    
    delay(5);
  }

  delay(5);
}
