FROM ubuntu:bionic

RUN apt-get update && apt-get install -y build-essential python3 python3-pip
RUN apt-get install -y libxml2-dev libxslt-dev

COPY brle.cpp brle.hpp ./

RUN g++ -std=c++17 -O2  -o /bin/brle /brle.cpp

COPY ./requirements.txt .
# RUN pip3 install -r requirements.txt

VOLUME [ "/build" ]

WORKDIR /build

RUN mkdir /build/obj

CMD ["make", "all"]
