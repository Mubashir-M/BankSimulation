FROM ubuntu:20.04

# Set the timezone for the container
ENV TZ=Europe/Helsinki

# Set environment variables to avoid interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required dependencies (including development tools and utilities)
RUN apt-get update && apt-get install -y --no-install-recommends \
    git \
    curl \
    gcc \
    g++ \
    clang \
    build-essential \
    make \
    cmake \
    unzip \
    tar \
    ca-certificates \
    libc6-dev \
    libpthread-stubs0-dev \
    libssl-dev \
    libgtest-dev \
    libsdl2-dev \
    libsdl2-2.0-0 \
    libglew-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Clone and install ImGui
RUN git clone https://github.com/ocornut/imgui.git /usr/src/imgui && \
    cp -a /usr/src/imgui/*.h /usr/local/include/


# Verify the GCC version (ensure it's installed and works)
RUN gcc --version

# Set the working directory in the container
WORKDIR /app

# Copy the rest of your project files to the container
COPY . .

# Build GoogleTest directly from the source
RUN rm -rf /usr/src/gtest && \
    mkdir -p /usr/src/gtest && \
    cd /usr/src/gtest && \
    git clone https://github.com/google/googletest.git . && \
    cmake . && \
    make && \
    cp -a lib/* /usr/local/lib/ && \
    cp -a googletest/include/* /usr/local/include/ && \
    cp -a googlemock/include/* /usr/local/include/

# Create build directory, run cmake and make to build the project and run tests
RUN mkdir -p build && cd build && cmake .. && make && ctest -V > /app/test_results.log

# Set the default command for the container
CMD ["./build/BankSimulation"]