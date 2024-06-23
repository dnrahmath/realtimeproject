# Gunakan image Ubuntu sebagai base
FROM ubuntu:latest

# Set working directory di dalam container
WORKDIR /app

# Update dan install dependensi yang dibutuhkan
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    libwebsocketpp-dev \
    libboost-all-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy source code ke dalam container
COPY RealTimeServer.cpp /app/

# Compile aplikasi C++
RUN g++ -std=c++11 -o RealTimeServer RealTimeServer.cpp -lwebsocketpp -lnlohmann_json -lboost_system -lpthread

# Expose port yang digunakan oleh server
EXPOSE 8080

# Command untuk menjalankan server saat container dijalankan
CMD ["./RealTimeServer"]
