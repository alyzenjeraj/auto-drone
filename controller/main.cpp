#include <gz/transport/Node.hh>
#include <gz/msgs/actuators.pb.h>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Initialize Gazebo Transport Node
    gz::transport::Node node;

    // Define the topic to publish motor speed commands
    std::string motor_speed_topic = "/X3/gazebo/command/motor_speed";

    // Create a publisher
    auto pub = node.Advertise<gz::msgs::Actuators>(motor_speed_topic);

    // Wait for the publisher to be ready
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Starting Drone Controller..." << std::endl;

    // Takeoff sequence (increase throttle gradually)
    for (int speed = 0; speed <= 700; speed += 50) {
        gz::msgs::Actuators msg;
        msg.add_velocity(speed);
        msg.add_velocity(speed);
        msg.add_velocity(speed);
        msg.add_velocity(speed);

        pub.Publish(msg);
        std::cout << "Sending motor speeds: " << speed << ", " << speed << ", " << speed << ", " << speed << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Hover for 5 seconds
    std::cout << "Hovering..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Landing sequence (reduce throttle gradually)
    for (int speed = 700; speed >= 200; speed -= 50) {
        gz::msgs::Actuators msg;
        msg.add_velocity(speed);
        msg.add_velocity(speed);
        msg.add_velocity(speed);
        msg.add_velocity(speed);

        pub.Publish(msg);
        std::cout << "Sending motor speeds: " << speed << ", " << speed << ", " << speed << ", " << speed << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Drone Landed Successfully!" << std::endl;
    return 0;
}
