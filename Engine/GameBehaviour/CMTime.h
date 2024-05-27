#include <iostream>

class CMTime
{
protected:
    static double _dt;

public:
    static double scaleFactor;

    static double dt() { return _dt * scaleFactor;; }
    static double unscaledDt() { return _dt; };
};

double CMTime::_dt = 0.0167;
double CMTime::scaleFactor = 1.0;