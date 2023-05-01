/**
 * @file basic.ino
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2023-05-01
 *
 * @copyright Copyright (c) 2023 / MaSiRo Project.
 *
 */
#include "linear_actuator_controller.hpp"

#include <ESP32Servo.h>
#include <M5Atom.h>

#define PIN_ADC       GPIO_NUM_33
#define PIN_MOTOR_IN1 GPIO_NUM_22
#define PIN_MOTOR_IN2 GPIO_NUM_19

////////////////////////////////////////////////////////////

LinearActuatorController lac(PIN_MOTOR_IN1, PIN_MOTOR_IN2, PIN_ADC);

void setup()
{
    M5.begin(true, false, true);
    M5.dis.begin();
    M5.dis.fillpix(CRGB::Blue);
    ///////////////////////////////
    lac.begin();
    ///////////////////////////////
    M5.dis.fillpix(CRGB::Green);
}

void loop()
{
    static int pos      = 0;
    static int step     = 10;
    static bool flag_up = true;
    M5.update();
    if (true == M5.Btn.wasReleased()) {
        M5.dis.fillpix(CRGB::Yellow);
        ////////////////////////
        log_d("==============================");

        lac.move(pos);
        if (true == flag_up) {
            pos += step;
            if (pos >= 110) {
                flag_up = false;
            }

        } else {
            pos -= step;
            if (pos <= -10) {
                flag_up = true;
            }
        }
        ////////////////////////
        M5.dis.fillpix(CRGB::Green);
    }
}
