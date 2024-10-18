FROM alpine:latest as builder
WORKDIR /build
COPY . .
RUN apk add --no-cache cmake build-base samurai && \
    cd cpp && mkdir out cd out && cmake .. -DAPPUI_WEBUI=on -DDEBUG_MODE=off -GNinja && \
    ninja && mv helper ../../out/

FROM alpine:latest
WORKDIR /build
COPY --from=builder /build/out/* .
CMD ["./helper -s"]
