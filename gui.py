import tkinter
import tkinter.messagebox
import customtkinter
import os
from tkinter import filedialog
from PIL import Image
import serial.tools.list_ports
import time

customtkinter.set_appearance_mode(
    "Light")  # Modes: "System" (standard), "Dark", "Light"
customtkinter.set_default_color_theme(
    "blue")  # Themes: "blue" (standard), "green", "dark-blue"


class App(customtkinter.CTk):

  def __init__(self):
    super().__init__()

    # configure window
    self.title("UNCC_ECE_PROBE")
    self.geometry(f"{1150}x{750}")

    # configure grid layout (4x4)
    self.grid_columnconfigure((0,1,2), weight=1)
    self.grid_rowconfigure((0, 1, 2), weight=1)

    # create sidebar frame with widgets
    self.sidebar_frame = customtkinter.CTkFrame(self,
                                                width=140,
                                                corner_radius=0)
    self.sidebar_frame.grid(row=0, column=0, rowspan=4, sticky="nsew")
    self.sidebar_frame.grid_rowconfigure(4, weight=1)
    self.logo_label = customtkinter.CTkLabel(self.sidebar_frame,
                                             text="UNCC_ECE_PROBE",
                                             font=customtkinter.CTkFont(
                                                 size=20, weight="bold"))
    self.logo_label.grid(row=0, column=0, padx=20, pady=(20, 10))
   # self.gap_avg = customtkinter.CTkLabel(master=self, height=20, width=20, text= "gap_average")
   # self.gap_avg.grid(row=2, column=1, padx=2, pady=(20, 10))

  # Open Program
    self.Open_program_button = customtkinter.CTkButton(
      self.sidebar_frame, text="Choose Program to Open", command = self.open_program)
    self.Open_program_button.grid(row=1, column = 0, padx=20, pady=10)

    self.Open_IV_program_button = customtkinter.CTkButton(
      self.sidebar_frame, text="Open IV Program", command = self.open_IV_program)
    self.Open_IV_program_button.grid(row=2, column=0, padx=20, pady=10)
    
    self.Open_arduino_program_button = customtkinter.CTkButton(
      self.sidebar_frame, text="Open Arduino Program", command = self.open_arduino_program)
    self.Open_arduino_program_button.grid(row=3, column=0, padx=20, pady=10)

    self.label_open_program = customtkinter.CTkLabel(
      self.sidebar_frame, text="Open Program:", anchor="w")
    self.label_open_program.grid(row=5, column=0, padx=20, pady=(10, 0))
    
    self.label_open_iv = customtkinter.CTkLabel(
      self.sidebar_frame, text="IV Path:", anchor="w")
    self.label_open_iv.grid(row=6, column=0, padx=20, pady=(10, 0))
    
    self.label_open_arduino = customtkinter.CTkLabel(
      self.sidebar_frame, text="Arduino Path:", anchor="w")
    self.label_open_arduino.grid(row=7, column=0, padx=20, pady=(10, 0))

  
    self.scaling_label = customtkinter.CTkLabel(self.sidebar_frame,
                                                text="UI Scaling:",
                                                anchor="w")
    self.scaling_label.grid(row=9, column=0, padx=20, pady=(10, 0))
    self.scaling_optionemenu = customtkinter.CTkOptionMenu(
        self.sidebar_frame,
        values=["80%", "90%", "100%", "110%", "120%"],
        command=self.change_scaling_event)
    self.scaling_optionemenu.grid(row=10, column=0, padx=20, pady=(10, 20))

      # configure tab view and locate in grid
    self.tabview = customtkinter.CTkTabview(self, width=150)
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
                                       sticky="nsew")
    self.slider_progressbar_frame.grid_columnconfigure(0, weight=1)
    self.slider_progressbar_frame.grid_rowconfigure(4, weight=1)
    self.progressbar_1 = customtkinter.CTkProgressBar(
        self.slider_progressbar_frame, progress_color= "green", width = 250)
    self.progressbar_1.grid(row=2,
                            column=0,
                            padx=(20, 10),
                            pady=(10, 10),
                            sticky="w")

  # Start Button
    self.button_frame = customtkinter.CTkFrame(self)
    self.button_frame.grid(row=2,
                              column=2,
                              padx=(20, 20),
                              pady=(10, 10),
                              sticky="n")

    self.start_button = customtkinter.CTkButton(
      master=self.button_frame, command=self.button_start, fg_color="green", text="Start", width = 150)
    self.start_button.grid(row=1, column=2, pady=20, padx=20, sticky="n")
    self.resume_button = customtkinter.CTkButton(
      master=self.button_frame, command=self.button_resume, fg_color="Orange", text="Pause", width = 150)
    self.resume_button.grid(row=2, column=2, pady=20, padx=20, sticky="n")
    self.stop_button = customtkinter.CTkButton(
      master=self.button_frame, command=self.button_stop, fg_color="red", text="Stop", width = 150)
    self.stop_button.grid(row=3, column=2, pady=20, padx=20, sticky="n")
  # set default values
    self.stop_button.configure(state="disabled")
    self.resume_button.configure(state="disabled")
    self.scaling_optionemenu.set("100%")
    self.progressbar_1.configure(mode="determinate",determinate_speed= 0.1)

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
    self.send_button = customtkinter.CTkButton(self.sidebar_frame, text= "Send Values", command=self.var_send_button)
    self.send_button.grid(row=5, column=0, padx=20, pady=10)

  def button_resume(self):
    print("button clicked")
    self.progressbar_1.stop()
    self.resume_button.configure(state="disabled")
    self.start_button.configure(state="enabled", text = "Resume" )
    self.stop_button.configure(state="enabled")
    
  def open_program(self):
    file_path = filedialog.askopenfilename()
    self.label_open_program.configure(text=file_path)
    os.system('"%s' % file_path)

  def open_IV_program(self):
    IV_path = '"C:/Users/PVRL-01/Documents/S24PROBE/IV.exe"'
    self.label_open_iv.configure(text=IV_path) 
    os.system('"%s' % IV_path)

  def open_arduino_program(self):
    arduino_path = '"C:/Users/PVRL-01/Documents/S24PROBE/ArduinoIDE/ArduinoIDE.exe"'
    self.label_open_arduino.configure(text=arduino_path)
    os.system('"%s' % arduino_path)
    
  def connect_arduino(self):
    ports = serial.tools.list_ports.comports()
    serial_inst = serial.Serial()
    ports_list = []
    port_val = 'COM3'

    for port in ports:
        ports_list.append(str(port))
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

    global send_button
    #send_button = 0   #CHANGE
    # Send Variable Values Button   #FIX
    #self.send_button = customtkinter.CTkButton(self.sidebar_frame, text= "Send Values", command=self.var_send_button)
    #self.send_button.grid(row=5, column=0, padx=20, pady=10)
       
        
    ## SEND VARIABLES 
    while True:
        global gap_width
        global num_fingers
     
        print("begin loop") 
        #input("Press Enter to continue...")
        #print("gap width", gap_width)
        #print("# of fingers:", num_fingers)
        #gap_width = gap_width + '\r'
        #num_fingers = num_fingers + '\r'
        #serial_inst.write(gap_width.encode('utf-8'))  
        #serial_inst.write(num_fingers.encode('utf-8'))
        
        
        if send_button == 1:
          variables = gap_width + "," + num_fingers
          variables = variables + '\n'
          serial_inst.write(variables.encode('utf-8'))
          #serial_inst.flush()
          print("SENT:", variables) 
          #time.sleep(5)
          #return None       #RUNS IN INFINITE LOOP CANNOT RUN IF THIS IS UNCOMMENTED (NEEDS FIX ASAP) 
        else:
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
    global send_button
    send_button = 1
    self.connect_arduino()
    return send_button

def open_input_parameters(selection):
        dialog = customtkinter.CTkInputDialog(text="Type in Value:",
                                          title=selection)
        var_name = selection
        var_data = dialog.get_input()
               
        if (var_name == "Gap Width (mm)"):
          global gap_width
          gap_width = var_data
          #print("gap width:", gap_width)
          label_gap_width = customtkinter.CTkLabel(app, text = "Gap Width: " + gap_width)   #MAKE TRANSPARENT AND INIT EARLIER, .UPDATE TEXT AND COLOR HERE
          label_gap_width.grid(row=0, column=2, padx=2, pady=80, sticky="n")
          return gap_width  
        else:
          global num_fingers
          num_fingers = var_data
          #print("# of fingers:", num_fingers)
          label_num_fingers = customtkinter.CTkLabel(app, text = "# of Fingers: " + num_fingers)
          label_num_fingers.grid(row=0, column=2, padx=2, pady=2, sticky="ew")
          return num_fingers
 
if __name__ == "__main__":
  app = App()
  app.mainloop()
  
  # PYTHON TO EXE line command below
  # pyinstaller gui.py --onefile --windowed 
