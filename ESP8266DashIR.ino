// @license ESP8266DashIR.ino
// MIT License
// (c) 2018 miyakelp


#include <ESP8266WiFi.h>
#include <cassert>
#include <netif/etharp.h>
#include <lwip/netif.h>
#include "IR.hpp"

extern "C" {
  #include <user_interface.h>
}

#define LED_OUT 12
#define IR_RECEIVER 13 // GP1UXC41QS
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"
#define INTERVAL 5000

#define READER_BUF_SIZE 256

struct eth_hdr;

// Please edit these fields.
const unsigned int irPatternLength = 2;
const unsigned int irPattern[] = {9000, 4500};
const unsigned int dashButtonAddr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};


ESP8266DashIR::IRLED led(LED_OUT, IR_RECEIVER);
netif_input_fn originalNetifInput = nullptr;
long lastPushed = 0;


err_t netifInput(pbuf* p, netif* inp)
{
  if (p->len < sizeof(eth_hdr)) {
    return (*originalNetifInput)(p, inp);
  }

  auto eth = reinterpret_cast<const eth_hdr*>(reinterpret_cast<const uint8_t *>(p->payload));
  for (int i = 0; i < 6; i++) {
    if (eth->src.addr[i] != dashButtonAddr[i]) {
      break;
    }
    if (i == 5 && (millis() > lastPushed + INTERVAL || millis() < lastPushed)) {
      Serial.println("\nSending...");
      led.sendPattern(irPattern, irPatternLength);
      Serial.println("Finish");
      lastPushed = millis();
    }
  }
  
  return (*originalNetifInput)(p, inp);
}


void innoculateCallbackFn()
{
  assert(netif_list != nullptr);
  assert(netif_list->next == nullptr);
  originalNetifInput = netif_list->input;
  netif_list->input = netifInput;
}


void setupWiFi(boolean innoc)
{
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  if(innoc) {
    innoculateCallbackFn();
  }
  WiFi.printDiag(Serial);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("*");
    Serial.flush();
    delay(500);
  }
  Serial.println("\nconnected\n");
  WiFi.printDiag(Serial);
  Serial.print("\n");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("\n");
  Serial.flush();
}


void setup()
{
  Serial.begin(115200);
  delay(10);
  setupWiFi(true);
  wifi_set_sleep_type(MODEM_SLEEP_T);
}

void loop()
{
  int readPatternLength;
  unsigned int data[READER_BUF_SIZE];

  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_OUT, LOW);
    Serial.println("\n\n### DISCONNECTED ###\n");
    Serial.flush();
    delay(5000);
    setupWiFi(false);
    return;
  }

  char c = Serial.read();
  switch (c) {
    case 'r':
      Serial.println("\nReader Mode");
      readPatternLength = led.readPattern(data, READER_BUF_SIZE);
      for (int i = 0; i < readPatternLength; i++) {
        Serial.print(data[i]);
        Serial.print(", ");
      }  
      Serial.print("\nLength : ");
      Serial.print(readPatternLength);
      Serial.println("\nFinish\n");
      Serial.flush();
      break;
    case 's':
      Serial.println("\nSending...");
      led.sendPattern(irPattern, irPatternLength);
      Serial.println("Finish");
      break;
    default:
      break;
  }

}
