FROM ubuntu:latest

RUN apt-get update

RUN apt-get install cmake gcc clang gdb build-essential -y

WORKDIR /src/home/app

COPY . .

EXPOSE 8080/udp

EXPOSE 8081/tcp

CMD mkdir build && cd build && cmake .. && make && src/w1nja

