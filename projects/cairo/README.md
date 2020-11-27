- You can find the fuzz targets under [targets](./targets) and the
  Cairo MR with these fuzzers
  [here](https://gitlab.freedesktop.org/cairo/cairo/-/merge_requests/69).

- I initially left out UBSan - as Abhishek suggested
  [here](https://github.com/google/oss-fuzz/pull/4703#issuecomment-734070232) -
  because [pdf_surface_fuzzer](./targets/pdf_surface_fuzzer.c) crashed with a
  `null-pointer derefence` run-time error in build checks. However, I re-enabled
  it in this branch since we think that this error might be due to a bug in
  Cairo.
