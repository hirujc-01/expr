#ifndef ANGLE_MODE_H
#define ANGLE_MODE_H

typedef enum
{
    ANGLE_RAD,
    ANGLE_DEG

} AngleMode;


void set_angle_mode(AngleMode mode);

AngleMode get_angle_mode(void);

double to_radians(double value);

#endif