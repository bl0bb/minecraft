FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    git \
    libglfw3-dev \
    libgl1-mesa-dev \
    libx11-dev \
    libxi-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libglew-dev \
    libglm-dev \
    zlib1g-dev \
    wget \
    unzip \
    && rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Copy source code
COPY . .

# Build
RUN cmake -Bbuild -H. && cmake --build build

# Set working directory for runtime
WORKDIR /app/build

CMD ["./your_executable_name"]
