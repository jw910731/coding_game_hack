FROM ubuntu:bionic

RUN apt-get update && apt-get install -y build-essential

VOLUME [ "/build" ]

WORKDIR /build

RUN mkdir /build/obj

CMD ["make", "all"]
