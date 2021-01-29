FROM rikorose/gcc-cmake:gcc-10

COPY --from=domino-docker:V1101_03212020prod /opt/hcl/domino/notes/latest/linux/libnotes.so /usr/local/lib64/
COPY build-linux-inner.sh /
RUN chmod +x /build-linux-inner.sh

WORKDIR /usr/src/myapp
ENTRYPOINT ["/build-linux-inner.sh"]