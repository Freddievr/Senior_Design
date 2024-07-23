import tkinter
import tkinter.messagebox
import customtkinter
import os
import matplotlib.pyplot as plt
import serial.tools.list_ports
from tkinter import filedialog
from PIL import Image
from datetime import datetime
import time
import pyvisa
import pandas as pd
import numpy as np

customtkinter.set_appearance_mode("System")  # Modes: "System" (standard), "Dark", "Light"
customtkinter.set_default_color_theme("uncc.json") 

class App(customtkinter.CTk):
  def __init__(self):
    super().__init__()

    # configure window
    self.title("PVRL - Contact Resistance Measurements")
    self.geometry(f"{800}x{650}")
    self.iconbitmap("C:/Users/PVRL-01/Documents/S24PROBE/CODE/PVRL-Logo.ico")
    # configure grid layout (4x4)
    self.grid_columnconfigure((0,1,2), weight=1)
    self.grid_rowconfigure((0, 1, 2), weight=1)
   
    # create sidebar frame with widgets
    self.sidebar_frame = customtkinter.CTkFrame(self,
                                                width=10,
                                                corner_radius=0)
    self.sidebar_frame.grid(row=0, column=0, rowspan=4, sticky="nsew")
    self.sidebar_frame.grid_rowconfigure(4, weight=1)
    self.logo_label = customtkinter.CTkLabel(self.sidebar_frame,
                                             text="UNCC_ECE_PROBE",
                                             font=customtkinter.CTkFont(
                                              size=18, weight="bold"))
    self.logo_label.grid(row=0, column=0, padx=10, pady=(10, 10))
   # self.gap_avg = customtkinter.CTkLabel(master=self, height=20, width=20, text= "gap_average")
   # self.gap_avg.grid(row=2, column=1, padx=2, pady=(20, 10))

  # Open Program
    self.Open_program_button = customtkinter.CTkButton(
      self.sidebar_frame, text="Choose Program to Open", command = self.open_program)
    self.Open_program_button.grid(row=1, column = 0, padx=10, pady=10)
   
    self.Open_arduino_program_button = customtkinter.CTkButton(
      self.sidebar_frame, text="Open Arduino Program", command = self.open_arduino_program)
    self.Open_arduino_program_button.grid(row=2, column=0, padx=10, pady=10)
    # configure tab view and locate in grid
    self.tabview = customtkinter.CTkTabview(self, width=150,segmented_button_selected_hover_color="#A49665", border_color= "#A49665")
    self.tabview.grid(row=0,
                      column=1,
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
        values=["Gap Width (mm)", "# of Fingers"],
        command=open_input_parameters,
        variable=self.optionmenu_1_var)
    self.optionmenu_1.grid(row=0, column=0, padx=10, pady=(10, 10))
        
    # create progressbar frame
    self.slider_progressbar_frame = customtkinter.CTkFrame(
        self, fg_color="transparent")
    self.slider_progressbar_frame.grid(row=6,
                                       column=1,
                                       padx=(20, 0),
                                       pady=(20, 0),
                                       sticky="nse")
    self.slider_progressbar_frame.grid_columnconfigure(0, weight=1)
    self.slider_progressbar_frame.grid_rowconfigure(4, weight=1)
    self.progressbar_1 = customtkinter.CTkProgressBar(
        self.slider_progressbar_frame, progress_color= "green", width = 250)
    self.progressbar_1.grid(row=2,
                            column=0,
                            padx=(20, 10),
                            pady=(10, 10),
                            sticky="w")
      
    self.progress_set_label = customtkinter.CTkLabel(self.slider_progressbar_frame,
                                             text="Progress: ",
                                             font=customtkinter.CTkFont(
                                              size=14, weight="bold"))
    self.progress_set_label.grid(row=1,
                            column=0,
                            padx=(20, 10),
                            pady=(10, 10),
                            sticky="w")
          
    self.progress_label = customtkinter.CTkLabel(self.slider_progressbar_frame,
                                             text=  "N/A",
                                             font=customtkinter.CTkFont(size=12, weight="bold"))
    self.progress_label.grid(row=1,
                            column=0,
                            padx=(10, 10),
                            pady=(10, 10),
                            sticky="e")
  # Start Button
    self.button_frame = customtkinter.CTkFrame(self)
    self.button_frame.grid(row=2,
                              column=2,
                              padx=(20, 20),
                              pady=(10, 10),
                              sticky="n")

    self.start_button = customtkinter.CTkButton(
      master=self.button_frame, command=self.connect_arduino, fg_color="green", text="Start", width = 150, hover_color= "#228B22", text_color = "black")
    self.start_button.grid(row=1, column=2, pady=20, padx=20, sticky="n")
    self.resume_button = customtkinter.CTkButton(
      master=self.button_frame, command=self.button_pause, fg_color="Orange", text="Pause", width = 150, text_color = "black")
    self.resume_button.grid(row=2, column=2, pady=20, padx=20, sticky="n")
    self.stop_button = customtkinter.CTkButton(
      master=self.button_frame, command=self.button_stop, fg_color="red", text="Stop", width = 150, text_color = "black")
    self.stop_button.grid(row=3, column=2, pady=20, padx=20, sticky="n")
  # set default values
    self.stop_button.configure(state="disabled")
    self.resume_button.configure(state="disabled")
    self.progressbar_1.configure(mode="determinate",determinate_speed= 0.1)
    
    self.label_gap_width = customtkinter.CTkLabel(self, text = "Gap Width: ")  
    self.label_gap_width.grid(row=0, column=2, padx=2, pady=80, sticky="n")
    self.label_num_fingers = customtkinter.CTkLabel(self, text = "# of Fingers: ")
    self.label_num_fingers.grid(row=0, column=2, padx=2, pady=2, sticky="ew")
    
# FUNCTIONS Define

  def button_stop(self):    
    self.progressbar_1.stop()
    self.progressbar_1.set(0.0)
    print("STOPPED")
    self.resume_button.configure(state="disabled")
    self.start_button.configure(state="enabled", text = "Start")
    self.stop_button.configure(state="disabled")
    self.progress_label.configure(text = "Stopped")

  def button_start(self):
    print("Program Started")
    self.progressbar_1.start()
    self.resume_button.configure(state="enabled")
    self.start_button.configure(state="disabled",text_color_disabled = "green", text = "Start")
    self.stop_button.configure(state="enabled")
    self.send_button = customtkinter.CTkButton(self.sidebar_frame, text= "Send Values", command=self.var_send_button)
    self.send_button.grid(row=5, column=0, padx=20, pady=10)
    self.progress_label.configure(text = "Starting...")

  def button_pause(self):
    print("PAUSED")
    self.progressbar_1.stop()
    self.resume_button.configure(state="disabled")
    self.start_button.configure(state="enabled", text = "Resume" )
    self.stop_button.configure(state="enabled")
    self.progress_label.configure(text = "Paused")
    
  def open_program(self):
    file_path = filedialog.askopenfilename()
    self.label_open_program.configure(text=file_path)
    os.system('"%s' % file_path)

  def open_arduino_program(self):
    arduino_path = '"C:/Users/PVRL-01/Documents/S24PROBE/ArduinoIDE/ArduinoIDE.exe"'
    self.label_open_arduino.configure(text=arduino_path)
    os.system('"%s' % arduino_path)
    
  ################################################################# 
  def connect_arduino(self):
    ports = serial.tools.list_ports.comports()
    serial_inst = serial.Serial(timeout = 0.1)
    ports_list = []
    port_val = 'COM4'

    for port in ports:
        #ports_list.append(str(port))
        print(str(port))
    
    # Commented OUT FOR DEMO          #
    #for x in range(0,len(ports_list)):
    #    val = self.COM_input_dialog()
    #    if ports_list[x].startswith("COM" +str(val)):
    #        port_val = "COM" + str(val)
    #        print(serial_inst.port)
    
    serial_inst.baudrate = 9600
    serial_inst.port = port_val
    serial_inst.open()

    # Send Variable Values Button   #FIX
    # self.send_button = customtkinter.CTkButton(self.sidebar_frame, text= "Send Values", command=self.var_send_button)
    # self.send_button.grid(row=5, column=0, padx=20, pady=10)
        

    ## SEND VARIABLES 
    global gap_width
    global num_fingers
    
    variables = gap_width + "," + num_fingers

    print('Message Sent: ' + variables)
    msgWR = variables
    time.sleep(2)
    serial_inst.write(bytes(msgWR,'utf-8'))
    msgRD = serial_inst.readline()
    msgRD = msgRD.decode('utf-8')
    print(f'From Arduino: {msgRD}\n')
##############################################################################
    
  def UNO_input_dialog(self): 
    val = customtkinter.CTkInputDialog(text="Arduino Command: (ON/OFF): ",
                                          title="Arduino Command")
    self.serial_inst.write(val.encode('utf-8'))

  def COM_input_dialog(self): 
    val = customtkinter.CTkInputDialog(text="Select Port: COM",
                                          title="Select Port")
    print("Selected Port:", val.get_input())
    return val
        
# given
  def open_input_dialog_event(self):
    dialog = customtkinter.CTkInputDialog(text="Type in Value:",
                                          title="Input Variables")
    print("Input Value:", dialog.get_input())

  def change_appearance_mode_event(self, new_appearance_mode: str):   #Remove near end
    customtkinter.set_appearance_mode(new_appearance_mode)

  def change_scaling_event(self, new_scaling: str):
    new_scaling_float = int(new_scaling.replace("%", "")) / 100
    customtkinter.set_widget_scaling(new_scaling_float)
    
  def var_send_button(self): # Send Values Button # NOT NEEDED (Placeholder)
    self.progress_label.configure(text = "Measuring...")
    self.connect_arduino()

def open_input_parameters(selection):
        dialog = customtkinter.CTkInputDialog(text="Type in Value:",
                                          title=selection)
        var_name = selection
        var_data = dialog.get_input()
        
        if (var_name == "Gap Width (mm)"):
          global gap_width
          gap_width = var_data
          #print("gap width:", gap_width)
          app.label_gap_width.configure(text = "Gap Width: " + gap_width + " mm")
          return gap_width  
        else:
          global num_fingers
          num_fingers = var_data
          #print("# of fingers:", num_fingers)
          app.label_num_fingers.configure(text = "# of Fingers: " + num_fingers)
          return num_fingers     

if __name__ == "__main__":
  app = App()
  app.mainloop()
  
  # PYTHON TO EXE line command below
  # pyinstaller gui.py --onefile --windowed 
