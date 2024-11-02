# RCU read-copy-update vs mutex naive benchmark

[![C/C++ CI](https://github.com/nenuadrian/read-copy-update-mutex-benchmark/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/nenuadrian/read-copy-update-mutex-benchmark/actions/workflows/c-cpp.yml)

RCU allows concurrent reads and updates by maintaining multiple versions of objects, ensuring that updates do not interfere with ongoing reads. The key components of RCU are the publish-subscribe mechanism for insertion, waiting for existing RCU readers to complete before deletion, and maintaining multiple versions of updated objects to avoid disrupting readers. This mechanism provides scalability and performance benefits, especially in multi-threaded environments.

# RCU

https://lwn.net/Articles/262464/

```
Read operations: 66117
Write operations: 1769
Total read time: 83723688 microseconds
Total write time: 2384 microseconds
Average read time: 1266 microseconds
Average write time: 1 microseconds

real	0m10.976s
user	0m0.206s
sys	0m0.736s
```

# Mutex

```
Read operations: 7880
Write operations: 4
Total read time: 80124002 microseconds
Total write time: 20000420 microseconds
Average read time: 10168 microseconds
Average write time: 5000105 microseconds

real	0m10.289s
user	0m0.043s
sys	0m0.198s
```
