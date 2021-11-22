OUT = 42sh
BIN = builddir
TEST_DIT = tests

all: $(OUT)
$(OUT):
	meson setup builddir
	ninja -C builddir

check:	clean
	@cd $(TEST_DIR)
	@python run.py ../$(BIN)/$(OUT) 0

RM += -r
clean:
	$(RM) $(BIN)

