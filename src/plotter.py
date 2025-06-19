import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import re

SERIAL_PORT = 'COM8'  # Cambia a tu puerto
BAUD_RATE = 115200
NUM_MICS = 5
SAMPLES_TO_PLOT = 10

ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

mic_buffers = [[0]*SAMPLES_TO_PLOT for _ in range(NUM_MICS)]

fig, ax = plt.subplots()
lines = [ax.plot([], [], label=f'Mic {i}')[0] for i in range(NUM_MICS)]
ax.set_ylim(0, 4095)  # Rango para ADC de 12 bits
ax.set_xlim(0, SAMPLES_TO_PLOT-1)
ax.set_xlabel('Muestra')
ax.set_ylabel('Valor ADC')
ax.set_title('Datos de cada micrófono')
ax.legend()

def update(frame):
    global mic_buffers
    for _ in range(NUM_MICS):
        line = ser.readline().decode(errors='ignore').strip()
        match = re.match(r'Mic(\d+):\s*([-\d, ]+)', line)
        if match:
            mic_idx = int(match.group(1))
            values = [int(v) for v in match.group(2).replace(' ', '').split(',') if v]
            if 0 <= mic_idx < NUM_MICS and len(values) == SAMPLES_TO_PLOT:
                mic_buffers[mic_idx] = values
    for i, l in enumerate(lines):
        l.set_data(range(SAMPLES_TO_PLOT), mic_buffers[i])
    return lines

ani = animation.FuncAnimation(fig, update, blit=True, interval=100)
plt.tight_layout()
plt.show()