// LÍBRERIAS
#include <SoftwareSerial.h> //Libreria Software Serial para Bluetooth (Se descarga), en esta caso usamos el HC-05 que sirve para usarse de Esclavo ó Maestro
#include <DHT.h> //Librería paa usar sensores DHT (Se descarga)
#include <DHT_U.h> //Complemento de la librería DHT

//PINES ESPECIALES
#define DHTPIN 6 //Se define el pin del DHT
#define DHTTYPE DHT11 //Se define el tipo de sensor que se usara para medir la temperatura y humedad, en este caso sera el DHT-11

//OBJETOS DE LAS LIBRERÍAS
DHT dht(DHTPIN, DHTTYPE); // Crea un objeto de la librería DHT, indicando así el sensor
SoftwareSerial BT(10,11); //Definir los pines RX y TX del Arduino conectados al Bluetooth

//OBJETOS
char Mensaje; //Variable que mandara los mensajes a los Switches y por voz

// Bocina
const int buzzer = 13; //Bocina conectada al Pin 13

//Sistema de IR
const int sensorPin = 12; //Sensor de IR conectado al Pin 12 
const int LEDIV = 8; //Led Interior Verde
const int LEDIR = 7; //Led Interior Rojo

//Sistema de luces interiores
const int LEDG1 = 2; //Led H.G 1
const int LEDG2 = 3; //Led H.G 2
const int LEDP1 = 4; //Led H.P 1
const int LEDP2 = 5; //Led H.P 2

//Variables para almacenar las lecturas del sensor DHT11
float celsius;
float fahrenheit;
float humedad;

void setup() {
  Serial.begin(9600); 
  BT.begin(38400); //Inicializamos el puerto serie BT (Para Modo AT 2)
  dht.begin(); //Comunicacion con el DHT11
  Serial.println("Iniciando todos los sistemas... Listo"); //Mensaje para indicar que esta el sistema listo
  delay(2000);
  
  
  //Se establecen los pines como entradas o salidas
  //Todos los LED's se establecen como Salida
  pinMode(sensorPin, INPUT); //Se establece como Entrada
  pinMode(buzzer, OUTPUT);
  pinMode(LEDIV,OUTPUT);
  pinMode(LEDIR,OUTPUT);
  
  pinMode(LEDG1,OUTPUT);   
  pinMode(LEDG2,OUTPUT);
  pinMode(LEDP1,OUTPUT);
  pinMode(LEDP2,OUTPUT);

}

void loop() 
{ 

  
  //SISTEMA DE TEMPERATURA Y HUMEDAD
  celsius = dht.readTemperature();  //Lee y almacena la temperatura del sensor DHT11
  fahrenheit = dht.readTemperature(true);  //Lee y almacena la temperatura del sensor DHT11
  humedad = dht.readHumidity();         //Lee y almacena la humedad del sensor DHT11
  
  //Envía los datos del DHT11 como la temperatura, humedad e intensidad a la aplicación móvil por medio del Bluetooth
  BT.print(celsius);    
  BT.print(" °C");
  BT.print("|");
  BT.print(fahrenheit);
  BT.print(" °F");
  BT.print("|");
  BT.print(humedad);
  BT.print(" %");
  BT.print("|");
  delay(1000);

  //Se envían los datos del DHT11 al Monitor Serie
  Serial.print(celsius);
  Serial.print(" °C");
  Serial.print("|");
  Serial.print(fahrenheit);
  Serial.print(" °F");
  Serial.print("|");
  Serial.print(humedad);
  Serial.print(" %");
  Serial.print("|");
  delay(1000);                

//SISTEMA DE ALARMA AL ENTRAR EN LA PUERTA
  int value = 0; //Declaramos una variable llamada value donde guardara el valor de la luz leida por el sensor
  value = digitalRead(sensorPin); //el valor va a ser leido por el sensor
  
  digitalWrite(LEDIV,HIGH); //Led verde indica que no hay movimiento y todo esta seguro
  digitalWrite(LEDIR,LOW); //Led Rojo indicara que hay movimiento inesperado
  
  if (value == HIGH) //Aquí indicamos que si el valor esta encendido, en el Serial mostrara que no hay obstaculo y por lo tanto en el noTOne(buzzer) no emitirá un sonido
  {
    Serial.println("Seguro: No hay movimiento");
    noTone(buzzer);
    delay(1000);
  }
  //En caso contrario 
  else
  {
    digitalWrite(LEDIR,HIGH); //El Led Rojo se enciende, indicando que hay movimiento
    digitalWrite(LEDIV,LOW); //El Led Verde se apaga
    delay(1000);
    tone(buzzer,2500); //Suena un tono
    delay(1000); //Durante 150 ms
    tone(buzzer,1500); //Suena otro tono
    delay(1000); //Durante 150 ms
    noTone(buzzer);
    Serial.println("Advertencia: Detectó movimiento"); //Manda un mensaje al Serial donde indica que hay movimiento
  }                       
  

delay(1000);

//SISTEMA DE LUCES INTERIORES Y SISTEMA DE ABRIR-CERRAR PUERTAS POR SWITCH Y VOZ
if(BT.available()) 
{  //Si llega un dato por el puerto BT se envía al monitor Serial
  char Mensaje = BT.read(); //Guarda en la variable “Mensaje” el dato recibido por Bluetooth
  
  if(Mensaje == 'A') 
  {
    Serial.println("Se ha encendido la habitacion Grande 1");
    digitalWrite(LEDG1,HIGH); 
  }
  else if(Mensaje == 'a') 
  {
    Serial.println("Se ha apagado la habitacion Grande 1");
    digitalWrite(LEDG1,LOW);
  }
    else if(Mensaje == 'B') 
  {
    Serial.println("Se ha encendido la habitacion Grande 2");
    digitalWrite(LEDG2,HIGH);
  }
    else if(Mensaje == 'b') 
  {
    Serial.println("Se ha apagado la habitacion Grande 2");
    digitalWrite(LEDG2,LOW);
  }
      else if(Mensaje == 'G') 
  {
    Serial.println("Se ha encendido la habitacion pequeña 1");
    digitalWrite(LEDP1,HIGH);
  }
    else if(Mensaje == 'g') 
  {
    Serial.println("Se ha apagado la habitacion pequeña 1");
    digitalWrite(LEDP1,LOW);
  }
    else if(Mensaje == 'H') 
  {
    Serial.println("Se ha encendido la habitacion pequeña 2");
    digitalWrite(LEDP2,HIGH);
  }
    else if(Mensaje == 'h') 
  {
    Serial.println("Se ha apagado la habitacion pequeña 2");
    digitalWrite(LEDP2,LOW);
  }
      else if(Mensaje == 'S') 
  {
    Serial.println("Se ha encendido todo");
    digitalWrite(LEDG1,LEDG2,LEDP1,LEDP2,HIGH);
  }
    else if(Mensaje == 's') 
  {
    Serial.println("Se ha apagado todo");
    digitalWrite(LEDG1,LEDG2,LEDP1,LEDP2,LOW);
  }
}
}
