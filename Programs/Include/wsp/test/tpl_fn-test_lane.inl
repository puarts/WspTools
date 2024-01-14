/**
 * @file fn-test_suite.inl
 * @author Jun-ichi Nishikata
 */

#ifndef WSP_TEST_FUNCTION_TEST_LINE_INL_
#define WSP_TEST_FUNCTION_TEST_LINE_INL_

#include <wsp/test/fn-test_io.h>
#include <vector>
#include <string>
#include <wsp/common/tpl_cl-function_lane.hpp>

extern "C"{
#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#ifndef __WSP_COMMONCORE_COMMON_RESULT_H__
#include <wsp/common/common_result.h>
#endif
}
#include "tpl_fn-test_lane.hpp"

template<class _TestClass>
wsp::TestLane<_TestClass>::TestLane(_TestClass &handle)
    : wsp::FunctionLane<_TestClass>( handle )
    , current_test_index_(0)
    , current_error_index_(0)
    , argv_(NULL)
    , argc_(0)
{
}

template<class _TestClass>
int wsp::TestLane<_TestClass>::argc( void ) const
{
    return argc_;
}
template<class _TestClass>
char** wsp::TestLane<_TestClass>::argv()
{
    return argv_;
}

template<class _TestClass>
void wsp::TestLane<_TestClass>::argv( char **o_argv ) const
{
    char* const *src_ptr = argv_;
    char **dst_ptr = o_argv;
    char **end_ptr = dst_ptr + argc_;
    for( ; dst_ptr!=end_ptr; ++dst_ptr )
    {
        strcpy( *dst_ptr, *src_ptr );
    }
}

template<class _TestClass>
char** wsp::TestLane<_TestClass>::GetArgvPtr()
{
    return argv_;
}


template<class _TestClass>
void wsp::TestLane<_TestClass>::GetArgs( int *o_argc, char **o_argv ) const
{ 
    *o_argc = argc();
    argv( o_argv );
}

template<class _TestClass>
void wsp::TestLane<_TestClass>::Run(int argc, char *argv[])
{
    argc_ = argc;
    argv_ = argv;

    char *test_name;
    bool pass_all_tests = true;
    for( int i=0; i<num_of_funcs_; ++i )
    {
        (func_handle_ptr_->*func_ptr_list_[i])();
        TestInfo* testinfo = &test_info_list_[i];
        test_name = testinfo->test_name;
        current_error_index_ = 0;
        pass_all_tests = pass_all_tests && WSP_ResultInclude( test_info_list_[i].test_result, WSP_ResultSuccess() );

    }
    printf(
        "-----------------------------------------\n");
    for( int i=0; i<num_of_funcs_; ++i )
    {
        printf(
            "<testcase name=\"%s\" result=\"%s\">\n"
            , test_name
            , WSP_ResultInclude( test_info_list_[i].test_result, WSP_ResultSuccess() )? "PASS":"FAIL"
        );
        TestInfo* testinfo = &test_info_list_[i];
        int num_errs = testinfo->errinfo_list.size();
        for( int errindex=0; errindex<num_errs; ++errindex )
        {
            ErrorInfo* errinfo = &testinfo->errinfo_list[errindex];
            printf(
                "    <error location=\"%s, %s, line: %d \">%s</error>\n"
                , errinfo->assert_file.c_str()
                , errinfo->assert_func.c_str()
                , errinfo->line
                , errinfo->assert_str.c_str()
            );
        }
        printf(
            "</testcase>\n"
        );
    }
    printf(
        "Test Result: %s\n"
        "-----------------------------------------\n"
        , pass_all_tests? "PASS" : "FAIL"
    );

}

template<class _TestClass>
WSP_Result wsp::TestLane<_TestClass>::Push( const char *test_name, LaneFuncPtr lane_func_ptr )
{
    if(num_of_funcs_ >= MAX_NUM_OF_FUNC_LANE){
        WSP_COMMON_ERROR_LOG("Too many functions registered\n");
        return WSP_ResultResourceLeak();
    }
    if(strlen(test_name)>MAX_LENGTH_OF_TEST_NAME){
        WSP_COMMON_ERROR_LOG("Too long test name\n");
        return WSP_ResultInvalidParameter();
    }
    /* set up test info */
    strncpy(test_info_list_[num_of_funcs_].test_name, test_name, MAX_LENGTH_OF_TEST_NAME-1);
    test_info_list_[num_of_funcs_].test_result = WSP_ResultSuccess();

    /* Add test func to test func lane */
    func_ptr_list_[ num_of_funcs_] = lane_func_ptr;
    printf("Test %s added\n", test_info_list_[num_of_funcs_].test_name);


    ++num_of_funcs_;
    return WSP_ResultSuccess();
}

template<class _TestClass>
void wsp::TestLane<_TestClass>::Assert( 
    bool condition, 
    int line, 
    const char* assert_str,
    const char* assert_file, 
    const char* assert_func 
)
{
    if( condition == false )
    {
        WSP_Printf( 
            "Assertion failed - %s (%s, line:%d, %s)\n"
            , assert_str, assert_file, line, assert_func );
        test_info_list_[current_test_index_].test_result = WSP_ResultStatusError();
        std::vector<ErrorInfo>* errinfo_list = &test_info_list_[current_test_index_].errinfo_list;
        errinfo_list->resize( errinfo_list->size() + 1 );
        ErrorInfo* errinfo = &(*errinfo_list)[current_error_index_];
        errinfo->line = line;
        errinfo->assert_str = assert_str;
        errinfo->assert_file = assert_file;
        errinfo->assert_func = assert_func;
        ++current_error_index_;
    }
}

#endif
