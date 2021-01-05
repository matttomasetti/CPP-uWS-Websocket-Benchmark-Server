FROM ubuntu

# Set timezone
ENV TZ=America/New_York
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Add source files to docker image
ADD .	/home/websocket

# Update and install dependencies
RUN apt-get -y update \
    && apt-get -y upgrade \
    && apt-get -y install cmake wget git build-essential libssl-dev libboost-all-dev libuv1-dev

#Compile uWS
RUN cd /home \
    && git clone https://github.com/uWebSockets/uWebSockets \
    && cd uWebSockets \
    && git checkout e94b6e1 \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make \
    && make install \
    && ls

#Compile websocket
RUN cd /home/websocket \
    && wget https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp \
    && cmake configure . \
    && cmake . \
    && make

EXPOSE 8080

WORKDIR /home/websocket
CMD ["./bin/cpp-uws-websocket-server"]
