FROM ubuntu:bionic

RUN apt-get update && apt-get install -y build-essential python3 python3-pip

COPY ./requirements.txt .
RUN pip3 install -r requirements.txt

VOLUME [ "/build" ]

WORKDIR /build

RUN mkdir /build/obj

CMD ["make", "all"]
