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
    explicit
    test_active(bool a_, bool b_, bool c_, bool d_):
        a{a_}, b{b_}, c{c_}, d{d_}
    {}
    bool a = false, b = false, c = false, d = false;
    bool conflict = false;
    std::size_t missing  = 0;

    friend bool operator == (const test_active& x, const test_active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d &&
                x.conflict == y.conflict && x.missing == y.missing);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const test_active& matches)
{
    using namespace clipp;

    {
        test_active m;

        auto cli = required("-a").set(m.a) |
                   required("-b").set(m.b) |
                   required("-c").set(m.c) |
                   required("-d").set(m.d);

        auto res = parse(args, cli);
        m.missing = res.missing().size();
        m.conflict = res.any_conflict();

        if(!(m == matches)) {
            throw std::runtime_error{"failed " + std::string( __FILE__ ) +
                                     " #1 in line " + std::to_string(lineNo)};
        }
    }
    {
        test_active m;

        auto cli = (
            option("?????"),
            (
               required("-a").set(m.a) |
               required("-b").set(m.b) |
               required("-c").set(m.c) |
               required("-d").set(m.d)
            )
        );

        auto res = parse(args, cli);
        m.missing = res.missing().size();
        m.conflict = res.any_conflict();

        if(!(m == matches)) {
            throw std::runtime_error{"failed " + std::string( __FILE__ ) +
                                     " #2 in line " + std::to_string(lineNo)};
        }
    }
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {
        {
            test_active e; e.missing  = 4;
            test(__LINE__, {""}, e);
            test(__LINE__, {"x"}, e);
            test(__LINE__, {"-x"}, e);
            test(__LINE__, {"x", "-y"}, e);
        }

        //parsing should catch all occurrences
        test(__LINE__, {"-a"}, test_active{1,0,0,0});
        test(__LINE__, {"-b"}, test_active{0,1,0,0});
        test(__LINE__, {"-c"}, test_active{0,0,1,0});
        test(__LINE__, {"-d"}, test_active{0,0,0,1});

        {
            test_active e; e.conflict = true;
            test(__LINE__, {"-a","-b"}, e);
            test(__LINE__, {"-b","-a"}, e);
            test(__LINE__, {"-b","-c"}, e);
            test(__LINE__, {"-c","-b"}, e);
            test(__LINE__, {"-a","-d"}, e);
            test(__LINE__, {"-d","-a"}, e);
            test(__LINE__, {"-d","-a","-c"}, e);
            test(__LINE__, {"-c","-b","-a"}, e);
            test(__LINE__, {"-a","-b","-c", "-d"}, e);
            test(__LINE__, {"-c","-a","-d", "-b"}, e);
        }
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
