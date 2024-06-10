# RCU read-copy-update vs mutex naive benchmark

RCU allows concurrent reads and updates by maintaining multiple versions of objects, ensuring that updates do not interfere with ongoing reads. The key components of RCU are the publish-subscribe mechanism for insertion, waiting for existing RCU readers to complete before deletion, and maintaining multiple versions of updated objects to avoid disrupting readers. This mechanism provides scalability and performance benefits, especially in multi-threaded environments.

# RCU

https://lwn.net/Articles/262464/

```
Read operations: 70603
Write operations: 1780
Total read time: 89571895 microseconds
Total write time: 12829 microseconds
Average read time: 1268 microseconds
Average write time: 7 microseconds
```

# Mutex

```
Read operations: 7895
Write operations: 4
Total read time: 80105498 microseconds
Total write time: 20020762 microseconds
Average read time: 10146 microseconds
Average write time: 5005190 microseconds
```