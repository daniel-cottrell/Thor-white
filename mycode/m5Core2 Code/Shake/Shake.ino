/*
**************************************************************************
 * M5Stack Core2 Shake detectiong
 * A simple program that allows the M5Core2 to detect shaking through 
 * the accelerometer.
 * 
 * Felix Pountney
 * Daniel Cottrell
 * 
 * Changelog:
 * v1.00 = - initial version for the M5Stack-Core2 Shake
 * 
 * Distributed as-is; no warranty is given.
************************************************************************
*/

#include "M5Core2.h"
#include <zephyr>

// Shake detected value
int Shake = 0;

// acceleration values
float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

void shake_detection(void* pvParameters) {  // Define the tasks to be executed in
                                  // thread 1.
    M5.IMU.Init();
    while (1) {  // Keep the thread running.
        M5.IMU.getAccelData(&accX,&accY,&accZ);
        Serial.print("acceleration X (?): ");
        Serial.println(accX);  // The running time of the serial port
        if (accX > 1){
          Shake = 1;
        }
                                   // printing program.
        delay(
            100);  // With a delay of 100ms, it can be seen in the serial
                   // monitor that every 100ms, thread 1 will be executed once.
    }
}

int Shake_detected(void) {
  if Shake {
    Shake = 0
    return 1
  } else {
    return 0
  }
    
}

void initlaise_shake() {
    M5.begin();  // Init M5Core2.  
    // Creat Task1.
    xTaskCreatePinnedToCore(
        shake_detection,    // Function to implement the task.
                  
        "shake_detection",
        4096,     // The size of the task stack specified as the number of *
                  // bytes.
        NULL,     // Pointer that will be used as the parameter for the task *
                  // being created.
        4,        // Priority of the task.
        NULL,     // Task handler.
        0);  // Core where the task should run.
}

void loop() {
  initlaise_shake()
}