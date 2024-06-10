# RCU read-copy-update vs mutex naive benchmark

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