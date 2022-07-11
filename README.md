# qos_test
## 简介
主要模拟了一个绘帧app启动后在不断在前后台切换的场景。

具体逻辑看`concurrency_test.c`下的`test_multithread_app`。

```c
static void test_multithread_app() {
	int times = 10;
	setuid(SYSTEM_UID);

	on_app_start();

	while (times--) {
		app_swap_to_backgroud(TEST_UID);
		usleep(FG_BG_SWAP_TIME);
		app_swap_to_front(TEST_UID);
	}

	on_app_dead();
}
```

## 编译
```
make arm64
```

## 执行
把编译生成的`qos_test`推上去执行即可

执行后有如下显示则为成功

![edad6ec76e7c62dacdceebf5c7bd224](https://user-images.githubusercontent.com/31315527/178429551-a5b6b3e8-a669-4995-b55e-6f8a1da74c74.png)
