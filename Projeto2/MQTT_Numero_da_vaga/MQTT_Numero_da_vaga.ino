#include <SPI.h>
#include <UIPEthernet.h>
#include "utility/logging.h"
#include <PubSubClient.h>

// Update these with values suitable for your hardware/network.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x42 };

int vagas[13] = {};

void callback(char* topic, byte* payload, unsigned int length) {
  char* mensagem = payload;
  int valorMensagem = String(mensagem[0]).toInt();

  Serial.println(valorMensagem);
  
  String vaga = String(topic[5]);
  vaga += topic[6];

  vagas[vaga.toInt()] = valorMensagem;

  Serial.println(vagas[vaga.toInt()]);
  
  Serial.print("vaga: ");
  Serial.print(vaga.toInt());
  
  Serial.println("");
}

EthernetClient ethClient;
PubSubClient client("m13.cloudmqtt.com", 10193, callback, ethClient);
  
long lastReconnectAttempt = 0;

boolean reconnect() {
  if (client.connect("arduino1", "arduino1", "arduino1")) {
    client.subscribe("vaga/#");
    Serial.println("conectado");
  }
  return client.connected();
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Conectando...");
  Ethernet.begin(mac);
  delay(1500);
  lastReconnectAttempt = 0;
  
}


void loop()
{
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      Serial.println("Reconectando...");
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();
  }

}
