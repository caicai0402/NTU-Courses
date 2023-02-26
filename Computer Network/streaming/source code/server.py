# Reference:
# 1. https://gist.github.com/0xAlon/09d0706c3c8f2601197569267c6552e2
# 2. https://itw01.com/23OR6EN.html

import sys
import socket
import cv2
import wave
import threading
import struct
import pickle
import zlib

class Server(object):
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind((self.host, self.port))

    def __del__(self):
        self.socket.close()
        self.cap.release()
        self.wf.close()
        
    def run(self):
        self.socket.listen(10)
        print("Server starts listening!")
        while True:
            client, address = self.socket.accept()
            print("Server receives a client from", address, end = '.\n')
            threading.Thread(target = self.handler, args = (client, address)).start()

    def handler(self, client, address):
        cap = cv2.VideoCapture("never.mp4")
        wf = wave.open("never.wav", "rb")

        print("Start sending video and audio.")
        while cap.isOpened():
            ret, frame = cap.read()
            data = zlib.compress(pickle.dumps(frame))
            try:
                client.sendall(struct.pack("L", len(data)) + data)
            except:
                print("Video sending fail.")
                break  

            data = zlib.compress(pickle.dumps(wf.readframes(1470)))
            try:
                client.sendall(struct.pack("L", len(data)) + data)
            except:
                print("Audio sending fail.")
                break
        
        cap.release()
        wf.close()
        return
        
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: python3 server.py {port number}")
    else:
        server = Server('', int(sys.argv[1]))
        server.run()