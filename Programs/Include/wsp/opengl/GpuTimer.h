#pragma once

#include <wsp/common/_define_commoncore.h>
#include <vector>
#include <string>

namespace wsp {
    namespace opengl {
        class WSP_DLL_EXPORT GpuTimer
        {
        public:
            GpuTimer();
            ~GpuTimer() throw();

            void Start(const char* query_id);

            void Stop();

            void ResetAllQueries();

            int GetQueryCount() const
            {
                return static_cast<int>(query_infos_.size());
            }

            double GetElapsedMilliseconds(int query_index);

            void SwapQueryBuffers();

            const char* GetQueryId(int query_index) const
            {
                return query_infos_[query_index].query_id.data();
            }

            void ClearQueries()
            {
                DeleteAllQueries();
            }

        private:
            int GetIndex(const char* query_id) const
            {
                int query_index = 0;
                for (std::vector<QueryInfo>::const_iterator iter = query_infos_.begin(), end = query_infos_.end(); iter != end; ++iter, ++query_index)
                {
                    const QueryInfo* query_info = &*iter;
                    if (query_info->query_id == query_id)
                    {
                        return query_index;
                    }
                }

                return -1;
            }

            void AddQuery(const char* query_id);

            void DeleteAllQueries();

            unsigned int GetFrontQueryLocation(int query_index) const
            {
                return query_infos_[query_index].query_locations[query_front_buffer_];
            }

            unsigned int GetBackQueryLocation(int query_index) const
            {
                return query_infos_[query_index].query_locations[query_back_buffer_];
            }

        private:
            static const int QUERY_BUFFER_COUNT = 2;
            struct QueryInfo
            {
                unsigned int query_locations[QUERY_BUFFER_COUNT];
                std::string query_id;
                bool is_queried[QUERY_BUFFER_COUNT];

                QueryInfo()
                {
                    is_queried[0] = false;
                    is_queried[1] = false;
                    memset(query_locations, 0, sizeof(query_locations));
                }
            };
            std::vector<QueryInfo> query_infos_;
            unsigned int query_back_buffer_;
            unsigned int query_front_buffer_;
        };
    }
}
