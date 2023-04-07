ARG WASM_TARGET=wasm32-wasip1

FROM rust:1.77-bookworm AS compile

ARG WASM_TARGET

WORKDIR /tmp
COPY / ./
RUN rustup default nightly && \
    rustup target add "${WASM_TARGET}" && \
    cargo build --bin node-launcher --release && \
    cargo build --bin xitca-web-wasm --features web --release --target "${WASM_TARGET}"

FROM node:20-bookworm

ARG BENCHMARK_ENV
ARG TFB_TEST_DATABASE
ARG TFB_TEST_NAME
ARG WASM_TARGET

COPY xitca-web.mjs /opt/xitca-web-wasm/
COPY --from=compile /tmp/target/release/node-launcher /opt/xitca-web-wasm/
COPY --from=compile \
     "/tmp/target/${WASM_TARGET}/release/xitca-web-wasm.wasm" \
     /opt/xitca-web-wasm/xitca-web.wasm
EXPOSE 8080

CMD /opt/xitca-web-wasm/node-launcher
