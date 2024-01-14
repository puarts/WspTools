
#include "GpuTimer.h"
#include <gl/glew.h>
#include <wsp/common/fn-debug.h>
#include <wsp/graphics/fn-graphic_log.h>
#include <wsp/opengl/GlProxy.h>

#define EXEC_GL_FUNC(gl_func) do { gl_func; GLenum gl_status = glGetError(); WSP_ASSERT(gl_status == GL_NO_ERROR, #gl_func " failed with %s", gluErrorString(gl_status)); } while(false)

using namespace wsp::opengl;

GpuTimer::GpuTimer()
    : query_back_buffer_(0)
    , query_front_buffer_(1)
{
}

GpuTimer::~GpuTimer() throw()
{
    DeleteAllQueries();
}

void GpuTimer::Start(const char* query_id)
{
    int query_index = GetIndex(query_id);
    if (query_index == -1)
    {
        query_index = static_cast<int>(query_infos_.size());
        AddQuery(query_id);
    }
    wsp::opengl::glproxy::BeginQuery(GL_TIME_ELAPSED, GetBackQueryLocation(query_index));
    query_infos_[query_index].is_queried[query_back_buffer_] = true;
}

void GpuTimer::Stop()
{
    wsp::opengl::glproxy::EndQuery(GL_TIME_ELAPSED);
}

void GpuTimer::SwapQueryBuffers()
{
    if (query_back_buffer_ == 1)
    {
        query_back_buffer_ = 0;
        query_front_buffer_ = 1;
    }
    else
    {
        query_back_buffer_ = 1;
        query_front_buffer_ = 0;
    }

    for (int query_index = 0, queryCount = GetQueryCount(); query_index < queryCount; ++query_index)
    {
        query_infos_[query_index].is_queried[query_back_buffer_] = false;
    }
}


void GpuTimer::ResetAllQueries()
{
    for (int query_index = 0, queryCount = GetQueryCount(); query_index < queryCount; ++query_index)
    {
        wsp::opengl::glproxy::BeginQuery(GL_TIME_ELAPSED, GetBackQueryLocation(query_index));
        wsp::opengl::glproxy::EndQuery(GL_TIME_ELAPSED);
    }
}

double GpuTimer::GetElapsedMilliseconds(int query_index)
{
    if (!query_infos_[query_index].is_queried[query_front_buffer_])
    {
        return 0.0;
    }

    GLuint64 elapsed_gpu_time;
    EXEC_GL_FUNC(glGetQueryObjectui64v(GetFrontQueryLocation(query_index), GL_QUERY_RESULT, &elapsed_gpu_time));

    return elapsed_gpu_time / 1000000.0;
}

void GpuTimer::AddQuery(const char* query_id)
{
    GLuint queries[QUERY_BUFFER_COUNT];
    EXEC_GL_FUNC(glGenQueries(2, queries));

    QueryInfo query_info;
    query_info.query_id = query_id;
    query_info.query_locations[query_front_buffer_] = queries[query_front_buffer_];
    query_info.query_locations[query_back_buffer_] = queries[query_back_buffer_];

    query_infos_.push_back(query_info);
}

void GpuTimer::DeleteAllQueries()
{
    GLsizei query_count = static_cast<GLsizei>(query_infos_.size());
    for (int query_index = 0; query_index < query_count; ++query_index)
    {
        glDeleteQueries(1, &query_infos_[query_index].query_locations[query_front_buffer_]);
        glDeleteQueries(1, &query_infos_[query_index].query_locations[query_back_buffer_]);
    }

    query_infos_.clear();
}
