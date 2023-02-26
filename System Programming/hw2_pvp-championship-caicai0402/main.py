letters = "ABCDEFGHIJKLMN"
nums = "01234567"
testcase = input()

for i in range(len(letters)):
    with open("./log_battle" + letters[i] + ".txt", "r") as f1:
        with open("./sample_output/testcase" + testcase + "/log_battle" + letters[i] + ".txt", "r") as f2:
            data1 = f1.readlines()
            data2 = f2.readlines()
            if len(data1) != len(data2):
                print("log_battle" + letters[i], "gg1")
            else:
                for j in range(len(data1)):
                    data1[j] = data1[j].split()
                    data2[j] = data2[j].split()
                    if len(data1[j]) != len(data2[j]):
                        print("log_battle" + letters[i], "gg2")
                    else:
                        for k in range(len(data1[j])):
                            if k == 0 or k == 3:
                                data1[j][k].split(',')
                                data2[j][k].split(',')
                                if data1[j][k][0] != data2[j][k][0]:
                                    print("log_battle" + letters[i], "gg3")
                            elif data1[j][k] != data2[j][k]:
                                print("log_battle" + letters[i], "gg4", data1[j][k], data2[j][k])

for i in range(len(nums)):
    with open("./log_player" + nums[i] + ".txt", "r") as f1:
        with open("./sample_output/testcase" + testcase + "/log_player" + nums[i] + ".txt", "r") as f2:
            data1 = f1.readlines()
            data2 = f2.readlines()
            if len(data1) != len(data2):
                print("log_player" + nums[i], "gg1")
            else:
                for j in range(len(data1)):
                    data1[j] = data1[j].split()
                    data2[j] = data2[j].split()
                    if len(data1[j]) != len(data2[j]):
                        print("log_player" + nums[i], "gg2")
                    else:
                        for k in range(len(data1[j])):
                            if k == 0 or k == 3:
                                data1[j][k].split(',')
                                data2[j][k].split(',')
                                if data1[j][k][0] != data2[j][k][0]:
                                    print("log_player" + nums[i], "gg3")
                            elif data1[j][k] != data2[j][k]:
                                print("log_player" + nums[i], "gg4", data1[j][k], data2[j][k])




                    

