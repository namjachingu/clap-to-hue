const int SAMPLE_WINDOW = 250;  // ms
const int LED_PIN = 9;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long startTime = millis();
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startTime < SAMPLE_WINDOW) {
    unsigned int soundLevel = analogRead(A0);

    if (soundLevel > signalMax) {
      signalMax = soundLevel;
    }
    if (soundLevel < signalMin) {
      signalMin = soundLevel;
    }
  }

  // Calculate peak-to-peak amplitude and convert to voltage
  unsigned int peakToPeak = signalMax - signalMin;
  double voltage = peakToPeak * 3.3 / 1024.0;

  Serial.println(voltage);

  if (voltage >= 1.0) {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    Serial.println("Clap clap");
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}