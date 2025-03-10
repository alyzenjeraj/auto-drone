# Autonomous Drone

## Getting Started

1. Set the models target location for Gazebo:

```bash
export GZ_SIM_RESOURCE_PATH=$GZ_SIM_RESOURCE_PATH:/home/alyzen/git/auto-drone/simulation/models
```

2. Test Messages
```bash
gz topic -t /X3/gazebo/command/motor_speed --msgtype gz.msgs.Actuators -p 'velocity:[700, 700, 700, 700]'
```
- And to turn off propellers:
```bash
gz topic -t /X3/gazebo/command/motor_speed --msgtype gz.msgs.Actuators -p 'velocity:[0, 0, 0, 0]'

gz topic -t /X3/gazebo/command/motor_speed -m gz.msgs.Actuators -p 'velocity: [700, 700, 700, 700]'
```

### Building the Docker Images
```bash
docker build -t controller-builder -f docker/controller.Dockerfile .
```
### Running the Docker Images
```bash
docker run -v $(pwd):/workspace controller-builder
```
