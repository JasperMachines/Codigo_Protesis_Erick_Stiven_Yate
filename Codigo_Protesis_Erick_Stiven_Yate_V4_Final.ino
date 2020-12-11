
//Firmware para Prótesis Erick Yate V3.0
//
//Sensores utilizados: Infrarrojo de proximidad (Actividad Muscular) - Botón pulsador(Selector de posición)

//Descripción breve: El micro captura la actividad muscular por medio del sensor conectado al pin análogo A3, 
//                   posiciona los dedos y la muñeca de acuerdo al estado seleccionado por el botón selector,
//                   las posiciones se encuentran en una tabla de valores formada por 5 variables correspondientes a cada servo,
//                   de este modo una variable puede guardar varios valores de posiciones, ser modificada y accesible.
//                   El micro posee un led RGB como indicador visual del estado de la prótesis, a cada pulsación del boton, el led
//                   pasará a un color diferente y apuntara a nuevas variables de posición, así el usuario puede tener control total
//                   y la posibilidad de añadir multiples posiciones de uso cotidiano.


#include <Servo.h>            //incluida la librería de control de servos de Arduino IDE

Servo muneca;
Servo pulgar;
Servo indice;                  //declaración de servos para dedos y muñeca.
Servo medio;
Servo anular_menique;

/////////////// ASIGNACION DE PINES PARA LED RGB INDICADOR

int redPin = 11;         //pin led rojo
int greenPin = 10;      //pin led verde
int bluePin = 9;       //pin led azul

int sensor = 0;        //resultado de promedio de señal muscular, variable dinamica. 
int selector = 0;      //contador del boton selector de posición, variable dinamica. 
int responsedelay = 5; //modifica la velocidad de respuesta en los servos, mayor valor = mayor velocidad, valor maximo = 30.

///////BANCO DE DATOS PARA GESTOS  DE LA MANO
//0 : extiende/relaja los dedos, el led indica la actividad muscular.
//1 : agarre tip bidigital entre dedos índice y pulgar.
//2 : agarre tip tridigital entre dedos índice, medio y pulgar.
//3 : agarre cilìndrico/esferico, flexiona los 5 dedos.
//4 : extensión/flexión de la muñeca.

int posiciones=4;
///////////////////////0////1/////2/////3////4////5
int varmuneca [] = { 90,   90,   90,   90,   90};   //,  90};   //continuar la matriz para mas posiciones
int varpulgar [] = {180,    0,    0,    0,   0};   //,   0};
int varindice [] = {  0,    0,    0,    0,   0};   //,   0};
int varmedio  [] = {  0,  180,    0,    0,   0};   //,   0};
int varanular [] = {180,  180,  180, -180,   0};   //,   0};

//////////////////////0////1////2////3////4////5
int red       [] = {255, 255, 255,   0,   0};   //,   0};
int green     [] = {2, 100, 255, 100, 255};   //, 100};     //continuar la matriz para mas colores
int blue      [] = {255, 100,  10, 255, 255};   //, 255};

/////////////// VARIABLES DE SENSOR DE ACTIVIDAD MUSCULAR Y PARA FILTRADO DE SEÑAL (PROMEDIO DE 10 MUESTRAS)
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

////////////////FUNCION PARA CAPTURA Y FILTRADO DE SEÑAL MEDIANTE PROMEDIO DE X MUESTRAS

void getmusclesignal() {
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(A3);
  total = total + readings[readIndex];                    //Calcula el promedio en tiempo real                                                  
  readIndex = readIndex + 1;                              //Fuente: https://www.arduino.cc/en/Tutorial/BuiltInExamples/Smoothing
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  sensor = map((total / numReadings), 500, 50, 0, 180);  // Mapea la señal obtenida entre valores no superiores a 180 
                                                          // para ser utilizados directamente en los servos
  
  sensor = constrain(sensor, 0, 180);                     // Mantiene los valores de la variable entre 0 y 180
  Serial.println(sensor);                                 // Reporta la lectura filtrada del sensor
 // Serial.println(analogRead(A3));                         // Reporta la lectura raw del sensor
}

/////////////// SECUENCIA PRELIMINAR DE ARDUINO
void setup() {

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);                   //Asignación de pines para led RGB indicador
  pinMode(bluePin, OUTPUT);
  pinMode(A5, INPUT_PULLUP);                   //Asignación de pin de entrada para botón selector

  muneca.attach(2);
  pulgar.attach(12);
  indice.attach(3);                            //Asignación de pines para control de servo-motores
  medio.attach(6);
  anular_menique.attach(8);

  Serial.begin(9600);                          //Activa puerto serial para debug.

}

void loop() {

  getmusclesignal();                          //llama a la función de captura de actividad muscular.
                                              //Esta aloja en la variable sensor el resultado de la operación.
delay(10);
  Serial.println(selector);                   //Reporta el estado del selector, variable al presionar el botón.

  if (selector == 0) {                        //Se define una rutina de posición relajada, donde los dedos se extienden, 
                                              //la muñeca retiene su actual posición y el led RGB muestra un color verde.

    analogWrite(redPin, red[selector]);
    analogWrite(greenPin, (green[selector] + sensor));    //indicador led RGB en verde.
    analogWrite(bluePin, (blue[selector] + sensor));

    pulgar.write(varpulgar[0]);
    indice.write(varindice[0]);                       //Extensión de los dedos pulgar, indice, medio, anular y meñique.    
    medio.write(varmedio[0]);
    anular_menique.write(varanular[0]);

  }

  if (!digitalRead(A5)) {                                     //Captura la actividad del botón selector
    selector++;                                               //Aumenta el valor del selector

    if (selector > posiciones)selector = 0;                   //Controla el rango de selección.

    analogWrite(redPin, red[selector]);
    analogWrite(greenPin, green[selector]);                    //indicador led RGB en el color asignado en la matriz de datos. linea 49
    analogWrite(bluePin, blue[selector]);

    delay(500);                                                //delay para retardo de botón.

  }
  
  if (selector > 0 && selector < 4) {                           //delimita las posiciones entre 1 y 3 para movimientos digitales unicamente
    pulgar.write(abs(sensor + varpulgar[selector]));
    delay(responsedelay);
    indice.write(abs(sensor + varindice[selector]));
    delay(responsedelay);
    medio.write(abs(sensor + varmedio[selector]));
    delay(responsedelay);
    anular_menique.write(abs(sensor + varanular[selector]));
    delay(responsedelay);
  }

if (selector > 3){                                             //delimita la posición 4 para control de flexión/extensión de la muñeca.
muneca.write(constrain(varmuneca[4] + sensor,0, 180));
delay(responsedelay*2); 
}

}
