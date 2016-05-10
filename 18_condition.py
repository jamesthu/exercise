#coding=utf-8
#!/usr/bin/env python

import threading
import time
import random

M = 10
N = 20

condition = threading.Condition()
student = 0
computer = 2 * M
finish = 0
test = [0] * N
enter = 0
number = -1
finish_number = 0


class Student(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.number = 0

    def run(self):
        global student, enter, condition, number, finish, finish_number, computer
        if condition.acquire():
            while 1:
                if student == 0:
                    student = 1
                    condition.notify()
                    break
                else:
                    condition.wait()
            print 1
            while 1:
                if enter == 1:
                    enter = 0
                    self.number = number
                    print 'One student of group %d enters' %(self.number + 1)
                    condition.notify()
                    break
                else:
                    condition.wait()
            print 111
            time.sleep(random.randrange(1, 4))
            while 1:
                if finish == 0:
                    finish = 1
                    finish_number = self.number
                    print 'One student of group %d finishes' % (self.number)
                    condition.notify()
                    break
                else:
                    condition.wait()
            while 1:
                if test[self.number] == 1:
                    computer += 1
                    condition.notify()
                    break
                else:
                    condition.wait()

            condition.release()


class Teacher(threading.Thread):
    def __init__(self):
        global N
        threading.Thread.__init__(self)
        self.finish_list = [0] * N

    def run(self):
        global finish, finish_number, condition, test
        while True:
            if condition.acquire():
                if finish == 1:
                    self.finish_list[finish_number] += 1
                    if self.finish_list[finish_number] == 2:
                        test[finish_number] = 1
                        print 'Tests the work of students of group %d!' %(finish_number)
                    condition.notify()
                else:
                    condition.wait()
                condition.release()


class Guard(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        global enter, student, condition, number, computer
        while True:
            if condition.acquire():
                while 1:
                    if student == 1:
                        student = 0
                        condition.notify()
                        break
                    else:
                        condition.wait()
                print 2
                while 1:
                    if student == 1:
                        student = 0
                        condition.notify()
                        break
                    else:
                        condition.wait()
                print 3
                while 1:
                    if computer >= 2:
                        computer -= 2
                        number += 1
                        enter = 1
                        condition.notify()
                        break
                    else:
                        condition.wait()
                while 1:
                    if enter == 0:
                        enter = 1
                        condition.notify()
                        break
                    else:
                        condition.wait()
                while 1:
                    if enter != 0:
                        condition.wait()
                    break
                condition.release()


if __name__ == '__main__':
    for s in range(0, 2 * N):
        s = Student()
        s.start()

    t = Teacher()
    t.start()

    g = Guard()
    g.start()



