#-------------------------------------
# Arduino-Python Serial Communication
#-------------------------------------
import os; os.system('cls')
import tkinter as tk
#---------------------------------------------------------------
import serial
arduino = serial.Serial(port='COM3', baudrate=9600, timeout=0.1)
#---------------------------------------------------------------
msgWR = input('Write Message: ')
arduino.write(bytes(msgWR,'utf-8'))
msgRD = arduino.readline()
msgRD = msgRD.decode('utf-8')
print(f'From Arduino: {msgRD}\n')

# #---------------------------------------------------------------
# def redLED():
#     arduino.write(bytes('R', 'utf-8'))
# def greenLED():
#     arduino.write(bytes('G', 'utf-8'))
# def blueLED():
#     arduino.write(bytes('B', 'utf-8'))
# #---------------------------------------------------------------
# win = tk.Tk(); win.title('Toggle LED'); win.minsize(230, 75)

# label = tk.Label(text='')
# label.grid(column=1, row=1)
# label = tk.Label(text=' Toggle ', font='calibri', fg='blue')
# label.grid(column=1, row=2)
# btn = tk.Button(text='  R  ',
#                 font='calibri 12 bold',
#                 bg='#FF6666', 
#                 command=redLED)
# btn.grid(column=2, row=2)
# btn = tk.Button(text='  G  ',
#                 font='calibri 12 bold',
#                 bg='#90EE90',
#                 command=greenLED)
# btn.grid(column=3, row=2)
# btn = tk.Button(text='  B  ',
#                 font='calibri 12 bold',
#                 bg='#ADD8E6',
#                 command=blueLED)
# btn.grid(column=4, row=2)
# label = tk.Label(text='   LED   ',
#                  font='calibri',
#                  fg='blue')
# label.grid(column=5, row=2)

# win.mainloop()

