# BiomedOne

BiomedOne is a three-part monitoring system for people quarantined due to COVID-19. It was designed as a tool to assist medical teams and is based on the detection of the main symptoms, namely temperature, heart rate and blood oxygen level (SpO2). It includes :
- A literature-based simulator of those symptoms for testing
- An embedded software declining itself in two versions for periodic and continuous monitoring 
- A email-based alert system which can run 24h/24h

## Simulator

### Usage
```
$ git clone https://github.com/Ibrassow/biomed-one.git
$ cd biomed-one/simulation
$ mkdir build 
$ cd build
$ cmake ..
$ cmake --build . --config Release //Windows
$ cd ../python
$ python test1.py 
```


## Alert system



## Embedded software

