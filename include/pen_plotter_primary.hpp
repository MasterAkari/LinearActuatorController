/**
 * @file pen_plotter_primary.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2.0
 * @date 2023-05-03
 *
 * @copyright Copyright (c) 2023 / MaSiRo Project.
 *
 */
#ifndef PEN_PLOTTER_PRIMARY_HPP
#define PEN_PLOTTER_PRIMARY_HPP

#include "linear_actuator_controller.hpp"

class PenPlotterPrimary {
public:
    PenPlotterPrimary(int rx = 32, int tx = 26);

public:
    void begin();
    void loop();

    void pen_control(bool flag_write);

    void postion_start();
    void move(double x, double y);

    double pos_x = 0;
    double pos_y = 0;

private:
    void _move(double x, double y);
    void _move_primary(double target);
    bool _move_replica_start(double target);
    bool _move_replica_wait();

private:
    LinearActuatorController _lac;

    int _pen_position_up   = 90;
    int _pen_position_down = 110;
    double _resolution     = 1;
    bool _initialized      = false;

private:
    const int TIMEOUT_MS      = 500;
    const int POSTION_START_X = 50;
    const int POSTION_START_Y = 25;
};

#endif
