#define LED 3
#define LDR A7
#define POT A6

//PID değişkenleri
int Hata=0, HD=0, P=0, I=0, D=0, PID=0, Ehata=0;
int OutMax = 255, OutMin = 0;
double Kp =2;
double Ki =5;
double Kd =0;
int SetPoint=200;
int SensorValue=0;
int output=0;

int LDRAvarage;


int toggle = LOW;
void setup(){
     pinMode(LED,OUTPUT);
     pinMode(LDR,INPUT);
     Serial.begin(9600); 

    cli();                    //Kesmeler durduruldu   
    TCCR1A = 0;               //timer 1 registerler temizleniyor
    TCCR1B = 0;               //timer 1 registerler temizleniyor
    TCNT1  = 0;               //Sayaç sıfırlandı
    OCR1A = 6249;             // ((16*10^6)/(10(bu değer istediğimiz kesme frekansı)*256))-1 Bize 10Hz kesme frekansı sağlar 
    TCCR1B |= (1 << WGM12);   //CTC modu seçildi
    TCCR1B |= (1 << CS12);    //* Adımlar arasında geçen süre kristal hızının 256'e bölümü olarak ayarlandı
    TIMSK1 |= (1 << OCIE1A);  
    sei();                    //kesmeleri aktif hale getir
  
}
  ISR(TIMER1_COMPA_vect){ // kesme fonksyonu
   

    SensorValue = analogRead(LDR);
    Hata = SetPoint - SensorValue;
    HD = Hata - Ehata;
    P = Kp * Hata;
    I = I + (Ki * Hata * 0.1);
    D = (Kd * HD) / 0.1;
    PID = P + I + D;
    Ehata = Hata;

     if(PID > OutMax) PID = OutMax;
      else if(PID < OutMin) PID = OutMin;

    analogWrite(LED,PID);
  }

  const int numReadings = 10; // Okuma sayısı (filtreleme için kullanılacak)
  int readings[numReadings];   // Okuma değerlerini saklamak için dizi
  int index = 0;               // Dizi içinde güncellenen okuma endeksi
  int total = 0;               // Okuma değerlerinin toplamı
  int average = 0;             // Filtrelenmiş ortalama değer

void loop() {
  

  total -= readings[index];
  readings[index] = analogRead(LDR);
  total += readings[index];
  index = (index + 1) % numReadings;
    // Filtrelenmiş ortalama değeri hesaplama
  LDRAvarage = total / numReadings;

  SetPoint = analogRead(POT);

  Serial.print("LDR:");
  Serial.print(SensorValue);
  Serial.print(' ');
   Serial.print("PID:");
  Serial.print(PID);
  Serial.print(' ');
  Serial.print("Referans:");
  Serial.print(SetPoint);
  Serial.print(' ');
 Serial.print("otuput:");
  Serial.print(output);
  Serial.print(' ');
   Serial.print("LDRAvarage:");
  Serial.print(LDRAvarage);
  Serial.print(' ');
  Serial.print('\n');
  
  delay(100);

}
