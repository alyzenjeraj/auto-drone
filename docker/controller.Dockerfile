# Use Ubuntu 22.04 as the base image
FROM ubuntu:22.04

# Set noninteractive mode to avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt update && apt upgrade -y && \
    apt install -y \
        curl \
        gnupg \
        build-essential \
        autoconf \
        libtool \
        pkg-config \
        cmake \
        clang \
        ninja-build \
        protobuf-compiler \
        libprotobuf-dev \
        libgrpc-dev \
        software-properties-common \
        lsb-release \
        wget \
        x11-apps  # For GUI support

# Install Bazel
RUN curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg \
    && mv bazel.gpg /etc/apt/trusted.gpg.d/ \
    && echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list \
    && apt-get update \
    && apt-get install -y bazel \
    && rm -rf /var/lib/apt/lists/*

# Install Gazebo Transport
RUN sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable $(lsb_release -cs) main" > /etc/apt/sources.list.d/gazebo-stable.list' && \
    wget https://packages.osrfoundation.org/gazebo.key -O - | apt-key add - && \
    apt update && \
    apt install -y \
    libgz-transport14-dev \
    libgz-msgs11-dev


# Set working directory inside the container (where your local controller directory will be mounted)
WORKDIR /app/controller

# Default command (can be overridden by docker-compose)
CMD ["bash"]
