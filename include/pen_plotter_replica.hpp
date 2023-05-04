/**
 * @file pen_plotter_replica.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2.0
 * @date 2023-05-03
 *
 * @copyright Copyright (c) 2023 / MaSiRo Project.
 *
 */
#ifndef PEN_PLOTTER_REPLICA_HPP
#define PEN_PLOTTER_REPLICA_HPP

#include "linear_actuator_controller.hpp"

#include <ESP32Servo.h>

#ifndef RC_SERVO_PIN
#define RC_SERVO_PIN 25
#endif
#ifndef RC_SERVO_WAIT_TIME_MS
#define RC_SERVO_WAIT_TIME_MS 500
#endif

class PenPlotterReplica {
public:
    PenPlotterReplica(int servo_wait_time_ms = RC_SERVO_WAIT_TIME_MS, int rx = 26, int tx = 32, int rc_servo_pin = RC_SERVO_PIN);

public:
    void begin();
    void loop();

private:
    void _move_pen(unsigned int postion);
    void _move_lac(double postion);
    void _fin();

private:
    LinearActuatorController _lac;

    int _servo_wait = RC_SERVO_WAIT_TIME_MS;
    Servo _pen;

private:
    const int TIMEOUT_MS = 100;
};

#endif
