# Run the rest in a Java-friendly world
FROM maven:3.6.3-adoptopenjdk-8
RUN useradd -ms /bin/bash notes

USER root
RUN apt update && apt install -y vim

# Bring in the Domino runtime
COPY --from=domino-docker:V1101_03212020prod /opt/hcl/domino /opt/hcl/domino
RUN mkdir -p /local/notesdata
# TODO check if there's a way to do this in a single ADD
COPY --from=domino-docker:V1101_03212020prod /tmp/notesdata.tbz2 /local/notesdata/
RUN cd /local/notesdata && \
    tar xjf notesdata.tbz2 && \
    rm notesdata.tbz2

# Copy in the built application
RUN ln -s /usr/src/myapp/target/debug/libdsapiforwardedfor.so /opt/hcl/domino/notes/11000100/linux/libdsapiforwardedfor.so

# Copy in our Domino server config
COPY --chown=notes:notes notesdata/* /local/notesdata/

EXPOSE 8880 1352 80 443 8889 8890

ENV NOTESBIN="/opt/hcl/domino/notes/11000100/linux"
ENV DEBUG="true"
RUN mkdir /local/notesdata/keepconfig.d
RUN chown notes:notes /local/notesdata/keepconfig.d

EXPOSE 8082

WORKDIR /local/notesdata
USER notes
CMD ["/opt/hcl/domino/bin/server"]