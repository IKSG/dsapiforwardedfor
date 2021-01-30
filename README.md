# Domino Reverse Proxy Check Filter

This project is a DSAPI filter that adds a verification check for requests coming in with `$WS*` connector headers,
as processed by Domino when `HTTPEnableConnectorHeaders=1` is set.

The mechanics of this are that it reads the `HTTPConnectorHeadersSecret` notes.ini property for a shared secret value,
and then looks for incoming requests that use the connector headers. Such requests must also contain a
`X-ConnectorHeaders-Secret` set to the same value as in notes.ini; if they don't, the filter immediately returns
a 404.

## Limitations

Since this method relies on a shared secret instead of network-level test, it creates the possibility that the shared
secret could get out.

Since the maximum size for a notes.ini property according to the SDK is 256, that's the upper limit for the shared key.

## Running test server

To run the test server, copy a names.nsf, notes.ini, and server.id to the `docker/notesdata` directory. Ensure that
the server referenced by the notes.ini and server.id has `libdsapiforwardedfor.so` in the "DSAPI filter file names"
field of an active web site config or the server document.

## Building

To build, copy the header files files from the `include` directory Notes/Domino C API Toolkit into the
"thirdparty/notesapi" directory