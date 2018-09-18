.PHONY: all
all:
	dune build

.PHONY: test
test:
	dune runtest --force

.PHONY: clean
clean:
	dune clean

compile_commands.json:
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' _build/default/src/*.o.json | jq -c '.|map(.directory=(.directory|sub("_build/default/";"")))' > $@
