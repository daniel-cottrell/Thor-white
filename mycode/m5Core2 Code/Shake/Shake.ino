/*
**************************************************************************
 * M5Stack Core2 Shake detectiong
 * A simple program that turns allows the M5Core2 to detect shaking through 
 * accelerometer
 * 
 * Felix Pountney
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
                                  // thread 1.  定义线程1内要执行的任务
    M5.IMU.Init();
    while (1) {  // Keep the thread running.  使线程一直运行
        M5.IMU.getAccelData(&accX,&accY,&accZ);
        Serial.print("acceleration X (?): ");
        Serial.println(accX);  // The running time of the serial port
        if (accX > 1){
          Shake = 1;
        }
                                   // printing program.  串口打印程序运行的时间
        delay(
            100);  // With a delay of 100ms, it can be seen in the serial
                   // monitor that every 100ms, thread 1 will be executed once.
                   // 延迟100ms,在串口监视器内可看到每隔100ms,线程1就会被执行一次
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
    M5.begin();  // Init M5Core2.  初始化M5Core2
    // Creat Task1.  创建线程1
    xTaskCreatePinnedToCore(
        shake_detection,    // Function to implement the task.
                  // 线程对应函数名称(不能有返回值)
        "shake_detection",  // 线程名称
        4096,     // The size of the task stack specified as the number of *
                  // bytes.任务堆栈的大小(字节)
        NULL,     // Pointer that will be used as the parameter for the task *
                  // being created.  创建作为任务输入参数的指针
        4,        // Priority of the task.  任务的优先级
        NULL,     // Task handler.  任务句柄
        0);  // Core where the task should run.  将任务挂载到指定内核
}

void loop() {
  initlaise_shake()
}