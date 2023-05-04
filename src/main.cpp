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
#if PLOT_PRIMARY || PLOT_REPLICA || PLOT_TEST

#include <ESP32Servo.h>
#include <M5Atom.h>

////////////////////////////////////////////////////////////
// PLOT_PRIMARY
////////////////////////////////////////////////////////////
#if PLOT_PRIMARY

#include "pen_plotter_primary.hpp"

PenPlotterPrimary pp;

void setup_pen_plotter()
{
    M5.dis.fillpix(CRGB::Yellow);
    pp.postion_start();
    M5.dis.fillpix(CRGB::Green);
}

void loop_pen_plotter()
{
    pp.loop();
}

////////////////////////////////////////////////////////////
// PLOT_REPLICA
////////////////////////////////////////////////////////////
#elif PLOT_REPLICA

#include "pen_plotter_replica.hpp"

PenPlotterReplica pp;

void setup_pen_plotter()
{
    M5.dis.fillpix(CRGB::Yellow);
    pp.begin();
    M5.dis.fillpix(CRGB::Black);
}

void loop_pen_plotter()
{
    pp.loop();
    M5.update();
    if (true == M5.Btn.wasReleased()) {
        pp.pen_release();
    }
}

////////////////////////////////////////////////////////////
// PLOT_PRIMARY[TEST]
////////////////////////////////////////////////////////////
#elif PLOT_TEST

#include "pen_plotter_primary.hpp"

PenPlotterPrimary pp;

void setup_pen_plotter()
{
    M5.dis.fillpix(CRGB::Yellow);
    if (0 < Serial.available()) {
        Serial.read();
    }
    pp.begin();
    M5.dis.fillpix(CRGB::Green);
}

void loop_pen_plotter()
{
    if (0 < Serial.available()) {
        log_d("==============================");
        unsigned int pos_min    = 0;
        unsigned int pos_max    = 100;
        unsigned int pos_middle = (unsigned int)(((double)abs((int)pos_max - (int)pos_min) / 2.0) + pos_min);

        uint16_t dat = Serial.read();
        log_d("  char: %c", dat);
        switch (dat) {
            case 'p':
                pp.pen_control(true);
                break;
            case 'o':
                pp.pen_control(false);
                break;
            case 'q':
                pp.move(pos_min, pos_min);
                break;
            case 'w':
                pp.move(pos_middle, pos_min);
                break;
            case 'e':
                pp.move(pos_max, pos_min);
                break;
            case 'a':
                pp.move(pos_min, pos_middle);
                break;
            case 's':
                pp.move(pos_middle, pos_middle);
                break;
            case 'd':
                pp.move(pos_max, pos_middle);
                break;
            case 'z':
                pp.move(pos_min, pos_max);
                break;
            case 'x':
                pp.move(pos_middle, pos_max);
                break;
            case 'c':
                pp.move(pos_max, pos_max);
                break;

            default:
                break;
        }
    }
}
#endif

////////////////////////////////////////////////////////////
// M5 ATOM
////////////////////////////////////////////////////////////

void setup()
{
    M5.begin(true, false, true);
    M5.dis.begin();
    ///////////////////////////////
    setup_pen_plotter();
    ///////////////////////////////
}

void loop()
{
    ///////////////////////////////
    loop_pen_plotter();
    ///////////////////////////////
}
#endif
