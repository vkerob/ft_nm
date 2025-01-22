# docker build -t nm-project-alpine .
# docker run -it nm-project-alpine

FROM alpine:latest

RUN apk update && apk add --no-cache \
    build-base \
    binutils \
    gcc \
    gdb \
    make \
    musl-dev \
    python3 \
    py3-pip \
    git \
    vim \
    bash \
    && rm -rf /var/cache/apk/*

WORKDIR /app

COPY . /app

CMD ["/bin/bash"]
