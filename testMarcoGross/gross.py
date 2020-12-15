MAX_FIELDS = 8
print("#define COUNT_ARGS(...) __COUNT_ARGS(__VA_ARGS__, {})".format(", ".join(map(str, range(MAX_FIELDS*2, 0, -1)))))
print("#define __COUNT_ARGS({}, N, ...) N".format(", ".join(map(lambda i: "N" + str(i), range(1, MAX_FIELDS*2+1)))))
print("#define FOREACHPAIR2(F, A, B) F(A, B)")
for i in range(2, MAX_FIELDS + 1):
  print("#define FOREACHPAIR{}(F, A, B, ...) F(A, B) FOREACHPAIR{}(F, __VA_ARGS__)".format(i * 2, (i - 1) * 2))

