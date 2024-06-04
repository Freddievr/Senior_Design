import serial.tools.list_ports

ports = serial.tools.list_ports.comports()
serial_inst = serial.Serial()

ports_list = []

for port in ports:
    ports_list.append(str(port))
    print(str(port))
          
val = input("Select Port: COM")
#val = self.COM_input_dialog()                                        title="Select Port")
for x in range(0,len(ports_list)):
    if ports_list[x].startswith("COM" + str(val)):
        port_var = "COM" + str(val)
        print(port_var)

serial_inst.baudrate = 9600
serial_inst.port = port_var
serial_inst.open()

while True:
    command = input("Arduino Command: (ON/OFF): ")
    serial_inst.write(command.encode('utf-8'))  #NEEDS TO BE UPDATED FOR GUI

    if command == 'EXIT':
        exit()




