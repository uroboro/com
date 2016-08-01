export TARGET = native:clang::latest

include $(THEOS)/makefiles/common.mk

TOOL_NAME = testDict testArray
testDict_FILES = tests/dictTest.c src/dictionary.c
testArray_FILES = tests/arrayTest.c src/array.c

include $(THEOS_MAKE_PATH)/tool.mk
