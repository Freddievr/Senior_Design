import tkinter
import tkinter.messagebox
import customtkinter
import os
from tkinter import filedialog
from PIL import Image
import serial.tools.list_ports

class App(customtkinter.CTk):
  def __init__(self):
    super().__init__()
    self.geometry("400x150")
    self.title("UNCC_ECE_PROBE")
    # configure grid view
    self.grid_columnconfigure((0,1,2), weight=1)
    self.grid_rowconfigure((0, 1, 2), weight=1)
    
    # configure tab view and locate in grid
    self.tabview = customtkinter.CTkTabview(self, width=150)
    self.tabview.grid(row=0,
                      column=2,
                      padx=(30, 30),
                      pady=(10, 10),
                      sticky="nsew")
    # add individual tabs
    self.tabview.add("Parameters")
    # configure grid of individual tabs
    self.tabview.tab("Parameters").grid_columnconfigure(
        0, weight=1)  

    self.optionmenu_1_var = customtkinter.StringVar(value="Select Variable")
    self.optionmenu_1 = customtkinter.CTkOptionMenu(
        self.tabview.tab("Parameters"),
        dynamic_resizing=False,
        values=["Gap Width", "# of Fingers"],
        command=open_input_parameters,
        variable=self.optionmenu_1_var)
    self.optionmenu_1.grid(row=0, column=2, padx=20, pady=(20, 10))

 # Start Button
    self.button_frame = customtkinter.CTkFrame(self)
    self.button_frame.grid(row=2,
                              column=3,
                              padx=(20, 20),
                              pady=(10, 10),
                              sticky="n")

    self.start_button = customtkinter.CTkButton(
      master=self.button_frame, command=button_start, fg_color="green", text="Start", width = 150)
    self.start_button.grid(row=1, column=1, pady=20, padx=20, sticky="n")
    self.resume_button = customtkinter.CTkButton(
      master=self.button_frame, command=button_resume, fg_color="Orange", text="Pause", width = 150)
    self.resume_button.grid(row=2, column=1, pady=20, padx=20, sticky="n")
    self.stop_button = customtkinter.CTkButton(
      master=self.button_frame, command=button_stop, fg_color="red", text="Stop", width = 150)
    self.stop_button.grid(row=3, column=1, pady=20, padx=20, sticky="n")
    
def open_input_parameters(selection):
        print("optionmenu dropdown clicked:", selection)
        dialog = customtkinter.CTkInputDialog(text="Type in Value:",
                                          title=selection)
        var_name = selection
        var_data = dialog.get_input()
        
               
        if (var_name == "Gap Width"):
            gap_width = var_data
            print("gap width", gap_width)     
        else:
            num_fingers = var_data
            print("# of fingers:", num_fingers)
       
        variable = str(var_name) + ": " + var_data
        print(variable)
    
        return 

# FUNCTIONS Define

def button_stop(self):    
    self.progressbar_1.stop()
    self.progressbar_1.set(0.0)
    print("STOPPED")
    self.resume_button.configure(state="disabled")
    self.start_button.configure(state="enabled", text = "Start")
    self.stop_button.configure(state="disabled")

def button_start(self):
    print("Program Started")
    self.progressbar_1.start()
    self.resume_button.configure(state="enabled")
    self.start_button.configure(state="disabled",text_color_disabled = "green", text = "Start")
    self.stop_button.configure(state="enabled")
    self.connect_arduino()

def button_resume(self):
    print("button clicked")
    self.progressbar_1.stop()
    self.resume_button.configure(state="disabled")
    self.start_button.configure(state="enabled", text = "Resume" )
    self.stop_button.configure(state="enabled")   
    
def connect_arduino(self):
    ports = serial.tools.list_ports.comports()
    serial_inst = serial.Serial()
    ports_list = []

    for port in ports:
        ports_list.append(str(port))
        print(str(port))
              
    for x in range(0,len(ports_list)):
        val = self.COM_input_dialog()
        if ports_list[x].startswith("COM" +str(val)):
            port_val = "COM" + str(val)
            print(serial_inst.port)

    serial_inst.baudrate = 9600
    serial_inst.port = port_val
    serial_inst.open()

    while True:
        command = input("Arduino Command: (ON/OFF): ")
        serial_inst.write(command.encode('utf-8'))  #NEEDS TO BE UPDATED FOR GUI

        ## SEND VARIABLES






        if command == 'EXIT':
            return None  
    
def UNO_input_dialog(self): 
    val = customtkinter.CTkInputDialog(text="Arduino Command: (ON/OFF): ",
                                          title="Arduino Command")
    self.serial_inst.write(val.encode('utf-8'))

def COM_input_dialog(self): 
    val = customtkinter.CTkInputDialog(text="Select Port: COM",
                                          title="Select Port")
    print("Selected Port:", val.get_input())
    return val

if __name__ == "__main__":
  app = App()
  app.mainloop()