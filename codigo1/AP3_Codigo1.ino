const uint8_t PIN_LED = 19;
// ticks hasta la alarma (1 tick = 1 us). Probar 1000000, 250000 y 50000
const uint64_t ALARMA_US = 1000000;

hw_timer_t *timer = NULL;
volatile bool hayAlarma = false;

void IRAM_ATTR alSonarAlarma() {
  hayAlarma = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);

  timer = timerBegin(1000000); // 1 tick = 1 us
  if (timer == NULL) {
    Serial.println("Error: no hay temporizador libre");
    return;
  }
  timerAttachInterrupt(timer, alSonarAlarma);
  timerAlarm(timer, ALARMA_US, true, 0);
}

void loop() {
  static bool estadoLed = false;

  if (hayAlarma) {
    hayAlarma = false;
    estadoLed = !estadoLed;
    digitalWrite(PIN_LED, estadoLed);
    Serial.printf("Cambio del LED en t = %lu ms\n", millis());
  }
}
