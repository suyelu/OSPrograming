## 功能描述
- 两个独立的.c文件：server.c 和 client.c
- 使用共享内存，pthread_mutex 和 pthread_cond
- 服务端，初始化共享内存及同步原语，阻塞接受消息
- 客户端，输入消息，并写入共享内存
- 需要传递的数据包括:name 和 msg
- name应该使用getopt获取

---

## 使用说明

```bash
 gcc server.c -o server -lpthread
 gcc client.c -o client -lpthread

 ./server 
 ./client -n Su 
```
