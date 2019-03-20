
#include "resources.h"

#include "../core/log.h"

unsigned long hash(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *(str++)))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

TW_Resource *_TW_ResourceFindByHashId(TW_Resource *root, unsigned long hid) {
  TW_Resource *result = 0;
  LL_SEARCH_SCALAR(root, result, hash_id, hid);
  return result;
}

TW_Resource *TW_ResourceAdd(TW_Resource *root, const char *name,
                            TW_Resource *res) {
  unsigned long hash_id = hash(name);
  TW_Resource *result = _TW_ResourceFindByHashId(root, hash_id);
  if (result) {
    app_log("Hash collision!");
    return 0;
  }
  res->hash_id = hash_id;
  LL_APPEND(root, res);
  return root;
}

TW_Resource *TW_ResourceFind(TW_Resource *root, const char *name) {
  return _TW_ResourceFindByHashId(root, hash(name));
}