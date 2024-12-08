# W1NJA

Simple P2P File-Sharing System

```bash
docker network create -d bridge w1nja-net
docker build -t w1nja1 ./
docker run -p 8080:8080/udp -p 8081:8081/tcp --network=w1nja-net --name=w1nja1 w1nja1
docker build -t w1nja2 ./
docker run -p 8082:8080/udp -p 8083:8081/tcp --network=w1nja-net --name=w1nja2 w1nja2
