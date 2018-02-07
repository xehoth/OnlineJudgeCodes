# coding=UTF-8
# python2.7
# Copyright (c) 2017, xehoth
# All rights reserved.
# 「BZOJ 1002」轮状病毒 26-03-2017
# @author xehoth
n = (int)(input())
a = 5
b = 1
if n <= 2:
    if n == 1:
        print(b)
    else:
        print(a)
else:
    for i in range(3, n + 1, 1):
        c = 3 * a - b + 2
        b = a
        a = c
    print(c)
