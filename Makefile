.PHONY: all
all:
	jbuilder build --dev

.PHONY: test
test:
	jbuilder runtest --force

.PHONY: clean
clean:
	jbuilder clean
