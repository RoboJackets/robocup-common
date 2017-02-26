#pragma once
#include <vector>

class PidTuner {
public:

    /**
     *@Brief Constructs a new PidTuner
     *@param ip The starting P value
     *@param ii The starting I value
     *@param id The starting D value
     *@param Sp The step used for P
     *@param Si The step used for I
     *@param Sd The step used for D
     */
    PidTuner(float ip, float ii, float id, float Sp=.5, float Si=.05, float Sd=.01);

    /** Starts a cycle to test a PID value
     *  Call Once at the beginning of a test cycle
     */
    void startCycle();

    /** Adds the error to the total score of the current PID set
     *  Call once each frame during a PID test
     */
    void run(float err);

    /** Ends the test of a PID value, returns True if more tuning is needed
     *  Call Once at the end of a test cycle
     */
    bool endCycle();

    float getP(){ return _currentPid.p; }
    float getI(){ return _currentPid.i; }
    float getD(){ return _currentPid.d; }

private:

    /** Struct to store P, I and D values, also stores a score of the error */
    struct PidSet {
        PidSet(float ip=0, float ii=0, float id=0);
        inline bool operator==(const PidSet& pidIn){
            return (p == pidIn.p) && (i == pidIn.i) && (d == pidIn.d);
        }
        float p;
        float i;
        float d;
        float score;
    };

    PidSet _initialPid;
    PidSet _currentPid;

    int _cycles;
    int _testNum;

    float _pScale;
    float _iScale;
    float _dScale;

    float _overScale;

    float _prevScore;

    float _threshold;

    float _overScale;

    std::vector <PidSet> _testSets;
};

