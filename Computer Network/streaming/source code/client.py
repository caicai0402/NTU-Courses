# Reference:
# 1. https://gist.github.com/0xAlon/09d0706c3c8f2601197569267c6552e2
# 2. https://itw01.com/23OR6EN.html

import sys
import socket
import cv2
import pyaudio
import time
import struct
import pickle
import zlib

class Client(object):
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.pyaudio = pyaudio.PyAudio()
        self.stream = self.pyaudio.open(format = 8,
                                        channels = 2,
                                        rate = 44100,
                                        output = True)
                                        # frames_per_buffer = 1024)

        cv2.namedWindow('Video', cv2.WINDOW_NORMAL)
        
    def __del__(self):
        self.socket.close()
        try:
            cv2.destroyAllWindows()
        except:
            pass
        if self.stream is not None:
            self.stream.stop_stream()
            self.stream.close()
        self.pyaudio.terminate()
        
    def run(self):
        print("Try to connect to server...")
        while True:
            try:
                self.socket.connect((self.host, self.port))
                break
            except:
                time.sleep(3)
        print("Client connects to server successfully!")
       
        data = "".encode("utf-8")
        aframes = b""
        while True:
            while len(data) < struct.calcsize("L"):
                data += self.socket.recv(4096)
            size, data = struct.unpack("L", data[:struct.calcsize("L")])[0], data[struct.calcsize("L"):]

            while len(data) < size:
                data += self.socket.recv(4096)
            vframe, data = pickle.loads(zlib.decompress(data[:size])), data[size:]

            while len(data) < struct.calcsize("L"):
                data += self.socket.recv(4096)
            size, data = struct.unpack("L", data[:struct.calcsize("L")])[0], data[struct.calcsize("L"):]

            while len(data) < size:
                data += self.socket.recv(4096)

            aframes, data = pickle.loads(zlib.decompress(data[:size])), data[size:]

            self.stream.write(aframes)
            cv2.imshow('Video', vframe)
            if cv2.waitKey(1) & 0xFF == 27:
                break

        return

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("usage: python3 client.py {IP Address} {Port Number}")
    else:
        Client = Client(sys.argv[1], int(sys.argv[2]))
        Client.run()