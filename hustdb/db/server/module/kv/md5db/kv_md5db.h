#ifndef _kv_md5db_h_
#define _kv_md5db_h_

#include "../../../include/i_server_kv.h"
#include "../../perf_target.h"

namespace md5db
{
    class bucket_data_item_t;
    class bucket_t;
    class block_id_t;
}

using md5db::bucket_t;

class kv_md5db_t : public i_server_kv_t
{
public:

    virtual void kill_me ( );

    virtual bool open ( );

    virtual void info (
                        std::stringstream & ss
                        );

    virtual int get_user_file_count ( );

    bool ok ( )
    {
        return m_ok;
    }

    virtual int flush ( );

    virtual int del (
                      const char * user_key,
                      size_t user_key_len,
                      uint32_t & version,
                      bool is_dup,
                      conn_ctxt_t conn,
                      item_ctxt_t * & ctxt
                      );

    virtual int put (
                      const char * user_key,
                      size_t use_key_len,
                      const char * val,
                      size_t val_len,
                      uint32_t & version,
                      bool is_dup,
                      conn_ctxt_t conn,
                      item_ctxt_t * & ctxt
                      );

    virtual int get (
                      const char * user_key,
                      size_t user_key_len,
                      uint32_t & version,
                      conn_ctxt_t conn,
                      std::string * & rsp,
                      item_ctxt_t * & ctxt
                      );

    virtual int exists (
                         const char * user_key,
                         size_t user_key_len,
                         uint32_t & version,
                         conn_ctxt_t conn,
                         item_ctxt_t * & ctxt
                         );

    virtual void hash (
                        const char * user_key,
                        size_t user_key_len,
                        conn_ctxt_t conn,
                        item_ctxt_t * & ctxt
                        );

    virtual int export_db (
                            int file_id,
                            const char * path,
                            export_record_callback_t callback,
                            void * callback_param
                            );

    virtual int export_db_mem (
                                conn_ctxt_t conn,
                                std::string * & rsp,
                                item_ctxt_t * & ctxt,
                                export_record_callback_t callback = NULL,
                                void * callback_param = NULL
                                );

    virtual int hash_info (
                            int user_file_id,
                            int & inner_file_id
                            );

    virtual i_kv_t * get_conflict (
                                    int file_id
                                    );

    virtual void set_inner_ttl (
                                 uint32_t ttl,
                                 conn_ctxt_t conn
                                 );

    virtual uint32_t get_inner_ttl (
                                     conn_ctxt_t conn
                                     );

    virtual void set_inner_table (
                                   const char * table,
                                   size_t table_len,
                                   uint8_t type,
                                   conn_ctxt_t conn,
                                   const char * added = NULL
                                   );

    virtual const char * get_inner_tbkey (
                                           const char * key,
                                           size_t & key_len,
                                           conn_ctxt_t conn
                                           );

    uint32_t find_version_by_key (
                                   const char * key,
                                   size_t key_len,
                                   const char * table,
                                   size_t table_len
                                   );

public:

    kv_md5db_t ( );
    ~kv_md5db_t ( );

    void destroy ( );

    static void * operator new(
                                size_t size
                                )
    {
        void * p = malloc ( size );
        if ( NULL == p )
        {
            throw std::bad_alloc ( );
        }
        return p;
    }

    static void operator delete(
                                 void * p
                                 )
    {
        free ( p );
    }

private:

    void user_key_to_inner (
                             void * inner_key,
                             const void * user_key,
                             unsigned int user_key_len
                             );

    bool check_user_key (
                          const void * user_key,
                          unsigned int user_key_len
                          );

    bool check_inner_key (
                           const void * inner_key,
                           unsigned int inner_key_len
                           );

    int delete_direct_data (
                             bucket_t & bucket,
                             const md5db::block_id_t & block_id,
                             conn_ctxt_t conn,
                             item_ctxt_t * & ctxt
                             );

    int delete_conflict_data (
                               bucket_t & bucket,
                               md5db::bucket_data_item_t * item,
                               const void * inner_key,
                               size_t inner_key_len,
                               uint32_t & version,
                               conn_ctxt_t conn,
                               item_ctxt_t * & ctxt,
                               bool from_binlog
                               );

    int del_conflict_block_id (
                                bucket_t & bucket,
                                md5db::bucket_data_item_t * item,
                                const void * inner_key,
                                size_t inner_key_len,
                                item_ctxt_t * & ctxt,
                                md5db::block_id_t & deleted_block_id,
                                uint32_t & deleted_version,
                                bool from_binlog
                                );

    int get_data (
                   const md5db::block_id_t & block_id,
                   const char * user_key,
                   size_t user_key_len,
                   conn_ctxt_t conn,
                   std::string * & rsp,
                   item_ctxt_t * & ctxt
                   );

    int get_data_without_content_db (
                                      const md5db::block_id_t & block_id,
                                      const char * user_key,
                                      size_t user_key_len,
                                      conn_ctxt_t conn,
                                      std::string * & rsp,
                                      item_ctxt_t * & ctxt
                                      );

    int get_data_with_content_db (
                                   const md5db::block_id_t & block_id,
                                   const char * user_key,
                                   size_t user_key_len,
                                   conn_ctxt_t conn,
                                   std::string * & rsp,
                                   item_ctxt_t * & ctxt
                                   );

    int get_conflict_block_id (
                                const void * key,
                                size_t key_len,
                                md5db::block_id_t & block_id,
                                uint32_t & version
                                );

    int add_conflict_data (
                            bucket_t & bucket,
                            md5db::bucket_data_item_t * item,
                            const char * inner_key,
                            size_t inner_key_len,
                            const char * user_key,
                            size_t user_key_len,
                            const char * val,
                            size_t val_len,
                            uint32_t & version,
                            conn_ctxt_t conn,
                            item_ctxt_t * & ctxt,
                            bool from_binlog
                            );

    int set_conflict_data (
                            bucket_t & bucket,
                            md5db::bucket_data_item_t * item,
                            const char * inner_key,
                            size_t inner_key_len,
                            const char * user_key,
                            size_t user_key_len,
                            const char * val,
                            size_t val_len,
                            uint32_t & version,
                            bool & is_found,
                            conn_ctxt_t conn,
                            item_ctxt_t * & ctxt,
                            bool from_binlog
                            );

    int set_data (
                   const md5db::block_id_t & block_id,
                   const char * user_key,
                   size_t user_key_len,
                   const char * val,
                   size_t val_len,
                   conn_ctxt_t conn,
                   item_ctxt_t * & ctxt
                   );

    int set_data_with_kv_array (
                                 const md5db::block_id_t & block_id,
                                 const char * user_key,
                                 size_t user_key_len,
                                 const char * val,
                                 size_t val_len,
                                 conn_ctxt_t conn,
                                 item_ctxt_t * & ctxt
                                 );

    int set_data_with_content_db (
                                   const md5db::block_id_t & block_id,
                                   const char * user_key,
                                   size_t user_key_len,
                                   const char * val,
                                   size_t val_len,
                                   conn_ctxt_t conn,
                                   item_ctxt_t * & ctxt
                                   );

    int add_data_with_content_db (
                                   const md5db::block_id_t & block_id,
                                   const char * user_key,
                                   size_t user_key_len,
                                   const char * val,
                                   size_t val_len,
                                   conn_ctxt_t conn,
                                   item_ctxt_t * & ctxt
                                   );

    int new_data (
                   bucket_t & bucket,
                   md5db::bucket_data_item_t * item,
                   const char * inner_key,
                   size_t inner_key_len,
                   const char * user_key,
                   size_t user_key_len,
                   const char * val,
                   size_t val_len,
                   uint32_t & version,
                   conn_ctxt_t conn,
                   item_ctxt_t * & ctxt,
                   bool from_binlog
                   );

    int check_same_key (
                         bucket_t & bucket,
                         const md5db::block_id_t & block_id,
                         const char * inner_key,
                         size_t inner_key_len,
                         const char * user_key,
                         size_t user_key_len
                         );

    int put_inner (
                    const char * user_key,
                    size_t user_key_len,
                    const char * val,
                    size_t val_len,
                    uint32_t & version,
                    conn_ctxt_t conn,
                    item_ctxt_t * & ctxt,
                    bool from_binlog
                    );

    int del_inner (
                    const char * user_key,
                    size_t user_key_len,
                    uint32_t & version,
                    conn_ctxt_t conn,
                    item_ctxt_t * & ctxt,
                    bool from_binlog
                    );

    uint32_t add_version (
                           uint32_t & version
                           );

private:

    struct inner;

    inner * m_inner;
    bool m_ok;

    perf_target_t m_perf_put_ok;
    perf_target_t m_perf_put_fail;
    perf_target_t m_perf_binlog_put_ok;
    perf_target_t m_perf_binlog_put_fail;
    size_t m_count_put_wrong_version;
    size_t m_count_binlog_put_wrong_version;

    perf_target_t m_perf_del_ok;
    perf_target_t m_perf_del_not_found;
    perf_target_t m_perf_del_fail;
    perf_target_t m_perf_binlog_del_ok;
    perf_target_t m_perf_binlog_del_not_found;
    perf_target_t m_perf_binlog_del_fail;
    size_t m_count_del_wrong_version;
    size_t m_count_binlog_del_wrong_version;

    perf_target_t m_perf_get_ok;
    perf_target_t m_perf_get_not_found;
    perf_target_t m_perf_get_fail;

    perf_target_t m_perf_conflict_get;
    perf_target_t m_perf_conflict_put;
    perf_target_t m_perf_conflict_del;
    perf_target_t m_perf_fast_conflict_get;
    perf_target_t m_perf_fast_conflict_put;
    perf_target_t m_perf_fast_conflict_del;

    perf_target_t m_perf_fullkey_get;
    perf_target_t m_perf_fullkey_write;
    perf_target_t m_perf_fullkey_del;

    perf_target_t m_perf_content_get;
    perf_target_t m_perf_content_write;
    perf_target_t m_perf_content_update;
    perf_target_t m_perf_content_del;

    perf_target_t m_perf_data_get;
    perf_target_t m_perf_data_put;
    perf_target_t m_perf_data_del;

    size_t m_count_conflicts[ 1000 ];

public:

    inner * get_inner ( )
    {
        return m_inner;
    }

private:
    // disable
    kv_md5db_t ( const kv_md5db_t & );
    const kv_md5db_t & operator= ( const kv_md5db_t & );
};

#endif
