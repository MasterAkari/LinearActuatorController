/**
 * @file PenPlotter_primary.ino
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2.0
 * @date 2023-05-03
 *
 * @copyright Copyright (c) 2023 / MaSiRo Project.
 *
 */
#include "pen_plotter_primary.hpp"

#include <ESP32Servo.h>
#include <M5Atom.h>

PenPlotterPrimary pp;

void setup_pen_plotter()
{
    pp.begin();
}

void loop_pen_plotter()
{
    pp.loop();
}

////////////////////////////////////////////////////////////
// M5 ATOM
////////////////////////////////////////////////////////////

void setup()
{
    M5.begin(true, false, true);
    M5.dis.begin();
    M5.dis.fillpix(CRGB::Yellow);
    ///////////////////////////////
    setup_pen_plotter();
    ///////////////////////////////
    M5.dis.fillpix(CRGB::Green);
}

void loop()
{
    M5.update();
    if (true == M5.Btn.wasReleased()) {
        M5.dis.fillpix(CRGB::Yellow);
        ////////////////////////
        loop_pen_plotter();
        ////////////////////////
        M5.dis.fillpix(CRGB::Green);
    }
}
