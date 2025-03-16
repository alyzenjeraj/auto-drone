#include <gz/transport14/gz/transport/Node.hh>
#include <gz/msgs11/gz/msgs/actuators.pb.h>

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <signal.h>

// Global flag for handling program termination
volatile sig_atomic_t running = 1;

// Signal handler for clean shutdown
void signalHandler(int signum) {
    std::cout << "\nInterrupt signal received. Shutting down..." << std::endl;
    running = 0;
}

int main(int argc, char** argv) {
    // Register signal handler for Ctrl+C
    signal(SIGINT, signalHandler);
    
    std::cout << "Drone Control Program Starting..." << std::endl;
    
    // Topic for sending motor speeds
    const std::string topic = "/X3/gazebo/command/motor_speed";
    
    // Create a transport node
    auto node = std::make_shared<gz::transport::Node>();
    
    // Base speed for hovering (adjust as needed for your drone model)
    double baseSpeed = 700.0;
    std::vector<double> motorSpeeds = {baseSpeed, baseSpeed, baseSpeed, baseSpeed};
    
    std::cout << "Sending motor speeds to topic: " << topic << std::endl;
    std::cout << "Press Ctrl+C to exit" << std::endl;
    
    // Simple control loop - repeatedly send hover commands
    while (running) {
        // Create an actuators message
        gz::msgs::Actuators msg;
        
        // Set the velocity values
        for (const auto& speed : motorSpeeds) {
            msg.add_velocity(speed);
        }
        
        // Publish the message
        if (!node->Publish(topic, msg)) {
            std::cerr << "Failed to publish motor speed message" << std::endl;
        } else {
            std::cout << "Sent motor speeds: [" 
                      << motorSpeeds[0] << ", " 
                      << motorSpeeds[1] << ", " 
                      << motorSpeeds[2] << ", " 
                      << motorSpeeds[3] << "]" << std::endl;
        }
        
        // Wait before sending the next command
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout << "Control program terminated." << std::endl;
    
    return 0;
}