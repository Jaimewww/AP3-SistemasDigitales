const uint8_t PIN_SENAL = 18;
const uint8_t PIN_LED = 19;
// Hz: frecuencia deseada. Probar 100, 1000 y 5000
const uint32_t FREC_SENAL = 100;

// Factor 2: cada alarma es medio periodo de la senal
const uint64_t ALARMA_US = 1000000 / (2 * FREC_SENAL);

hw_timer_t *timer = NULL;
volatile bool nivel = false;
volatile uint32_t cambios = 0;

void IRAM_ATTR alSonarAlarma() {
  nivel = !nivel;
  digitalWrite(PIN_SENAL, nivel);
  cambios++;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SENAL, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  Serial.printf("Frecuencia pedida: %lu Hz\n", (unsigned long)FREC_SENAL);
  Serial.printf("Alarma: %lu ticks (us)\n", (unsigned long)ALARMA_US);

  timer = timerBegin(1000000);
  if (timer == NULL) {
    Serial.println("Error: no hay temporizador libre");
    return;
  }
  timerAttachInterrupt(timer, alSonarAlarma);
  timerAlarm(timer, ALARMA_US, true, 0);
}

void loop() {
  static uint32_t ultimo = 0;
  static bool estadoLed = false;

  if (cambios - ultimo >= 2 * FREC_SENAL) {
    ultimo += 2 * FREC_SENAL;
    estadoLed = !estadoLed;
    digitalWrite(PIN_LED, estadoLed);
    Serial.printf("Cambios del pin: %lu\n", (unsigned long)cambios);
  }
}
