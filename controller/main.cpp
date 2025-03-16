#include <gz/transport/Node.hh>
#include <gz/msgs/actuators.pb.h>
#include <gz/msgs/pose_v.pb.h>
#include <iostream>
#include <thread>
#include <chrono>

const double TARGET_HEIGHT = 5.0; // Target height (m)

// Global variables
gz::msgs::Pose latestPose;
bool poseReceived = false;

// Callback function for receiving pose updates
void poseCallback(const gz::msgs::Pose_V &msg) {
    for (int i = 0; i < msg.pose_size(); i++) {
        auto pose = msg.pose(i);
        if (pose.name() == "X3_UAV") {
            latestPose = pose;
            poseReceived = true;
        }
    }
}

// Function to print the current drone pose
void printPose() {
    if (poseReceived) {
        std::cout << "Drone Pose (X3_UAV): x=" << latestPose.position().x()
                  << ", y=" << latestPose.position().y()
                  << ", z=" << latestPose.position().z()
                  << " | Orientation (w, x, y, z): "
                  << latestPose.orientation().w() << ", "
                  << latestPose.orientation().x() << ", "
                  << latestPose.orientation().y() << ", "
                  << latestPose.orientation().z() << std::endl;
    } else {
        std::cout << "Waiting for first pose update..." << std::endl;
    }
}

int main() {
    gz::transport::Node node;

    // Subscribe to pose topic
    std::string pose_topic = "/world/quadcopter/dynamic_pose/info";
    node.Subscribe(pose_topic, &poseCallback);

    // Advertise motor speed topic
    std::string motor_speed_topic = "/X3/gazebo/command/motor_speed";
    auto pub = node.Advertise<gz::msgs::Actuators>(motor_speed_topic);

    std::cout << "Starting Drone Controller..." << std::endl;

    // Wait for pose updates to start
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Takeoff sequence (increase throttle gradually)
    for (int speed = 500; speed <= 700; speed += 50) {
        gz::msgs::Actuators msg;
        msg.add_velocity(speed);
        msg.add_velocity(speed);
        msg.add_velocity(speed);
        msg.add_velocity(speed);

        pub.Publish(msg);
        printPose();

        std::cout << "Sending motor speeds: " << speed << ", " << speed << ", " << speed << ", " << speed << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Hover for 5 seconds
    std::cout << "Hovering..." << std::endl;
    for (int i = 0; i < 5; i++) {
        printPose();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Landing sequence (reduce throttle gradually)
    for (int speed = 700; speed >= 400; speed -= 50) {
        gz::msgs::Actuators msg;
        msg.add_velocity(speed);
        msg.add_velocity(speed);
        msg.add_velocity(speed);
        msg.add_velocity(speed);

        pub.Publish(msg);
        printPose();

        std::cout << "Sending motor speeds: " << speed << ", " << speed << ", " << speed << ", " << speed << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Drone Landed Successfully!" << std::endl;
    return 0;
}
