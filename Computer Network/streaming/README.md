# Phase 2

## 資工四 胡材溢 B08208032

## Usage

```
requirements.txt

source code
    |- server.py
    |- client.py

media source

```

To excute two python files, packages required are written in requirements.txt.

Run following command to install.

```
pip install -r reqirements.txt
```

In MacOS, an error may occur when installing PyAudio.

Run

```
brew install portaudio
```

first, and try the pip install command above again would help.

To excute server.py, you have to specify the port using.

```
python3 server.py [PORT]
```

For example:

```
python3 server.py 1124
````

And the server will run on localhost and create a socket with the port number PORT.


To excute client.py, you have to specify the server address, port.

```
python3 client.py [ADDRESS] [PORT]
```

For example:

```
python3 client.py linux7.csie.ntu.edu.tw 1124
````

Then client will start to receive and play music video.

## Deployed Server Address

A server has already been deployed at:

```
linux7.csie.ntu.edu.tw:1124
```

## Implement Items

1. Video Streaming +20
2. Audio Streaming +20
3. Multithread +8
4. Data Compression +?