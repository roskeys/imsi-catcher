IMSI catcher
======

# This program was made for research purpose and demonstrate how IMSI catcher work. Not for bad hacking!
![image](https://github.com/user-attachments/assets/aa33f8e5-6187-4afb-ae44-872e9467874e)

This project setup a fake base station modified from SRSRAN and tries to attract cell phones to connect to the fake base station and extract the International Mobile Subscriber Identity.

## What you need?
- PC with linux installed (tested with ubuntu 22.04)
- 1 SDR (tested with B210)

## How to build?
```bash
sudo apt install build-essential cmake libfftw3-dev libmbedtls-dev libboost-program-options-dev libconfig++-dev libsctp-dev
mkdir build
cmake -B build
cmake --build build --target srsenb
```

## Docker
You can simply use docker with out affecting local environment
```bash
docker compose build
docker compose up
```

Tutorial:
- https://www.roskey.net/2024/12/%e5%b0%86srsran-4g%e6%94%b9%e4%b8%baimsi-catcher/
