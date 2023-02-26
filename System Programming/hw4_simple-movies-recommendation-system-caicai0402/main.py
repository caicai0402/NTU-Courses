testcase = input()
wrong = []

with open("./testcases/input" + testcase + ".txt", "r") as f:
    num = int(f.readline())
    for i in range(num):
        id = (f.readline().split())[0]
        with open("./answers/" + id + "s.out", "r") as fs:
            with open(id + "t.out", "r") as ft:
                with open(id + "p.out", "r") as fp:
                    s = fs.readlines()
                    t = ft.readlines()
                    p = fp.readlines()
                    if s != t or s != p:
                        wrong.append(id)

if len(wrong) == 0:
    print(f"\033[32mAC on test case {testcase}!\033[0m")
else:
    print("\033[31mWA on id:", end = ' ')
    for i in range(len(wrong)):
        if i != len(wrong) - 1:
            print(wrong[i], end = ", ")
        else:
            print(wrong[i], end = "...")
    print("\033[0m")
