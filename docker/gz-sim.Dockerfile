FROM osrf/ros:humble-desktop

# Set noninteractive mode to avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y wget

# Install Gazebo Sim
RUN sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable $(lsb_release -cs) main" > /etc/apt/sources.list.d/gazebo-stable.list' && \
    wget https://packages.osrfoundation.org/gazebo.key -O - | apt-key add - && \
    apt update && \
    apt install -y \
    gz-tools \
    libgz-sim7-dev \
    libgz-transport14-dev

# Verify installation
RUN gz sim --version
