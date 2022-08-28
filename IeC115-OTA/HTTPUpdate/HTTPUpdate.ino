#include <WiFi.h>
#include <HTTPUpdate.h>
#include <SPIFFS.h>

// Constantes -------------------------------------------
// Wi-Fi
const char*   SSID      = "HVC_SUPERIOR2";
const char*   PASSWORD  = "hvdoc3110";

// Setup ------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("\nIniciando");

  // Inicializa SPIFFS
  if (SPIFFS.begin()) {
    Serial.println("SPIFFS Ok");
  } else {
    Serial.println("SPIFFS Falha");
  }

  // Verifica / exibe arquivo
  if (SPIFFS.exists("/Teste.txt")) {
    File f = SPIFFS.open("/Teste.txt", "r");
    if (f) {
      Serial.println("Lendo arquivo:");
      Serial.println(f.readString());
      f.close();
    }
  } else {
    Serial.println("Arquivo não encontrado");
  }

  // Conecta WiFi
  WiFi.begin(SSID, PASSWORD);
  Serial.println("\nConectando WiFi " + String(SSID));
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  // LED indicador de progresso
  httpUpdate.setLedPin(2, HIGH);

  // Callback - Progresso
  Update.onProgress([](size_t progresso, size_t total) {
    Serial.print(progresso * 100 / total);
    Serial.print(" ");
  });

  // Não reiniciar automaticamente
  httpUpdate.rebootOnUpdate(false);

  // Cria instância de Cliente seguro
  WiFiClientSecure client;
  //WiFiClient client;

  // Instrui Cliente a ignorar assinatura do Servidor na conexao segura
  client.setInsecure();

  // Atualização SPIFFS ---------------------------------
  Serial.println("\n*** Atualização da SPIFFS ***");
  for (byte b = 5; b > 0; b--) {
    Serial.print(b);
    Serial.println("... ");
    delay(1000);
  }
  // Encerra SPIFFS
  SPIFFS.end();

  // Efetua atualização da SPIFFS
  t_httpUpdate_return resultado = httpUpdate.updateSpiffs(client, "https://raw.githubusercontent.com/GesseMarchioro/gessetesting/master/Blink.spiffs.esp32.bin");
  //"https://cdn.glitch.global/63cff899-ae0a-4a4d-b5e6-ab9bf6888117/Blink.spiffs.esp32.bin?v=1661692584865"); //https://internetecoisas.com.br/download/IeC115-OTA/Blink.spiffs.esp32.bin");

  // Verifica resultado
  switch (resultado) {
    case HTTP_UPDATE_FAILED: {
      String s = httpUpdate.getLastErrorString();
      Serial.println("\nFalha: " + s);
    } break;
    case HTTP_UPDATE_NO_UPDATES: {
      Serial.println("\nNenhuma atualização disponível");
    } break;
    case HTTP_UPDATE_OK: {
      Serial.println("\nSucesso");
    } break;
  }

  // Inicializa SPIFFS
  if (SPIFFS.begin()) {
    Serial.println("SPIFFS Ok");
  } else {
    Serial.println("SPIFFS Falha");
  }

  // Verifica / exibe arquivo
  if (SPIFFS.exists("/Teste.txt")) {
    File f = SPIFFS.open("/Teste.txt", "r");
    if (f) {
      Serial.println("Lendo arquivo:");
      Serial.println(f.readString());
      f.close();
    }
  } else {
    Serial.println("Arquivo não encontrado");
  }

  // Atualização Sketch ---------------------------------
  Serial.println("\n*** Atualização do sketch ***");
  for (byte b = 5; b > 0; b--) {
    Serial.print(b);
    Serial.println("... ");
    delay(1000);
  }

  // Efetua atualização do Sketch
  resultado = httpUpdate.update(client, ""); //o link era: https://www.dropbox.com/s/biskcw07t0j3hx9/Blink.ino.esp32.bin?dl=0
                                         https://www.dropbox.com/s/        biskcw07t0j3hx9/Blink.ino.esp32.bin?dl=0
                                          //"https://drive.google.com/uc?export=download&id=1xKQ8CBlYgrGm_arAocdRZ8Kfx4idoqRi");//o link era: https://drive.google.com/file/d/1xKQ8CBlYgrGm_arAocdRZ8Kfx4idoqRi/view?usp=sharing
                                        

  // Verifica resultado
  switch (resultado) {
    case HTTP_UPDATE_FAILED: {
      String s = httpUpdate.getLastErrorString();
      Serial.println("\nFalha: " + s);
    } break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("\nNenhuma atualização disponível");
      break;
    case HTTP_UPDATE_OK: {
      Serial.println("\nSucesso, reiniciando...");
      ESP.restart();
    } break;
  }
}

// Loop --------------------------------------------
void loop() {
  // Nada aqui
}
