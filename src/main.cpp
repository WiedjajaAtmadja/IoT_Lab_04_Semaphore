#include <Arduino.h>
#define LED_GREEN   4
#define LED_YELLOW  5
#define LED_RED    18

SemaphoreHandle_t xSemaphore;
int nCounter = 0;

void taskProduce(void *pvParameters) {
  while (1) {
    digitalWrite(LED_RED, HIGH);
    nCounter++;
    Serial.printf("==> Produce: %d \n", nCounter); fflush(stdout);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    xSemaphoreGive(xSemaphore);
    digitalWrite(LED_RED, LOW);
    vTaskDelay(950 / portTICK_PERIOD_MS);
  }
}

void taskConsume(void *pvParameters) {
  while (1) {
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    digitalWrite(LED_GREEN, HIGH);
    Serial.printf("<== Consume: %d \n", nCounter); fflush(stdout);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    digitalWrite(LED_GREEN, LOW);
    // vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);

  xSemaphore = xSemaphoreCreateBinary();
  xTaskCreatePinnedToCore(taskProduce, "taskProduce", configMINIMAL_STACK_SIZE+2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskConsume, "taskConsume", configMINIMAL_STACK_SIZE+2048, NULL, 2, NULL, 0);
  Serial.println("System ready");  
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  vTaskDelay(50 / portTICK_PERIOD_MS);
  digitalWrite(LED_BUILTIN, LOW);
  vTaskDelay(950 / portTICK_PERIOD_MS);
}