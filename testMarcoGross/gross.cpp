#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "gross.h"

// for-each, two arg macro
#define FOREACHPAIR(F, ...) __TO_FOREACHPAIRN(COUNT_ARGS(__VA_ARGS__))(F, __VA_ARGS__)
#define __TO_FOREACHPAIRN(N) __CONCAT_FOREACHPAIR_N(N)
#define __CONCAT_FOREACHPAIR_N(N) FOREACHPAIR ## N

#define STRUCT(S, ...) \
struct S { \
  STRUCT_FIELDS(__VA_ARGS__) \
  STRUCT_LIST_FIELDS(__VA_ARGS__) \
  STRUCT_SET_FIELD(__VA_ARGS__) \
}

// STRUCT_FIELDS
#define STRUCT_FIELDS_ONE(A, B) A B;
#define STRUCT_FIELDS(...) FOREACHPAIR(STRUCT_FIELDS_ONE, __VA_ARGS__)

// STRUCT_LIST_FIELDS
#define STRUCT_LIST_FIELDS_PUSH_BACK_ONE(A, B) fields.push_back(#B);
#define STRUCT_LIST_FIELDS_PUSH_BACK(...) FOREACHPAIR(STRUCT_LIST_FIELDS_PUSH_BACK_ONE, __VA_ARGS__)

#define STRUCT_LIST_FIELDS(...) \
static vector<string> ListFields() { \
  vector<string> fields; \
  STRUCT_LIST_FIELDS_PUSH_BACK(__VA_ARGS__); \
  return fields; \
}

// STRUCT_SET_FIELD
#define STRUCT_SET_FIELD_COMPARE_AND_SET_ONE(A, B) \
if (field.compare(#B) == 0) { \
  this->B = *(static_cast<A*>(value)); \
  return; \
}

#define STRUCT_SET_FIELD_COMPARE_AND_SET(...) FOREACHPAIR(STRUCT_SET_FIELD_COMPARE_AND_SET_ONE, __VA_ARGS__)
 
#define STRUCT_SET_FIELD(...) \
void SetField(string field, void * value) { \
  STRUCT_SET_FIELD_COMPARE_AND_SET(__VA_ARGS__); \
  throw runtime_error("Field `" + field + "` does not exist!"); \
}

STRUCT(
  MyStruct,
  int, foo,
  string, bar,
  float, field3,
  vector<int>, field4,
  char, field5,
  char, field6,
  char, field7,
  char, field8
  // char, field9 <-- error (even with comma after field8)
);

int main() {
  for (auto s : MyStruct::ListFields()) {
    cout << s << endl;
  }
  int n = 128;
  vector<int> f = {1, 2, 3};
  MyStruct mys;
  mys.SetField("foo", &n);
  mys.SetField("field4", &f);
  cout << mys.foo << endl;
  cout << mys.field4[1] << endl;
  return 0;
}
