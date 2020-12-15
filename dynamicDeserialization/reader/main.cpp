#include <iostream>
#include <dlfcn.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "makePlugin.h"
#include "runPlugin.h"
#include "readerStruct.h"

using namespace std;

int main(int argc, char** argv) {

  // 1.Build the plugin for each WriterStruct based on /data#/meta.json
  string meta_file_path = "../data1/meta.json";   // Use data1 WriterStruct for sample testing 
  string dll_path = make_plugin(meta_file_path);
  std::cout << "dll_path: " << dll_path << endl;

  // 2.Load WriterStruct data into ReaderStruct using the DLL
  // string dll_path = "../lib/lib_template.so";
  ReaderStruct * rs = run_plugin(dll_path.c_str());  

  // Print the ReaderStruct
  std::cout << "\nPrint ReaderStruct:" << endl;
  for (auto s : ReaderStruct::ListFields()) {
    std::cout << s << endl;
  } 
  std::cout << "foo: " << rs->foo <<endl;
  std::cout << "bar: " << rs->bar <<endl;

  return 0;
}