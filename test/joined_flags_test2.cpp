/*****************************************************************************
 *
 * CLIPP - command line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include "testing.h"


namespace {

//-------------------------------------------------------------------
struct test_active {
    test_active() = default;
    test_active(bool a_, bool b_): a{a_}, b{b_} {}
    bool a = false, b = false;

    friend bool operator == (const test_active& x, const test_active& y) noexcept {
        return (x.a == y.a && x.b == y.b);
    }
};


//-------------------------------------------------------------------
static void
test(int lineNo,
     const std::initializer_list<const char*> args,
     const test_active& matches)
{
    using namespace clipp;
    test_active m;

    auto cli = joinable(
            option("-a").set(m.a),
            option("-b").set(m.b) );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = test_active{}; },
              [&]{ return m == matches; });
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    using std::string;

    try {
        test(__LINE__, {""}, test_active{});

        test(__LINE__, {"-a"},      test_active{1,0});
        test(__LINE__, {"-b"},      test_active{0,1});
        test(__LINE__, {"-a","-b"}, test_active{1,1});
        test(__LINE__, {"-b","-a"}, test_active{1,1});
        test(__LINE__, {"-ab"},     test_active{1,1});
        test(__LINE__, {"-ba"},     test_active{1,1});
        test(__LINE__, {"-a-b"},    test_active{1,1});
        test(__LINE__, {"-b-a"},    test_active{1,1});
        //fail cases
        test(__LINE__, {"--ab"}, test_active{});
        test(__LINE__, {"--ab"}, test_active{});
        test(__LINE__, {"bca"},  test_active{});
        test(__LINE__, {"bca-"}, test_active{});
        test(__LINE__, {"bc-a"}, test_active{});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
