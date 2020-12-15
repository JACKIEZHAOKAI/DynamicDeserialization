// POC #1 - Load a different shared object at runtime dynamically based on some user input.
// POC #2 - Automatically transfer fields between two different structs at runtime 
// using a primitive reflection system (GetField, SetField, ListFields).
#include <iostream>
#include <dlfcn.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "readerStruct.h"

// define the JSON payload(a serialized object of the WriterStruct) for testing
#define JSON_PAYLOAD  "{\"foo\": 100, \"bar\": \"lib1 bar\", \"odd\": \"lib1 odd\"}"

using namespace std;

// load the symbol
typedef void * (*create_t)(const vector<uint8_t>&);
typedef void * (*get_field_t)(void *, string);
typedef void (*delete_t)(void *);

template<typename T>
T load_symbol(void* handle, const char*name) {
  T symbol;
  dlerror();
  symbol = (T) dlsym(handle, name);
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
      cerr << "Cannot load symbol '" << name << "': " << dlsym_error << '\n';
      dlclose(handle);
  }
  return symbol;
}

// Returns a ReaderStruct whose fields are populated using ListFields, GetField, 
// and SetField from the WriterStruct.
ReaderStruct* load_WriterStruct_data(void * ws, get_field_t GetField) { 
  cout << "\nLoading data from WriterStruct to ReaderStruct...\n";
  void * fieldPtr;
  ReaderStruct * obj = new ReaderStruct();

  cout << "Calling ListFields...\n";
  for (auto field : ReaderStruct::ListFields()) {
    cout << "Calling GetField and Setfield...\n";
    // Load WriterStruct data using GetField()
    // Fill data into the ReaderStruct using SetField()
    obj->SetField(field, GetField(ws, field));  
  } 
  return obj;
}

ReaderStruct* run_plugin(const char* path) {
  cout << "\nOpening the dynamic loaded library...\n";  
  void* handle = dlopen (path, RTLD_LAZY);
  if (!handle) {
      cerr << "Cannot open library: " << dlerror() << '\n';
      return NULL;
  }
  
  // load symbols
  create_t Create = load_symbol<create_t>(handle, "Create");
  get_field_t GetField = load_symbol<get_field_t>(handle, "GetField");
  delete_t Delete = load_symbol<delete_t>(handle, "Delete");

  string json_data = JSON_PAYLOAD;
  vector<uint8_t> payload(json_data.begin(), json_data.end());
  cout << "\nConverting the payload to a WriterStruct...\n";
  void * ws = Create(payload);

  ReaderStruct *rs = load_WriterStruct_data(ws, GetField);

  cout << "\nDeleting the WriterStruct...\n";
  Delete(ws);

  cout << "\nClosing library...\n";
  dlclose(handle);

  return rs;
}