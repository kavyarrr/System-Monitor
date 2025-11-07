# Use Ubuntu as base image (provides /proc and Linux environment)
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies and ncurses
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    make \
    libncurses5-dev \
    libncursesw5-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source files
COPY CMakeLists.txt Makefile ./
COPY src/ ./src/
COPY include/ ./include/

# Build the application
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make

# Set the entrypoint to run the monitor
ENTRYPOINT ["./build/monitor"]

