#include "angle_mode.h"
#include <math.h>

#define PI 3.14159265358979323846

static AngleMode current_mode = ANGLE_RAD;


void set_angle_mode(AngleMode mode)
{
    current_mode = mode;
}


AngleMode get_angle_mode(void)
{
    return current_mode;
}


double to_radians(double value)
{
    if (current_mode == ANGLE_DEG)
        return value * PI / 180.0;

    return value;
}