/**
 * @file linear_actuator_controller.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2023-05-01
 *
 * @copyright Copyright (c) 2023 / MaSiRo Project.
 *
 */
#include "linear_actuator_controller.hpp"

#define POS_MIN (0)
#ifdef ANALOG_10_BIT
#define POS_MAX (1023)
#else
#define POS_MAX (4095)
#endif

LinearActuatorController::LinearActuatorController(uint8_t mt_driver_1, uint8_t mt_driver_2, uint8_t adc)
{
    this->_initialized = false;
    this->_mt_driver_1 = mt_driver_1;
    this->_mt_driver_2 = mt_driver_2;
    this->_adc         = adc;
    this->setup_margin(5, 95);
    pinMode(this->_adc, INPUT); // ADC
}

void LinearActuatorController::setup_margin(double inside_percentage, double outside_percentage)
{
    this->_margin_min = (uint16_t)((POS_MAX - POS_MIN) * (max(0.0, min(100.0, inside_percentage))) / 100.0);
    this->_margin_max = (uint16_t)((POS_MAX - POS_MIN) * (max(0.0, min(100.0, outside_percentage))) / 100.0);
}

void LinearActuatorController::begin(uint16_t timeout_ms)
{
    pinMode(this->_mt_driver_1, OUTPUT);                // Motor Driver[IN1]
    pinMode(this->_mt_driver_2, OUTPUT);                // Motor Driver[IN2]
    digitalWrite(this->_mt_driver_1, this->MOTOR_STOP); // Motor Stop[IN1]
    digitalWrite(this->_mt_driver_2, this->MOTOR_STOP); // Motor Stop[IN2]

    this->_timeout_count_max = (timeout_ms * 1000) / this->_samples_analog;
    this->_initialized       = true;
    this->_pos               = this->_read_position(this->_adc);
}

bool LinearActuatorController::move(double percentage)
{
    bool result = true;
    if (true == this->_initialized) {
        MODE_LINEAR_ACTUATOR_T mode;
        uint32_t timeout = this->_timeout_count_max;
        uint16_t target  = 0;
        if (percentage < 0) {
            percentage = 0;
        } else if (percentage > 100) {
            percentage = 100;
        }
        target = (uint16_t)((POS_MAX - POS_MIN) * (percentage / 100.0));

        if (target > this->_pos) {
            mode = MODE_LINEAR_ACTUATOR_OUTSIDE;
            digitalWrite(this->_mt_driver_1, this->MOTOR_RUN);
            digitalWrite(this->_mt_driver_2, this->MOTOR_STOP);
            log_d(" Postion : OUTSIDE : %d -> %d[%0.3f%%]", this->_pos, target, percentage);
        } else if (target < this->_pos) {
            mode = MODE_LINEAR_ACTUATOR_INSIDE;
            digitalWrite(this->_mt_driver_1, this->MOTOR_STOP);
            digitalWrite(this->_mt_driver_2, this->MOTOR_RUN);
            log_d(" Postion : INSIDE  : %d -> %d[%0.3f%%]", this->_pos, target, percentage);
        } else {
            mode = MODE_LINEAR_ACTUATOR_NOT_MOVING;
            log_d(" NOT MOVING");
        }

        while (MODE_LINEAR_ACTUATOR_NOT_MOVING != mode) {
            this->_pos = this->_read_position(this->_adc);
            log_v(" MOVE : %s[%d -> %d]", (MODE_LINEAR_ACTUATOR_INSIDE == mode) ? "INSIDE " : "OUTSIDE", this->_pos, target);

            if ((MODE_LINEAR_ACTUATOR_OUTSIDE == mode) && (target <= this->_pos)) {
                mode = MODE_LINEAR_ACTUATOR_NOT_MOVING;
                break;
            } else if ((MODE_LINEAR_ACTUATOR_INSIDE == mode) && (target >= this->_pos)) {
                mode = MODE_LINEAR_ACTUATOR_NOT_MOVING;
                break;
            }
            if (0 > timeout) {
                result = false;
                break;
            }
            timeout--;
        }
        // Motor Brake
        digitalWrite(this->_mt_driver_1, this->MOTOR_STOP);
        digitalWrite(this->_mt_driver_2, this->MOTOR_STOP);
    } else {
        result = false;
    }
    return result;
}

double LinearActuatorController::get_position_percentage()
{
    uint16_t pos      = this->_read_position(this->_adc);
    double percentage = ((double)(pos - POS_MIN) / (double)(POS_MAX - POS_MIN)) * 100.0;
    return percentage;
}

////////////////////////////////////////////////////////////////////////////
// Private Methods
////////////////////////////////////////////////////////////////////////////

uint16_t LinearActuatorController::_read_position(uint8_t pin_no)
{
    uint16_t pos     = 0;
    uint16_t max_pos = POS_MIN;
    uint16_t min_pos = POS_MAX;

    for (int8_t i = 0; i < this->_samples_analog; i++) {
        uint16_t dat = analogRead(pin_no);
        pos += dat;
        max_pos = max(max_pos, dat);
        min_pos = min(min_pos, dat);
        delayMicroseconds(1);
    }
    //////////////////////
    // Exclude outliers
    pos -= max_pos;
    pos -= min_pos;
    //////////////////////
    pos = pos / (this->_samples_analog - 2);
    if (pos < this->_margin_min) {
        pos = this->_margin_min;
    } else if (pos > this->_margin_max) {
        pos = this->_margin_max;
    }
    pos = map(pos, this->_margin_min, this->_margin_max, POS_MIN, POS_MAX);

    return pos;
}
