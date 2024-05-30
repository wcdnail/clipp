#pragma once

#ifdef ALL_EXAMPLES_AS_SINGLE_PRJ
#  define EXAMPLE_MAIN _STR_CAT(THIS_EXAMPLE_FNAME, _main)
#else
#  define EXAMPLE_MAIN main
#endif
