##课堂问题：Bakery算法若去除choosing会如何
没有choosing时，假设此时存在两个进程pid = 0、1

* T0 在计算出max+1时被打断，此时尚未完成赋值num[0]，切换到 T1 执行， T1 可以通过所有判断进入临界区。
* T1 进入临界区后被打断，T0 继续执行，因为 0 < 1 ，T0 也可以通过所有判断然后进入临界区。
* 去除之后无法实现忙则等待，所以不能去掉choosing，且在相应的number未完成赋值之前需要进行等待。

##小组讨论题
2.（spoc)了解race condition. 进入race-condition代码目录。

* 执行 `./x86.py -p loop.s -t 1 -i 100 -R dx`， 请问dx的值是什么？

	`-1`
* 执行 `./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx` ， 请问dx的值是什么？

	`-1`
* 执行 `./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`， 请问dx的值是什么？

	`-1`
* 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 1 -M 2000`, 请问变量x的值是什么？

	`1`
* 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`, 请问变量x的值是什么？为何每个线程要循环3次？

	`6`
因为bx寄存器初始值为3，每次循环减少1直到为0，所以衣柜循环3次。
* 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`， 请问变量x的值是什么？

	`2`
* 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1`， 请问变量x的值是什么？

	`2`
* 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2`， 请问变量x的值是什么？

	`2`
* 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`， 请问变量x的值是什么

	`1`