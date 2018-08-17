#ifndef STORAGE_LEVELDB_INCLUDE_BTREE_INDEX_H_
#define STORAGE_LEVELDB_INCLUDE_BTREE_INDEX_H_

#include <cstdint>
#include <map>
#include <deque>
#include <shared_mutex>
#include "leveldb/env.h"
#include "leveldb/iterator.h"
#include "leveldb/options.h"
#include "leveldb/index.h"
#include "db/version_edit.h"
#include "index/ff_btree.h"
#include "port/port.h"
#include "db/table_cache.h"

namespace leveldb {

class BtreeIndex : public Index{
public:
  BtreeIndex();

  ~BtreeIndex() = default;

  IndexMeta Get(const Slice& key);

  void Insert(const uint32_t& key, IndexMeta meta);

  void AddQueue(std::deque<KeyAndMeta>& queue, VersionEdit* edit);

  Iterator* NewIterator(const ReadOptions& options, TableCache* table_cache);

  FFBtreeIterator* BtreeIterator();

private:
  void Runner();
  static void* ThreadWrapper(void* ptr);

  FFBtree tree_; // Temporary
  bool bgstarted_;
  pthread_t thread_;
  port::Mutex mutex_;
  port::CondVar condvar_;

  std::deque<KeyAndMeta> queue_;
  VersionEdit* edit_;

  BtreeIndex(const BtreeIndex&);
  void operator=(const BtreeIndex&);
};

} // namespace leveldb

#endif // STORAGE_LEVELDB_INCLUDE_BTREE_INDEX_H_