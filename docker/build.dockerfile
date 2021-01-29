FROM rust:1.49.0

RUN apt update && apt install -y llvm-dev libclang-dev clang

COPY --from=domino-docker:V1101_03212020prod /opt/hcl/domino/notes/latest/linux/libnotes.so /usr/local/lib64/

WORKDIR /usr/src/myapp
ENTRYPOINT ["cargo", "build"]