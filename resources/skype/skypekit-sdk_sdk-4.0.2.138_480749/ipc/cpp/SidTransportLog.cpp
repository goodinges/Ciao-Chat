#include <SidTransportLog.hpp>
#include <assert.h>
#include <string.h>

// a lot more could be done: compression of files, an index file for all writes date and location (thread)
// currently only a rotating log with 2 files for both input and output and only the bytes.

namespace Sid {

class TransportLog::Finalizer {
public:
        static Finalizer* instance() { return &M_instance; }
        void add(TransportLog* log)  { 
                assert(M_checker);
                log->m_next = m_logs;
                m_logs = log;
        }
        void del(TransportLog* log)  { 
//printf("deleting log\n");
                TransportLog* l = m_logs, *p = 0;
                while (l != log && l) {
                        p = l;
                        l = l->m_next;
                }
                if (!p) 
                        m_logs = log->m_next;
                else
                        p->m_next = log->m_next;
        }
private:
        Finalizer() { m_logs = 0; M_checker = this; }
        ~Finalizer() {
                while (m_logs) delete m_logs;
        }
        TransportLog* m_logs;
        static Finalizer* M_checker;
        static Finalizer M_instance;
};

TransportLog::Finalizer* TransportLog::Finalizer::M_checker = 0;
TransportLog::Finalizer TransportLog::Finalizer::M_instance;

static char* make_name(const char* base, const char* suffix) {
  int lenbase = strlen(base), lensuffix = strlen(suffix);
  char* name = new char[lenbase+lensuffix+1];
  strcpy(name, base);
  return strcat(name, suffix);
}

TransportLog::TransportLog(const char* dest_file_names)
{
        m_in_idx  = 0;
        m_out_idx = 0;
        m_filenames = new char*[4];
        m_filenames[0] = make_name(dest_file_names,"_log_in.1");
        m_filenames[1] = make_name(dest_file_names,"_log_in.2");
        m_filenames[2] = make_name(dest_file_names,"_log_out.1");
        m_filenames[3] = make_name(dest_file_names,"_log_out.2");
        m_in_file  = fopen(m_filenames[0], "wb");
        if (m_in_file) {
                m_out_file = fopen(m_filenames[2], "wb");
                if (!m_out_file) {
                        fclose(m_in_file);
                        m_in_file = 0;
                }
        }
        Finalizer::instance()->add(this); 
        if (!m_in_file) fprintf(stderr, "ERR: Can't write transport log %s\n", dest_file_names);
}

TransportLog::~TransportLog()
{
        if (m_in_file) {
                fclose(m_in_file);
                m_in_file = 0;
        }
        if (m_out_file) {
                fclose(m_out_file);
                m_out_file = 0;
        }
        Finalizer::instance()->del(this);  
}

static void store(const CommandInitiator*, const uint& num_bytes, const char* buf, FILE*& file, int& idx,  char** filename) {
        if (!file) return;
        size_t written = fwrite(buf, 1, num_bytes, file);
        if (written < num_bytes) {
                fclose(file);
                idx = 1 - idx;
                file = fopen(filename[idx], "wb");
                if (!file) return;
                written = fwrite(buf, 1, num_bytes - written, file);
                if (written < num_bytes) {
                        fclose(file);
                        file = 0;
                } else fflush(file);
        } else fflush(file);
}

void TransportLog::read(const CommandInitiator* curr, const uint& num_bytes, const char* buf)
{
        store(curr, num_bytes, buf, m_in_file, m_in_idx, &m_filenames[0]);
}

void TransportLog::skip_read(const CommandInitiator* curr, const uint& num_bytes, const char* buf)
{
        store(curr, num_bytes, buf, m_in_file, m_in_idx, &m_filenames[0]);
}

void TransportLog::skip_read_final(const CommandInitiator* curr, const uint& num_bytes, const char* buf)
{
        store(curr, num_bytes, buf, m_in_file, m_in_idx, &m_filenames[0]);
}

void TransportLog::write(const CommandInitiator* curr, const uint& num_bytes, const char* buf)
{
        store(curr, num_bytes, buf, m_out_file, m_out_idx, &m_filenames[2]);
}

}

