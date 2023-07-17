#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 11
#define RST_PIN 6
MFRC522 mfrc522(SS_PIN, RST_PIN);   

//cards and their balances
byte card1UID[] = {0x33, 0x4F, 0x64, 0xA4};
byte card1Balance = 500;

byte card2UID[] = {0xC3, 0x0E, 0x7C, 0xA4};
byte card2Balance = 100;

byte card3UID[] = {0x03, 0x00, 0x82, 0xA4};
byte card3Balance = 150;

byte card4UID[] = {0x13, 0x24, 0x7B, 0xA4};
byte card4Balance = 69;

byte card5UID[] = {0x93, 0xB9, 0x58, 0xA4};
byte card5Balance = 69;


void setup() {
  Serial.begin(9600);   
  SPI.begin();      
  mfrc522.PCD_Init();
  Serial.println("Scan a card");
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Printing card UID
  Serial.print("");
  Serial.print("Card UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Check if card is registered
  if (memcmp(mfrc522.uid.uidByte, card1UID, sizeof(card1UID)) == 0) {
    processCard(card1Balance);
  } else if (memcmp(mfrc522.uid.uidByte, card2UID, sizeof(card2UID)) == 0) {
    processCard(card2Balance);
  } else if (memcmp(mfrc522.uid.uidByte, card3UID, sizeof(card3UID)) == 0) {
    processCard(card3Balance);
  }  else if (memcmp(mfrc522.uid.uidByte, card4UID, sizeof(card4UID)) == 0) {
    processCard(card4Balance);
  } 
     else if (memcmp(mfrc522.uid.uidByte, card5UID, sizeof(card5UID)) == 0) {
    processCard(card5Balance);
  }    
  else {
    Serial.println("Invalid card");
    Serial.print("-----------------------------------");
    Serial.println();
    digitalWrite(5, HIGH);
    digitalWrite(4, LOW);
    delay(1000);
    digitalWrite(5, LOW);
  }

  
  mfrc522.PICC_HaltA();

  mfrc522.PCD_StopCrypto1();
}

void processCard(byte& cardBalance) {
  // Display remaining balance
  Serial.print("Remaining balance: ");
  Serial.println(cardBalance);

  // Check if sufficient balance
  if (cardBalance >= 10) {
    // Deduct amount 
    cardBalance -= 10;

    // Display success message and updated balance
    Serial.println("Transaction successful!");
    Serial.print("Updated balance: ");
    Serial.println(cardBalance);
    Serial.print("-----------------------------------");
    Serial.println();

    // Turn on green LED
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    delay(1000);
    digitalWrite(4, LOW);
  } else {
    // Display insufficient funds message and current balance
    Serial.println("Insufficient funds");
    Serial.print("Current balance: ");
    Serial.println(cardBalance);
    Serial.print("-----------------------------------");
    Serial.println();

    // Turn on red LED
    digitalWrite(5, HIGH);
    digitalWrite(4, LOW);
    delay(1000);
    digitalWrite(5, LOW);
  }
}