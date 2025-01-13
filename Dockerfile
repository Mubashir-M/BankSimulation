# Start from your base image
FROM ubuntu:20.04

# Set the timezone for the container
ENV TZ=Europe/Berlin

# Set environment variables to avoid interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required dependencies (including development tools and utilities)
RUN apt-get update && apt-get install -y --no-install-recommends \
    git \
    curl \
    gcc-7 \
    g++-7 \
    clang-10 \
    build-essential \
    cmake \
    unzip \
    tar \
    ca-certificates \
    libpthread-stubs0-dev \
    libgtest-dev && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Set gcc-7 and g++-7 as the default compilers
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 100

# Verify the GCC version
RUN gcc --version

# Set the working directory in the container
WORKDIR /app

# Copy the rest of your project files to the container
COPY . .

# Run the build and tests, output test results to a log file
RUN mkdir -p build && cd build && cmake .. && make && ctest -V > /app/test_results.log

# Set the default command for the container
CMD ["./build/BankSimulation"]
