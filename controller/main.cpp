#include <gz/transport/Node.hh>
#include <gz/msgs/actuators.pb.h>
#include <gz/msgs/pose_v.pb.h>
#include <iostream>
#include <thread>
#include <chrono>

// Global variable to store the latest pose for X3_UAV
gz::msgs::Pose latestPose;
bool poseReceived = false;

// PID controller parameters for altitude (z-axis)
double setpoint = 2.0;
double Kp = 50.0;     
double Ki = 0.0;      
double Kd = 20.0;     

double prevError = 0.0;
double integral = 0.0;
std::chrono::steady_clock::time_point prevTime;

// Callback function to receive pose updates
void poseCallback(const gz::msgs::Pose_V &msg) {
    for (int i = 0; i < msg.pose_size(); i++) {
        auto pose = msg.pose(i);
        if (pose.name() == "X3_UAV") {
            latestPose = pose;
            poseReceived = true;
        }
    }
}

// Function to compute elapsed time in seconds
double computeDt(std::chrono::steady_clock::time_point &prevTime) {
    auto now = std::chrono::steady_clock::now();
    double dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - prevTime).count() / 1000.0;
    prevTime = now;
    return dt;
}

int main() {
    gz::transport::Node node;

    std::string poseTopic = "/world/quadcopter/dynamic_pose/info";
    node.Subscribe(poseTopic, &poseCallback);

    std::string motorSpeedTopic = "/X3/gazebo/command/motor_speed";
    auto pub = node.Advertise<gz::msgs::Actuators>(motorSpeedTopic);

    std::cout << "Starting Hover PID Controller..." << std::endl;

    while (!poseReceived) {
        std::cout << "Waiting for first pose update..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    prevTime = std::chrono::steady_clock::now();

    int baseSpeed = 700;

    while (true) {
        // Ensure we have a valid pose update
        if (!poseReceived) {
            std::cerr << "No pose data available." << std::endl;
            continue;
        }

        // Get current altitude from the pose (z-axis)
        double currentAltitude = latestPose.position().z();
        // Compute error between setpoint and current altitude
        double error = setpoint - currentAltitude;
        // Compute time difference
        double dt = computeDt(prevTime);

        integral += error * dt;
        double derivative = (error - prevError) / dt;
        prevError = error;

        double correction = (Kp * error) + (Ki * integral) + (Kd * derivative);

        int newSpeed = static_cast<int>(baseSpeed + correction);

        if (newSpeed < 400) newSpeed = 400;
        if (newSpeed > 800) newSpeed = 800;

        gz::msgs::Actuators motorMsg;
        motorMsg.add_velocity(newSpeed);
        motorMsg.add_velocity(newSpeed);
        motorMsg.add_velocity(newSpeed);
        motorMsg.add_velocity(newSpeed);
        pub.Publish(motorMsg);

        std::cout << "dt: " << dt << " s, "
                  << "Error: " << error << ", "
                  << "Correction: " << correction << ", "
                  << "New Motor Speed: " << newSpeed << ", "
                  << "Current Altitude: " << currentAltitude << " m" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
