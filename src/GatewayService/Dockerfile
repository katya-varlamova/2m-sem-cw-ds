FROM katyavarlamova/ds-project as builder
RUN git clone https://github.com/Thalhammer/jwt-cpp && mkdir jwt-cpp/build && cd jwt-cpp/build && cmake ../ &&  make && make install && cd / 

RUN apt install nlohmann-json3-dev

COPY ./GatewayService /src
WORKDIR /src
RUN mkdir build && cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++-10 -S./ -B./build

RUN cd build && make  
FROM katyavarlamova/ds-project

COPY --from=builder /src/ /app
RUN apt update && apt install python3
RUN pip3 install confluent-kafka
ENTRYPOINT ["/app/build/main/main", "/app/Configs/Config.yml"]
