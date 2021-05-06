# Haven't figured this out.

FROM gcc:9.2

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y cmake
RUN apt-get update && apt-get install -y libsdl2-dev libegl1-mesa-dev libgles2-mesa-dev liblua5.3-dev
RUN ls -a
RUN cmake
RUN cmake --build ..
