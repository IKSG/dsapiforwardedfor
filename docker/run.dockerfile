# Build the 
FROM rikorose/gcc-cmake:gcc-10 as build

COPY --from=domino-docker:V1101_03212020prod /opt/hcl/domino/notes/latest/linux/libnotes.so /usr/local/lib64/
COPY . /usr/src/myapp/
RUN chmod +x /usr/src/myapp/docker/build-linux-inner.sh

WORKDIR /usr/src/myapp
RUN /usr/src/myapp/docker/build-linux-inner.sh

FROM maven:3.6.3-adoptopenjdk-8
RUN useradd -ms /bin/bash notes
USER root

# Bring in the Domino runtime
COPY --from=domino-docker:V1101_03212020prod /opt/hcl/domino /opt/hcl/domino
RUN mkdir -p /local/notesdata
# TODO check if there's a way to do this in a single ADD
COPY --from=domino-docker:V1101_03212020prod /tmp/notesdata.tbz2 /local/notesdata/
RUN cd /local/notesdata && \
    tar xjf notesdata.tbz2 && \
    rm notesdata.tbz2
RUN chown -R notes:notes /local/notesdata

COPY --chown=notes:notes docker/notesdata/* /local/notesdata/
COPY --from=build /usr/src/myapp/build/linux64/libdsapiforwardedfor.so /opt/hcl/domino/notes/latest/linux/

WORKDIR /local/notesdata
USER notes
CMD ["/opt/hcl/domino/bin/server"]
