#include <stdio.h>
#include <SidPlatform.hpp>
#include "SidCommonConfig.hpp"

namespace Sid {

        class CommandInitiator;

        class TransportLog {
        public:
                TransportLog(const char* dest_file_names = 0);
                ~TransportLog();
                void read(const CommandInitiator* curr, const uint& num_bytes, const char* buf);
                void skip_read(const CommandInitiator* curr, const uint& num_bytes, const char* buf);
                void skip_read_final(const CommandInitiator* curr, const uint& num_bytes, const char* buf);
                void write(const CommandInitiator* curr, const uint& num_bytes, const char* buf);
        private:
                FILE*         m_out_file;
                FILE*         m_in_file;
                int           m_in_idx;
                int           m_out_idx;
                TransportLog* m_next;
                char**        m_filenames;
                class  Finalizer;
                friend class Finalizer;
                SKYPEKIT_DISABLE_COPY(TransportLog);
       };

}

