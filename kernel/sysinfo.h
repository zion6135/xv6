struct sysinfo {
  uint64 freemem;   // amount of free memory (bytes) == 遍历freeelist==>得到可用内存大小
  uint64 nproc;     // number of process  == myproc()->nextpid
};
