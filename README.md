# Domino X-Forwarded DSAPI Filter

## Running test server

To run the test server, copy a names.nsf, notes.ini, and server.id to the `docker/notesdata` directory. Ensure that
the server referenced by the notes.ini and server.id has `libdsapiforwardedfor.so` in the "DSAPI filter file names"
field of an active web site config or the server document.

The script and Dockerfile assume that Domino will host HTTP on port 8082, so tweak those two files as necessary to
match your config.

## Updating Notes Bindings

The Notes API binding are created using bindgen, though are slightly tweaked afterwards.

The headers included are defined in `notes.h` - to add an additional Notes API header:

- Add it as an `#include` line in `notes.h`
- Uncomment `bindgen = "0.56.0"` in `Cargo.toml`
- Uncomment all of the code in `build.rs`
- Run `build-linux.sh`
- Run `cp target/debug/build/dsapiforwardedfor-*/out/bindings.rs src/notesapi.rs`
- Reformat the text of that file for good measure
- Change the type of the first parameter for `AddInLogMessageText` to `*const i8`
- Change the type of the `filterDesc` member of the `FilterInitData` struct to `[u8; 256usize]`
	- Ideally, I'd tweak the lib code to not require these two changes, but I don't know Rust enough to do that yet
- Add the following warning-suppression lines to the top of the file:

```
#![allow(dead_code)]
#![allow(non_camel_case_types)]
#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
```

### Header Files

To rebuild the bindings, copy the header files files from the `include` directory Notes/Domino C API Toolkit
into the "thirdparty/notesapi" directory.