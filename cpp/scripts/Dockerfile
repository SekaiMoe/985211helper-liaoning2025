FROM alpine:edge as builder
WORKDIR /build
COPY . .
RUN apk add --no-cache cmake build-base samurai python3 linux-headers && \
    cd cpp && mkdir out && cd out && \
    cmake .. -DAPPUI_WEBUI=on -DDEBUG_MODE=off -DSTATIC_BUILD=on -DBUILD_TEST=off -GNinja && \
    ninja && \
    mv helper ../../out/

FROM alpine:edge
WORKDIR /build
COPY --from=builder /build/out/helper .
COPY --from=builder /build/out/metadata.json .
CMD ["./helper", "-s"]

EXPOSE 4000
