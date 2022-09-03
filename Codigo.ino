//Bibliotecas gerais
#include <dht.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

//Bibliotecas produzidas por Davide Gironi
#include "adc/adc.h"  
#include "uart/uart.h"
#include "mq135/mq135.h"

int Pinbuzzer = 8;
int Pinled = 7;
int PinA0 = A0;
int PinA1 = A1;
int PinA2 = A2;
int leitura_sensor_mq2 = 1000;
int leitura_sensor_mq7 = 100;
int leitura_sensor_mq135 = 134;
int DHTPIN = 2;
int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);
int leitura_sensor_dht_max = 65;
int leitura_sensor_dht_min = 30;
int resistorpulld = 22000;
int uart_baud_rate = 2400;

void setup(){
  Serial.begin(9600);
  
  pinMode(PinA0, INPUT);
  pinMode(PinA1, INPUT);
  pinMode(PinA2, INPUT);
  pinMode(Pinbuzeer, OUTPUT);
  pinMode(Pinled, OUTPUT);

  dht.begin();
}

void loop(){
  Serial.print("--- Leitura dos Níveis de Concentração ---")
  
  auto[leitura_mq2] = leituramq2();
  auto[leitura_mq8] = leituramq8();  
  auto[leitura_mq135] = leituramq135();
    
  valormq2();
  valormq7();
  valormq135();
  valordht11();

  delay(2000);
}

tuple<float> leituramq2(){
  char printbuff[100];
  double d = 0;
  uint16_t ad = 0;
  
  uart_init(uart_baud_select(uart_baud_rate, F_CPU));
  adc_init();
  sei();
  
  long mq2_ro = 0;
  
  for(;;){
    adc = adc_read(0);
    long res = adc_getresistence(adc, resistorpulld);
    mq2_ro = mq2_getro(res, mq135_defaultppm);
    d = mq2_getppm(res, mq135_defaultro);

    itoa(adc, printbuff, 10);
    ltoa(res, printbuff, 10);
    ltoa(mq2_ro, printbuff, 10);
    leitura_mq2 = dtostrf(d, 3, 5, printbuff);
  }
  
  return {leitura_mq2};
}

void valormq2(float leitura_mq2){
  if (leitura_mq2 > leitura_sensor_mq2){
    Serial.println("Nível perigoso de gases inflamáveis: ");
      Serial.print(leitura_mq2);
        Serial.print(" ppm.")
    digitalWrite(Pinbuzzer, HIGH);
      digitalWrite(Pinled, HIGH);
  }
  else{
    Serial.println("Níveis estáveis de gases inflamáveis:");
    digitalWrite(Pinbuzzer, LOW);
      digitalWrite(Pinled, LOW);  
   if (leitura_mq2 < 300){
    Serial.print(" valor abaixo do nível de sensibilidade do sensor.") 
   }
   else{
    Serial.print(" ");
      Serial.print(leitura_mq2);
        Serial.print(" ppm.");
   }
  }
}

tuple<float> leituramq7(){
  char printbuff[100];
  double d = 0;
  uint16_t ad = 0;
  
  uart_init(uart_baud_select(uart_baud_rate, F_CPU));
  adc_init();
  sei();
  
  long mq7_ro = 0;
  
  for(;;){
    adc = adc_read(0);
    long res = adc_getresistence(adc, resistorpulld);
    mq7_ro = mq7_getro(res, mq135_defaultppm);
    d = mq135_getppm(res, mq135_defaultro);

    itoa(adc, printbuff, 10);
    ltoa(res, printbuff, 10);
    ltoa(mq7_ro, printbuff, 10);
    leitura_mq7 = dtostrf(d, 3, 5, printbuff);
  }
  
  return {leitura_mq7};
}

void valormq7(float leitura_mq7){
  if (leitura_mq7 > leitura_sensor_mq7){
    Serial.println("Nível de Monóxido de Carbono: ");
      Serial.print(leitura_mq7);
        Serial.print(" ppm.")
    digitalWrite(Pinbuzzer, HIGH);
      digitalWrite(Pinled, HIGH);
  }
  else{
    Serial.println("Níveis estáveis de Monóxido de Carbono:");
    digitalWrite(Pinbuzzer, LOW);
      digitalWrite(Pinled, LOW);  
   if (leitura_mq7 < 10){
    Serial.print(" valor abaixo do nível de sensibilidade do sensor.") 
   }
   else{
    Serial.print(" ");
      Serial.print(leitura_mq7);
        Serial.print(" ppm.");
   }
  }
}

tuple<float> leituramq135(){
  char printbuff[100];
  double d = 0;
  uint16_t ad = 0;
  
  uart_init(uart_baud_select(uart_baud_rate, F_CPU));
  adc_init();
  sei();
  
  long mq135_ro = 0;
  
  for(;;){
    adc = adc_read(0);
    long res = adc_getresistence(adc, resistorpulld);
    mq135_ro = mq135_getro(res, mq135_defaultppm);
    d = mq135_getppm(res, mq135_defaultro);

    itoa(adc, printbuff, 10);
    ltoa(res, printbuff, 10);
    ltoa(mq135_ro, printbuff, 10);
    leitura_mq135 = dtostrf(d, 3, 5, printbuff);
  }
  
  return {leitura_mq135};
}

void valormq135(float leitura_mq135){
  if (leitura_mq135 > leitura_sensor_mq135){
    Serial.println("Nível de Gases Tóxicos: ");
      Serial.print(leitura_mq135);
        Serial.print(" ppm.")    
    digitalWrite(Pinbuzzer, HIGH);
      digitalWrite(Pinled, HIGH);
  }
  else{
    Serial.println("Níveis estáveis de gases tóxicos:");
    digitalWrite(Pinbuzzer, LOW);
      digitalWrite(Pinled, LOW);  
   if (leitura_mq135 < 10){
    Serial.print(" valor abaixo do nível de sensibilidade do sensor.") 
   }
   else{
    Serial.print(" ");
      Serial.print(leitura_mq135);
        Serial.print(" ppm.");
   }
  }
}

void valordht11(){
  int valor_dht11 = dht.readHumidity();
    Serial.println("Nível de umidade relativa: ");
      Serial.print(valor_dht11);
        Serial.print(" %.")
  if (valor_dht11 > leitura_sensor_dht_max or valor_dht11 < leitura_sensor_dht_min){
    digitalWrite(Pinled, HIGH);
  }
  else{
    digitalWrite(Pinled, LOW);
  }
}
