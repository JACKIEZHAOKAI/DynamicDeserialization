// Helper functions to break ReaderStruct/WriterStruct user input fields
// used by reader/readerStruct.h and lib/lib#.cpp  
#define COUNT_ARGS(...) __COUNT_ARGS(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define __COUNT_ARGS(N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11, N12, N13, N14, N15, N16, N, ...) N
#define FOREACHPAIR2(F, A, B) F(A, B)
#define FOREACHPAIR4(F, A, B, ...) F(A, B) FOREACHPAIR2(F, __VA_ARGS__)
#define FOREACHPAIR6(F, A, B, ...) F(A, B) FOREACHPAIR4(F, __VA_ARGS__)
#define FOREACHPAIR8(F, A, B, ...) F(A, B) FOREACHPAIR6(F, __VA_ARGS__)
#define FOREACHPAIR10(F, A, B, ...) F(A, B) FOREACHPAIR8(F, __VA_ARGS__)
#define FOREACHPAIR12(F, A, B, ...) F(A, B) FOREACHPAIR10(F, __VA_ARGS__)
#define FOREACHPAIR14(F, A, B, ...) F(A, B) FOREACHPAIR12(F, __VA_ARGS__)
#define FOREACHPAIR16(F, A, B, ...) F(A, B) FOREACHPAIR14(F, __VA_ARGS__)