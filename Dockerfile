FROM registry.cn-shanghai.aliyuncs.com/lebai/l-master:latest AS base

CMD [ "bash" ]

RUN apt-get update \
        && DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential doxygen python3-pip python3-dev

FROM base AS lebai-sdk
