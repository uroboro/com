include $(THEOS)/makefiles/common.mk

TOOL_NAME = test
test_FILES = test/dict.c sources/dictionary.c

include $(THEOS_MAKE_PATH)/tool.mk
