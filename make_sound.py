import numpy as np
import math
import scipy

symbol_time = 8192/12000
sr = 22050

tick_hz = 450
tack_hz = 700
start_pad = 1.0
end_pad = 1.0
total_time = 162*symbol_time + start_pad + end_pad
n_samples = int(total_time*sr)

samples = np.zeros(n_samples, dtype=np.uint8)

def add_sound_at(start_time, frequency, length=0.1):
    start_sample = int(start_time*sr)
    for i in range(int(length*sr)):
        samples[start_sample+i] = 128 + int(64*math.sin(2*math.pi*i*frequency/sr))

def main():
    for i in range(162):
        frequency = tack_hz if (i+1) % 9 == 0 else tick_hz
        add_sound_at(start_pad + i*symbol_time, frequency)
    add_sound_at(0.0, 1000.0, length=start_pad)

    #print(list(samples))
    scipy.io.wavfile.write("ticks.wav", sr, samples)

main()
