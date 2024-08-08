#include <Arduino.h>

#define pinLatch 4
#define pinClk 7
#define pinData 8

#define trimr A5

const byte LED[] = {13, 12, 11, 10};

#define tlacitkoA1 A1
#define tlacitkoA2 A2
#define tlacitkoA3 A3

#define sirena 

#define led1 D1
#define ON  LOW
#define OFF HIGH

bool tlac1 = false;
bool tlac2 = false;
bool tlac3 = false;

const byte mapaSegment[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90};
const byte mapaCisloSeg[] = {0xF1, 0xF2, 0xF4, 0xF8};

int rezim = 0;
bool blik;

int desitkyM = 0;  //0
int jednotkyM = 0; //1
int desitkyS = 0;  //2
int jednotkyS = 0; //3

int desitkyM2;
int jednotkyM2;
int desitkyS2;
int jednotkyS2;

int whileZ;



bool timeRunning = false;

void setup() {
  Serial.begin(9600);

  pinMode(pinLatch, OUTPUT);
  pinMode(pinClk, OUTPUT);
  pinMode(pinData, OUTPUT);

  pinMode(LED[0], OUTPUT);
  pinMode(LED[1], OUTPUT);
  pinMode(LED[2], OUTPUT);
  pinMode(LED[3], OUTPUT);

  digitalWrite(LED[0], OFF);
  digitalWrite(LED[1], OFF);
  digitalWrite(LED[2], OFF);
  digitalWrite(LED[3], OFF);
}

void loop() {
  digitalWrite(LED[0], ON);
  while(timeRunning == false){
    tlacitkaScan();

    if(tlac3==ON)timeRunning = true;

    if(tlac1 == ON){
      if(rezim == 0){
        desitkyM++;
        if(desitkyM>5)desitkyM = 0;
        vypsat(0, desitkyM);
      }
      if(rezim == 1){
        jednotkyM++;
        if(jednotkyM>9)jednotkyM = 0;
        vypsat(1, jednotkyM);
      }
      if(rezim == 2){
        desitkyS++;
        if(desitkyS>5)desitkyS = 0;
        vypsat(2, desitkyS);
      }
      if(rezim == 3){
        jednotkyS++;
        if(jednotkyS>9)jednotkyS = 0;
        vypsat(3, jednotkyS);
      }
      if(rezim == 4){
      delay(300);
      }

      while(tlac1==ON){
          vypsatVse();
          tlacitkaScan();
      }//Konec tlac1 While
  }

    if(tlac2 == ON){
    rezim++;
    if(rezim>4)rezim = 0;
    while(tlac2==ON){
      vypsatPodleRezimu();
      tlacitkaScan();
    }
  }    

    for(int i = 0; i<500; i++){
      vypsatPodleRezimu();
      delay(1);
      tlacitkaScan();
      if(tlac1==ON or tlac2==ON)i=500;
      if(tlac3 == ON)i=500;
    }
    for(int i = 0; i<500; i++){
      vypsatVse();
      delay(1);      
      tlacitkaScan();
      if(tlac1==ON or tlac2==ON)i=500;
      if(tlac3 == ON)i=500;
    }

  } //Konec WHILE pro nastavení času

  desitkyM2 = desitkyM;
  jednotkyM2 = jednotkyM;
  desitkyS2 = desitkyS;
  jednotkyS2 = jednotkyS;

  //Casovac spusten
  timer();
  
  
}

void tlacitkaScan(){
  tlac1 = digitalRead(tlacitkoA1);
  tlac2 = digitalRead(tlacitkoA2);
  tlac3 = digitalRead(tlacitkoA3);
}

void vypsat(byte segment, byte hodnota) {
  digitalWrite(pinLatch, LOW);
  shiftOut(pinData, pinClk, MSBFIRST, mapaSegment[hodnota]);
  shiftOut(pinData, pinClk, MSBFIRST, mapaCisloSeg[segment] );
  digitalWrite(pinLatch, HIGH);
}

void vypsatVse(){
  abs(desitkyM);
  abs(jednotkyM);
  abs(desitkyS);
  abs(jednotkyS);
  vypsat(0, desitkyM);
  vypsat(1, jednotkyM);
  vypsat(2, desitkyS);
  vypsat(3, jednotkyS);
}

void vypsatPodleRezimu(){
  if(rezim == 0){
    vypsat(1, jednotkyM);
    vypsat(2, desitkyS);
    vypsat(3, jednotkyS);
  }
  if(rezim == 1){
    vypsat(0, desitkyM);
    vypsat(2, desitkyS);
    vypsat(3, jednotkyS);
  }
  if(rezim == 2){
    vypsat(0, desitkyM);
    vypsat(1, jednotkyM);
    vypsat(3, jednotkyS);
  }
  if(rezim == 3){
    vypsat(0, desitkyM);
    vypsat(1, jednotkyM);
    vypsat(2, desitkyS);
  }
  if(rezim==4){
    vypsatVse();
  }
}

void timer(){
    Serial.print("casovac spusten\n");
  
  for(desitkyM; desitkyM >-1; desitkyM-- ){
    for(jednotkyM; jednotkyM>-1;jednotkyM--){
      for(desitkyS; desitkyS>-1; desitkyS--){
        for(jednotkyS;jednotkyS>0;jednotkyS--){
          for(int i=0;i<500;i++){
            vypsatVse();
            delay(1);
            if(!(jednotkyS %2) ==0){
              digitalWrite(LED[0], ON);

            }

            }digitalWrite(LED[0], OFF);

          
        }
            for(int i=0;i<500;i++){
            vypsatVse();
            delay(1);
            } 
         if(jednotkyS<1)jednotkyS = 9;
      }

       if(desitkyS<1)desitkyS = 5;
    }

     if(jednotkyM<1) jednotkyM = 9;
  }
  vypsat(0,0);
  vypsat(1,0);
  vypsat(2,0);
  vypsat(3,0);
  desitkyM = 0;
  jednotkyM = 0;
  desitkyS = 0;
  jednotkyS = 0;
  rezim = 0;
  timeRunning = false;
}