/**
 * @file linear_actuator_controller.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2023-05-01
 *
 * @copyright Copyright (c) 2023 / MaSiRo Project.
 *
 */
#ifndef LINEAR_ACTUATOR_CONTROLLER_HPP
#define LINEAR_ACTUATOR_CONTROLLER_HPP

#include <Arduino.h>

class LinearActuatorController {
public:
    /**
     * @brief Construct a new Linear Actuator Controller object
     *
     * @param mt_driver_1
     * @param mt_driver_2
     * @param adc
     */
    LinearActuatorController(uint8_t mt_driver_1 = 22, //
                             uint8_t mt_driver_2 = 19,
                             uint8_t adc         = 33);

    /**
     * @brief Set the up margin object
     *
     * @param inside_percentage
     * @param outside_percentage
     */
    void setup_margin(double inside_percentage, double outside_percentage);
    /**
     * @brief
     *
     * @param timeout_ms time out unit:[ms]
     */
    void begin(uint16_t timeout_ms = (60 * 1000));
    /**
     * @brief move linear actuator
     *
     * @param percentage
     */
    bool move(double percentage);
    /**
     * @brief Get the position
     *      unit:[%]
     *
     * @return double value
     */
    double get_position_percentage();

private:
    typedef enum
    {
        MODE_LINEAR_ACTUATOR_NOT_MOVING,
        MODE_LINEAR_ACTUATOR_INSIDE,
        MODE_LINEAR_ACTUATOR_OUTSIDE,
    } MODE_LINEAR_ACTUATOR_T;
    uint16_t _read_position(uint8_t pin_no);

private:
    uint8_t _mt_driver_1 = 0;
    uint8_t _mt_driver_2 = 0;
    uint8_t _adc         = 0;
    uint16_t _pos        = 0;

    int8_t _samples_analog = 16;
    uint16_t _margin_min;
    uint16_t _margin_max;

    uint32_t _timeout_count_max = 0xFFFFFFFF;

    bool _initialized = false;

private:
    const uint8_t MOTOR_STOP = LOW;
    const uint8_t MOTOR_RUN  = HIGH;
};

#endif
