import socket
import numpy as np
import json
from pythonosc.udp_client import SimpleUDPClient

HOST = '0.0.0.0'
PORT = 9000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(5)

###########

def out_json(data, out_path):
    with open(out_path, 'w') as outfile:
        json.dump(data, outfile)

def reset():
    global data_t
    data_t = np.array([])
    return

#def callback(path, args, types, src, params):
def callback(args) -> None:
    print("QWERQWER start")
    ip, port = "localhost", 9001
    client = SimpleUDPClient(ip, port)
    tmp = args.split()
    data = [int(tmp[0]), int(tmp[1]) / 255, int(tmp[2]), int(tmp[3]) / 255, int(tmp[4]), int(tmp[5]) / 255]
    client.send_message("/first", data)
    
    print("QWERQWER done")
    return

##############

print('server start at: %s:%s' % (HOST, PORT))
print('wait for connection...')

while True:
    conn, addr = s.accept()
    print('connected by ' + str(addr))

    while True:
        print("ZXCVZXCV")
        indata = conn.recv(4096)
        if len(indata) == 0: # connection closed
            conn.close()
            print('client closed connection.')
            break
        data = indata.decode()
        data = data.split("\n")[-1]
        # print('recv: ' + indata.decode())
        print("QWERQWER", data, "QWERQWER")

        conn.send(indata)

        callback(data)