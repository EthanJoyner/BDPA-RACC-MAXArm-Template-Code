#include "ESP32PWMServo.h"

Servo servo1;
Servo servo2;
static const int servo1Pin = 15;
static const int servo2Pin = 4;
static const int servoMinPulse = 500;
static const int servoMaxPulse = 2500;
static const int servoStepMs = 20;

int servo_stat = 0;
int init_pulse[2] = {500,500};

static void writeServoById(int id, int pulse) {
    if (id == 1) {
        servo1.writeMicroseconds(pulse);
    } else if (id == 2) {
        servo2.writeMicroseconds(pulse);
    }
}

void PWMServo_init(){
    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    servo_stat = 0;
    init_pulse[0] = servoMinPulse;
    init_pulse[1] = servoMinPulse;
}

void SetPWMServo(int id, int pul, int duration){
    if(0 < id && id < 3){
        pul = constrain(pul, servoMinPulse, servoMaxPulse);

        if (duration <= 0 || !servo_stat) {
            writeServoById(id, pul);
            init_pulse[id - 1] = pul;
            servo_stat = 1;
            return;
        }

        int startPulse = init_pulse[id - 1];
        int delta = pul - startPulse;
        int stepCount = max(1, duration / servoStepMs);
        int stepDelay = max(1, duration / stepCount);

        for (int count = 1; count <= stepCount; count++) {
            int nextPulse = startPulse + (delta * count) / stepCount;
            writeServoById(id, nextPulse);
            delay(stepDelay);
        }

        init_pulse[id - 1] = pul;
    }
}
