# Build definitions for Linux-side test builds.

# Provide access to test infrastructure and test mock definitions.
INCS += test/include

# Enable tests in all source files: most of the doctest tests are
# inside the C++ files where the code they test is defined, bracketed
# by #ifdef TEST ... #endif.
COMMON_FLAGS += -DTEST
