FROM katyavarlamova/ds-project as builder
RUN git clone https://github.com/Thalhammer/jwt-cpp && mkdir jwt-cpp/build && cd jwt-cpp/build && cmake ../ &&  make && make install && cd / 
# RUN git clone https://github.com/arun11299/cpp-jwt.git && mkdir cpp-jwt/build && cd cpp-jwt/build && cmake -DCPP_JWT_BUILD_TESTS=False ../ && make && make install && cd / 

#RUN apt install nlohmann-json3-dev

COPY ./GatewayService /src
WORKDIR /src
RUN mkdir build && cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++-10 -S./ -B./build && \
    cd build && make

FROM katyavarlamova/ds-project

COPY --from=builder /src/ /app
ENTRYPOINT ["/app/build/main/main", "/app/Configs/Config.yml"]
