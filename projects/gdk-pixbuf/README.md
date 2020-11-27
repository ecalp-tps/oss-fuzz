- You can find the fuzz targets under [targets](./targets) and the
  Gdk-Pixbuf MR with these fuzzers
  [here](https://gitlab.gnome.org/GNOME/gdk-pixbuf/-/merge_requests/84). You can
  also find the discussion I had with the upstream in the MR. There was an issue
  with the CI pipeline that we could not fix, which is why the MR has not been
  merged yet. We will probably need help from upstream to solve the CI problem.

- I left out MSan for now since it fails the
  [build](https://github.com/google/oss-fuzz/runs/1448949557#step:7:7447).
