include $(THEOS)/makefiles/common.mk

TOOL_NAME = test
test_FILES = tests/dict.c src/dictionary.c

include $(THEOS_MAKE_PATH)/tool.mk
