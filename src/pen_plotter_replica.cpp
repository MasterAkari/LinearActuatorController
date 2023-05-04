/**
 * @file pen_plotter_replica.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2.0
 * @date 2023-05-03
 *
 * @copyright Copyright (c) 2023 / MaSiRo Project.
 *
 */
#include "pen_plotter_replica.hpp"

#include <Arduino.h>

PenPlotterReplica::PenPlotterReplica(int servo_wait_time_ms, int rx, int tx, int rc_servo_pin)
{
    this->_connected = false;
    ///////////////////////////////
    // RC Servo
    ///////////////////////////////
    this->_servo_wait = servo_wait_time_ms;
    pinMode(rc_servo_pin, OUTPUT);
    this->_pen.setPeriodHertz(50);
    this->_pen.attach(rc_servo_pin, 500, 2400);
    ESP32PWM::allocateTimer(0);
    ///////////////////////////////
    // Linear Actuator
    ///////////////////////////////
    this->_lac.begin(1000);
    ///////////////////////////////
    // Serial
    ///////////////////////////////
    pinMode(rx, INPUT_PULLUP);
    pinMode(tx, INPUT_PULLUP);
    Serial2.begin(115200, SERIAL_8N1, rx, tx);
    Serial2.flush();
}

/////////////////////////////////////////////////
// public method
/////////////////////////////////////////////////
void PenPlotterReplica::begin()
{
    // this->_pen.write(90);
    // this->_move_lac(50);
    do {
        this->loop();
    } while (false == this->_connected);
}

void PenPlotterReplica::loop()
{
    if (0 < Serial2.available()) {
        int timeout_count = TIMEOUT_MS * 1000;
        uint16_t pos;
        uint16_t dat = Serial2.read();

        if ('P' == dat) {
            while (0 >= Serial2.available()) {
                timeout_count--;
                if (0 >= timeout_count) {
                    break;
                }
                delayMicroseconds(1);
            }
            if (0 < timeout_count) {
                pos = Serial2.read();
                this->_move_pen(pos);
            }
        } else if ('M' == dat) {
            while (1 >= Serial2.available()) {
                timeout_count--;
                if (0 >= timeout_count) {
                    break;
                }
                delayMicroseconds(1);
            }
            if (0 < timeout_count) {
                dat = Serial2.read();
                pos = Serial2.read();

                double postion = (dat * 0x100 + pos) / 100.0;
                this->_move_lac(postion);
            }
        } else if ('C' == dat) {
            while (0 >= Serial2.available()) {
                timeout_count--;
                if (0 >= timeout_count) {
                    break;
                }
                delayMicroseconds(1);
            }
            if (0 < timeout_count) {
                pos = Serial2.read();
                this->_connection(pos);
            }
        } else {
            Serial2.read();
            timeout_count = 0;
        }
        if (0 >= timeout_count) {
            this->_fin();
            Serial2.clearWriteError();
        }
    }
}
void PenPlotterReplica::pen_release()
{
    this->_move_pen(this->_postion_pen_default);
}

/////////////////////////////////////////////////
// private method
/////////////////////////////////////////////////
void PenPlotterReplica::_move_pen(unsigned int postion)
{
    if (0 <= postion && postion <= 180) {
        this->_pen.write(postion);
        delay(this->_servo_wait);
    }
    this->_fin();
}
void PenPlotterReplica::_move_lac(double postion)
{
    this->_lac.move(postion);
    this->_fin();
}
void PenPlotterReplica::_connection(unsigned int postion)
{
    this->_postion_pen_default = postion;
    this->_connected           = true;
    this->_fin();
}

void PenPlotterReplica::_fin()
{
    Serial2.write(0xAA);
}
