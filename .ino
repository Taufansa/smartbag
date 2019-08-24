#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define SS_PIN D4
#define RST_PIN D3
#define FIREBASE_HOST "projectfirebase.com"
#define FIREBASE_AUTH "token"
#define WIFI_SSID "ussid"
#define WIFI_PASSWORD "password"
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;
int readdata;
int itemprimer1 = 0;
int itemprimer2 = 0;
int itemsekunder1 = 0;
int itemsekunder2 = 0;
int x = 0;
boolean back;
boolean next;

String barangprimer[] ={"xxxxxxx" , "xxxxxxx"};
String barangsekunder[] ={"xxxxxxx" , "xxxxxxx"};

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  
 // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Firebase.setString ("barang"," Loading ");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Firebase.setFloat("number", 0.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  
  Firebase.setBool("update", false);
  
}
// --------------------------------------------------------------------
void loop() {
 readsuccess = getid();
 if(readsuccess){
   Serial.println(StrUID);
   Serial.println(" ");
   if ( StrUID == barangprimer[0]){
    Serial.println("item primer 1 masuk");
    itemprimer1 = 1;
   }
   if ( StrUID == barangprimer[1]){
    Serial.println("item primer 2 masuk");
    itemprimer2 = 1;
   }
   if ( StrUID == barangsekunder[0]){
    Serial.println("item sekunder 1 masuk");
    itemsekunder1 = 1;
   }
   if ( StrUID == barangsekunder[1]){
    Serial.println("item sekunder 2 masuk");
    itemsekunder2 = 1;
   }

   Serial.println(" ");
   Serial.println(" item primer 1 = ");
   Serial.println(itemprimer1);
   Serial.println(" item primer 2 = ");
   Serial.println(itemprimer2);
   Serial.println(" item sekunder 1 = ");
   Serial.println(itemsekunder1);
   Serial.println(" item sekunder 2 = ");
   Serial.println(itemsekunder2);
   Serial.println(" ");

   if (((itemprimer1 == 1) && (itemprimer2 == 1) && (itemsekunder1 == 1)) || ((itemprimer1 == 1) && (itemprimer2 == 1) && (itemsekunder2 == 1)) || ((itemprimer1 == 1) && (itemprimer2 == 1))){
      delay(10000);
      Serial.println("barang terbawa");
      Firebase.setString ("barang","tidak ada yang tertinggal");
      // handle error
      if (Firebase.failed()) {
         Serial.print("setting /message failed:");
         Serial.println(Firebase.error());  
         return;
      }
      Firebase.setFloat("number", 1.0);
      // handle error
      if (Firebase.failed()) {
         Serial.print("setting /number failed:");
         Serial.println(Firebase.error());  
         return;
      }
      Firebase.setBool("truth", true);
      // handle error
      if (Firebase.failed()) {
          Serial.print("setting /truth failed:");
          Serial.println(Firebase.error());  
          return;
      }
      Firebase.setBool("update", true);
      itemprimer1 = 0;
      itemprimer2 = 0;
      itemsekunder1 = 0;
      itemsekunder2 = 0;
      back = true;
   }

   if (((itemprimer1 == 1) && (itemprimer2 == 0) && (itemsekunder1 == 1) && (itemsekunder2 == 1)) || ((itemprimer1 == 0) && (itemprimer2 == 1) && (itemsekunder1 == 1) && (itemsekunder2 == 1)) || ((itemprimer1 == 0) && (itemprimer2 == 0) && (itemsekunder1 == 1) && (itemsekunder2 == 1))){
      delay(30000);
      Serial.println("barang tertinggal");
      Firebase.setString ("barang","ada yang tertinggal");
      // handle error
      if (Firebase.failed()) {
         Serial.print("setting /message failed:");
         Serial.println(Firebase.error());  
         return;
      }
      Firebase.setFloat("number", 1.0);
      // handle error
      if (Firebase.failed()) {
         Serial.print("setting /number failed:");
         Serial.println(Firebase.error());  
         return;
      }
      Firebase.setBool("truth", false);
      // handle error
      if (Firebase.failed()) {
          Serial.print("setting /truth failed:");
          Serial.println(Firebase.error());  
          return;
      }
      Firebase.setBool("update", true);
      itemprimer1 = 0;
      itemprimer2 = 0;
      itemsekunder1 = 0;
      itemsekunder2 = 0;
      back = true;
   }

   if (back == true){
    delay(10000);
    Firebase.setString ("barang"," Loading ");
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
    }
    Firebase.setFloat("number", 0.0);
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
    }
    Firebase.setBool("truth", false);
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
    } 
    Firebase.setBool("update", false);
   }
 } 
}
// --------------------------------------------------------------------

int getid(){  
  if(!mfrc522.PICC_IsNewCardPresent()){
    return 0;
  }
  if(!mfrc522.PICC_ReadCardSerial()){
    return 0;
  }
 
  
  
  Serial.println("THE UID OF THE SCANNED CARD IS:");
  
  for(int i=0;i<4;i++){
    readcard[i]=mfrc522.uid.uidByte[i]; //storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}
// --------------------------------------------------------------------
void array_to_string(byte array[], unsigned int len, char buffer[])
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}


//taufan samudra akbar(telkom university)
