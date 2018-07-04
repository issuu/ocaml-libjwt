# OCaml Bindings for libjwt

This is bindings for [libjwt](https://github.com/benmcollins/libjwt)

## Development

Setup opam switch (this requires OPAM2) and install dependencies:
```
brew install libjwt
opam switch create ./ ocaml-base-compiler.4.06.1
```

Now you can build and run the test program with with `make test`
