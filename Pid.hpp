#pragma once
#include <vector>

class Pid {
public:
    Pid(float p = 0, float i = 0, float d = 0, float dAlpha = 0, unsigned int windup=0);

    float run(float err, float dt=1);

    unsigned int windup() const { return _windup; }

    void setWindup(unsigned int w);

    /** clear any windup */
    void clearWindup();

    float kp, ki, kd;
    float derivAlpha;

    void set_saturated(bool is_saturated) { _saturated = is_saturated; }

private:

    /** amount to sum up */
    unsigned int _windup;

    unsigned int _windupLoc;


    float _errSum;

    float _lastError;

    float _lastDeriv;

    std::vector<float> _oldErr{};
    /* [0, 1] -> [all new data, all old data] */

    bool _saturated;
};
