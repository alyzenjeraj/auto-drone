#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

class PIDController {
    public:
        PIDController(double kp, double ki, double kd);
        ~PIDController();
        void resetPID();
        double calculate(double setpoint, double current, double dt);

    private:
        double kp_, ki_, kd_;
        double integral_;
        double prev_error_;
};

#endif // PIDCONTROLLER_H