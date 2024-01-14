/**
 * @file fn-test_suite.hpp
 * @author Jun-ichi Nishikata
 */

#ifndef WSP_TEST_FUNCTION_TEST_SUITE_HPP_
#define WSP_TEST_FUNCTION_TEST_SUITE_HPP_

#include <vector>
#include <string>

#include <wsp/common/tpl_cl-function_lane.hpp>

extern "C"{
#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
}

#define MAX_LENGTH_OF_TEST_NAME 64

namespace wsp{

    /**
     * @brief Test class which is implemented by module test class.
     */
    template<class _TestClass>
    class TestLane
        : wsp::FunctionLane<_TestClass>
    {
    public:
        TestLane(_TestClass &handle);

        /** @brief  Run test */
        void Run(int argc, char *argv[]);

    protected:
        /* Getter */
        int  argc( void ) const;
        void argv( char **o_argv ) const;
        char** argv();
        char** GetArgvPtr();
        void GetArgs( int *o_argc, char **o_argv ) const;

        /** @brief  Add test function */
        WSP_Result Push( const char *test_name, LaneFuncPtr lane_func_ptr );

        /** @brief  Assert any condition, save error info if assertion failed */
        void Assert( 
            bool condition, 
            int line, 
            const char* assert_str,
            const char* assert_file,
            const char* assert_func
        );

    private:
        typedef struct{
            std::string assert_str;
            std::string assert_file;
            std::string assert_func;
            int line;
        } ErrorInfo;

        typedef struct{
            char test_name[MAX_LENGTH_OF_TEST_NAME];
            std::vector<ErrorInfo> errinfo_list;
            WSP_Result             test_result;
        } TestInfo;

    private:
        int argc_;
        char **argv_;

        TestInfo test_info_list_[MAX_NUM_OF_FUNC_LANE];
        u32 current_test_index_;
        u32 current_error_index_;
    };
}

#define WSP_TESTLANE_ADD_TEST( test_func ) Push(#test_func, &test_func);

#define WSP_TESTLANE_RETURN_IF_ASSERT_FAILED( condition )\
    if(condition==false)\
    {\
        WSP_TESTLANE_ASSERT( condition );\
        return ;\
    }

#define WSP_TESTLANE_RETURN_IF_ASSERT_RESULT_FAILED( result )\
    do{\
        WSP_Result WSP_TESTLANE_RETURN_IF_ASSERT_RESULT_FAILED_result = result;\
        if(WSP_TESTLANE_RETURN_IF_ASSERT_RESULT_FAILED_result.IsFailure())\
        {\
            Assert( WSP_TESTLANE_RETURN_IF_ASSERT_RESULT_FAILED_result.IsSuccess(), __LINE__, #result, __FILE__, __FUNCTION__ );\
            WSP_Printf("    Result = %s ", WSP_TESTLANE_RETURN_IF_ASSERT_RESULT_FAILED_result.result_string);\
            return ;\
        }\
    }while(0)

#define WSP_TESTLANE_ASSERT( condition )\
    Assert( condition, __LINE__, #condition, __FILE__, __FUNCTION__ )

#define WSP_TEST_SUITE_MAIN( test_name ) \
    int main( int argc, char *argv[])    \
    {                                    \
        test_name test_suite;            \
        return (0);                      \
    }

#include "tpl_fn-test_lane.inl"

#endif

