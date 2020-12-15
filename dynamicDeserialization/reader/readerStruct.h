// This file allow the user to define the ReaderStruct at below
// POC #3 - Automatically insert methods in a struct definition that 
// are required by POC #2 using macros. Help simplify user code.
#ifndef READERSTRUCT_H
#define READERSTRUCT_H

#include "gross.h"
#include <string>
#include <vector>

// for-each, two arg macro
#define FOREACHPAIR(F, ...) __TO_FOREACHPAIRN(COUNT_ARGS(__VA_ARGS__))(F, __VA_ARGS__)
#define __TO_FOREACHPAIRN(N) __CONCAT_FOREACHPAIR_N(N)
#define __CONCAT_FOREACHPAIR_N(N) FOREACHPAIR ## N

// Definition of Marco STRUCT
#define STRUCT(S, ...) \
struct S { \
  STRUCT_FIELDS(__VA_ARGS__) \
  STRUCT_LIST_FIELDS(__VA_ARGS__) \
  STRUCT_SET_FIELD(__VA_ARGS__) \
}

// Marco function of STRUCT_FIELDS
#define STRUCT_FIELDS_ONE(A, B) A B;
#define STRUCT_FIELDS(...) FOREACHPAIR(STRUCT_FIELDS_ONE, __VA_ARGS__)

// Marco function of ListFields()
#define STRUCT_LIST_FIELDS_PUSH_BACK_ONE(A, B) \
fields.push_back(#B);

#define STRUCT_LIST_FIELDS_PUSH_BACK(...) \
FOREACHPAIR(STRUCT_LIST_FIELDS_PUSH_BACK_ONE, __VA_ARGS__)

#define STRUCT_LIST_FIELDS(...) \
static std::vector<std::string> ListFields() { \
  std::vector<std::string> fields; \
  STRUCT_LIST_FIELDS_PUSH_BACK(__VA_ARGS__); \
  return fields; \
}

// Marco function of SetField()
#define STRUCT_SET_FIELD_COMPARE_AND_SET_ONE(A, B) \
if (field.compare(#B) == 0) { \
  this->B = *(static_cast<A*>(value)); \
  return; \
}

#define STRUCT_SET_FIELD_COMPARE_AND_SET(...) \
FOREACHPAIR(STRUCT_SET_FIELD_COMPARE_AND_SET_ONE, __VA_ARGS__)

#define STRUCT_SET_FIELD(...) \
void SetField(std::string field, void * value) { \
  STRUCT_SET_FIELD_COMPARE_AND_SET(__VA_ARGS__); \
  throw runtime_error("Field `" + field + "` does not exist!"); \
}

// ############ user input of ReaderStruct ############
STRUCT(
  ReaderStruct,
  int, foo,
  std::string, bar
  // Max taking 8 fields
  // char, field9 <-- error (even with comma after field8)
);
//#####################################################

#endif