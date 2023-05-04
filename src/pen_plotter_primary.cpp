/**
 * @file pen_plotter_primary.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2.0
 * @date 2023-05-03
 *
 * @copyright Copyright (c) 2023 / MaSiRo Project.
 *
 */
#include "pen_plotter_primary.hpp"

#include <Arduino.h>

PenPlotterPrimary::PenPlotterPrimary(int rx, int tx)
{
    this->_initialized = false;
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

void PenPlotterPrimary::begin()
{
    do {
        Serial2.write('C');
        Serial2.write(this->_pen_position_up);
    } while (false == this->_move_replica_wait());
    this->_initialized = true;
    this->postion_start();
}
void PenPlotterPrimary::loop()
{
}

void PenPlotterPrimary::pen_control(bool flag_write)
{
    if (true == this->_initialized) {
        Serial2.write('P');
        if (true == flag_write) {
            Serial2.write(this->_pen_position_down);
        } else {
            Serial2.write(this->_pen_position_up);
        }
        this->_move_replica_wait();
    }
}

void PenPlotterPrimary::move(double x, double y)
{
    if (true == this->_initialized) {
        double a_x          = 0;
        double a_y          = 0;
        double current_x    = (double)this->pos_x;
        double current_y    = (double)this->pos_y;
        double difference_x = (x - this->pos_x);
        double difference_y = (y - this->pos_y);
        int max_value       = (int)(max(abs(difference_x), abs(difference_y)) / this->_resolution);

        if (0 != max_value) {
            a_x = (double)(difference_x / max_value);
            a_y = (double)(difference_y / max_value);
        }

        for (int i = 0; i < max_value; i++) {
            current_x += a_x;
            current_y += a_y;
            this->_move(current_x, current_y);
        }
        this->_move(x, y);
    }
}

void PenPlotterPrimary::postion_start()
{
    this->pen_control(false);
    this->_move(this->POSTION_START_X, this->POSTION_START_Y);
}

/////////////////////////////////////////////////
// private method
/////////////////////////////////////////////////
void PenPlotterPrimary::_move(double x, double y)
{
    bool result = this->_move_replica_start(x);
    this->_move_primary(y);
    if (true == result) {
        this->_move_replica_wait();
    }
}

void PenPlotterPrimary::_move_primary(double target)
{
    if (this->pos_y != target) {
        this->_lac.move(target);
        this->pos_y = target;
    }
}

bool PenPlotterPrimary::_move_replica_start(double target)
{
    bool result = false;
    if (this->pos_x != target) {
        unsigned int send_data = (unsigned int)(target * 100.0);
        result                 = true;
        Serial2.write('M');
        Serial2.write((send_data >> 8) & 0x00FF);
        Serial2.write(send_data & 0x00FF);
        this->pos_x = target;
    }
    return result;
}
bool PenPlotterPrimary::_move_replica_wait()
{
    bool result      = true;
    int data         = 0;
    int timeout_cont = (1000) * this->TIMEOUT_MS;
    do {
        if (0 < Serial2.available()) {
            data = Serial2.read();
        } else {
            delayMicroseconds(1);
        }
        if (0 >= timeout_cont) {
            result = false;
            break;
        }
        timeout_cont--;
    } while (data != 0xAA);
    return result;
}
