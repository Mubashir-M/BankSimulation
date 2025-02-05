# Base image with dependencies
FROM ubuntu:20.04 AS builder

# Set timezone and avoid interactive prompts
ENV TZ=Europe/Helsinki
ENV DEBIAN_FRONTEND=noninteractive
ENV DOCKER_BUILD=1

# Install required dependencies (builder stage)
RUN apt-get update && apt-get install -y --no-install-recommends \
    git curl gcc g++ clang build-essential make cmake unzip tar \
    ca-certificates libc6-dev libssl-dev libgl1-mesa-glx libglu1-mesa \
    libsdl2-dev libglew-dev sqlite3 libsqlite3-dev && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Install GoogleTest (optional)
RUN git clone --depth=1 https://github.com/google/googletest.git /tmp/googletest && \
    cd /tmp/googletest && mkdir build && cd build && \
    cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON && \
    make -j$(nproc) && make install && \
    rm -rf /tmp/googletest

# Install SQLiteCpp from source (fetch directly from GitHub)
RUN git clone --depth=1 https://github.com/SRombauts/SQLiteCpp.git /tmp/sqlitecpp && \
    cd /tmp/sqlitecpp && mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local \
             -DSQLITECPP_INTERNAL_SQLITE=OFF \
             -DSQLITECPP_BUILD_EXAMPLES=OFF \
             -DSQLITECPP_BUILD_TESTS=OFF \
             -DSQLite3_INCLUDE_DIR=/usr/include \
             -DSQLite3_LIBRARY=/usr/lib/x86_64-linux-gnu/libsqlite3.so && \
    make -j$(nproc) && make install && \
    ldconfig && \
    rm -rf /tmp/sqlitecpp

# Set working directory for your application
WORKDIR /app

# Copy project files into the container
COPY external /app/external
COPY . .

# Build the application
RUN mkdir -p build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/usr/local" .. && \
    make -j$(nproc)

# Run tests and ensure failures are captured
RUN cd build && ctest --output-on-failure || cat /app/build/Testing/Temporary/LastTest.log

# Runtime image to keep it lightweight
FROM ubuntu:20.04 AS runtime

# Install runtime dependencies (runtime stage)
RUN apt-get update && apt-get install -y --no-install-recommends \
    libsdl2-2.0-0 libglew2.1 sqlite3 && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Copy built application from builder stage
WORKDIR /app
COPY --from=builder /app/build/BankSimulation /app/BankSimulation

# Set the default command for the container
CMD ["./BankSimulation"]
