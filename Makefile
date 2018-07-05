.PHONY: all
all:
	jbuilder build --dev

.PHONY: test
test:
	jbuilder runtest --force

.PHONY: clean
clean:
	jbuilder clean

compile_commands.json:
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' _build/default/src/*.o.json | jq -c '.|map(.directory=(.directory|sub("_build/default/";"")))' > $@
