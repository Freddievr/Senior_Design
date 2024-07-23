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

   # self.label_open_program = customtkinter.CTkLabel(
   #   self.sidebar_frame, text="Open Program:", anchor="w")
   # self.label_open_program.grid(row=5, column=0, padx=20, pady=(10, 0))
       
    #self.label_open_arduino = customtkinter.CTkLabel(
    #  self.sidebar_frame, text="Arduino Path:", anchor="w")
    #self.label_open_arduino.grid(row=7, column=0, padx=20, pady=(10, 0))

    self.Open_graph_button = customtkinter.CTkButton(
      self.sidebar_frame, text="Open Graph", command = self.open_graph)
    self.Open_graph_button.grid(row=4, column = 0, padx=20, pady=10)

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
      master=self.button_frame, command=self.button_start, fg_color="green", text="Start", width = 150, hover_color= "#228B22", text_color = "black")
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
    
  def open_graph(self):
    get_measurement_keithley_2401() 
    
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
    gap_width = "0.12"    
    num_fingers = "12"
      
    # variables = "<" + num_fingers + ">"
    variables = "<" + "Oo," + num_fingers + "," + gap_width + ">"
    #gapWidth = "g" + gap_width + '\n' + ">"
    #numFingers= "n" + num_fingers + '\n' + ">"
    #input("Press Enter to continue...")
    #print("gap width", gap_width)
    #print("# of fingers:", num_fingers)
    #gap_width = gap_width + '\r'
    #num_fingers = num_fingers + '\r'
    #serial_inst.write(gap_width.encode('utf-8'))
    #serial_inst.write(num_fingers.encode('utf-8'))
    print('Data Sent: ' + variables)
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

def get_measurement_keithley_2401():
    # Setup Connection to Meter
    rm = pyvisa.ResourceManager('@ivi')
    resources = rm.list_resources()
    print(resources)
    os.makedirs("C:/Users/PVRL-01/Documents/S24PROBE_Test_Results", exist_ok=True)      ## DIRECTORY FOR TEST RESULTS TO BE SAVED
    # Open Connection to Meter
    keithley = rm.open_resource('GPIB0::11::INSTR')
    print(keithley.query('*IDN?'))
    # Reset/Clear Status on Meter
    keithley.write('*RST')                  
    keithley.write('*CLS')
    # Setup VARIABLES 
    gap_width = 0.89            # in mm
    strip_width = 2             # in mm
    num_fingers = 10                                    ## CHANGE ##               
    numReadings = str(num_fingers*2)
    keithley.timeout = 11000 * num_fingers             # timeout set up according to # of fingers
        
    # Setup Meter
    keithley.write(":SOUR:FUNC:MODE CURR")             # Select current source 
    keithley.write("ROUTE:TERMINALS REAR")             # Select Rear Terminals 
    # setup current sweep mode & points
    keithley.write(":SOUR:CURR:MODE LIST")
    keithley.write(":SOUR:LIST:CURR -0.020,0.020")     # Sweep points -20mA,20mA
    keithley.write(":SOUR:SWE:POIN 2")
    keithley.write(":SOUR:SWE:DIR UP")
    # turn off concurrent functions
    keithley.write(":SENSE:FUNC:CONC OFF")
    keithley.write(":SENSE:CURR:PROT:RSYN ON")  
    # 0ms Trigger delay
    keithley.write(":TRIG:DEL 5")
    # 0ms Source delay
    keithley.write(":SOUR:DEL 0.0046")
    # set current complaince to 10x10^-3 (10mA)
    keithley.write(":SENSE:CURR:PROT 10e-3")
    keithley.write(":SYST:RSEN ON")                #4 point probe
    # Disable auto-zero state
    keithley.write(":SYSTEM:AZERO:STAT OFF")
    # ??? clear trace buffer?
    keithley.write(":TRAC:CLE")
    
    # store n readings in buffer
    keithley.write(":TRAC:POINTS " + numReadings)
    # set ARM count n= num_fingers
    keithley.write(":ARM:COUN " + str(num_fingers))
    # set ARM delay 
    #keithley.write(":ARM:TIM 4")
    # enable buffer for trace?
    keithley.write(":TRAC:FEED:CONT NEXT")
    # of sweep points n
    keithley.write("TRIG:COUN 2 ")
    
    # turn on output
    keithley.write(":OUTPUT ON")
    # trigger readings
    keithley.write(":INIT")
    inputs = keithley.query_ascii_values("trace:data?")
  
    values = np.array(inputs)
    values = values.reshape(-1, 10)
    # setup main dataframe
    col = ['Voltage (V) @ -20mA','Current-','Resistance1','Time1','Status1', 'Voltage (V) @ +20mA','Current+','Resistance2','Time2','Status2']
    df = pd.DataFrame(values,columns=col)
    df.drop(columns = ['Resistance1','Time1','Status1','Resistance2','Time2','Status2'], inplace = True)   
    row_array = []
    distance_array = []
    for i in range(0, num_fingers, 1) :
        row_array.append("Finger_" + str(i+1))
        distance_array.append(0.89*(i+1))
    df.index = row_array                        # index dataframe with finger #
    ### Calculate Contact Resistance ###
    # Setup Resistance & Distance Dataframe Columns
    df['Voltage (V) @ -20mA'] = df['Voltage (V) @ -20mA'] * -1
    df['R+'] = df['Voltage (V) @ +20mA'] / df['Current+'] 
    df['R-'] = df['Voltage (V) @ -20mA'] / df['Current-'] 
    df['Rmittel (\u03A9)'] = (df['R+'] - df['R-'])/2
    df['Distance (mm)'] = distance_array
    dft = df.copy()
    dft.drop(columns= ['R-','R+','Current-','Current+'], inplace = True)
    # Setup Temp DATAFRAME FOR CALCULATIONS
    df2 = pd.DataFrame()
    df2.loc[:,'Rmittel (\u03A9)'] = df.loc[:,'Rmittel (\u03A9)']
    df2.loc[:,'Distance (mm)'] = df.loc[:,'Distance (mm)']
    # Setup Temp DATAFRAME FOR CALCULATIONS 
    # CALCULATE INTERCEPTS
    distance_np = np.array(distance_array,dtype=np.float64)
    rmittel_array = df2['Rmittel (\u03A9)']
    rmittel_np = np.array(rmittel_array,dtype=np.float64)
    xs = np.array(distance_np, dtype=np.float64)
    ys = np.array(rmittel_np, dtype=np.float64)   
    ### ACI-RD0123G EXAMPLE DATA REPLACE # OF FINGERS WITH 11 ###
    #xs = np.array([0.89, 1.815, 2.74, 3.665, 4.59, 5.515, 6.44, 7.365, 8.29, 9.215, 10.14], dtype=np.float64)
    #ys = np.array([130.65, 252.225, 362.525, 475.575, 581.3, 665.975, 739.275, 791.175, 827.225, 861.075, 905.225], dtype=np.float64)
    m1, b1 = find_intercept(ys,xs)   
    m2, y_int = find_intercept(xs,ys)
    x_int = abs(b1)/num_fingers
    contact_resistance = (x_int/strip_width)*(y_int/strip_width)*(strip_width/num_fingers) # oh m-cm^2
    sheet_resistance = strip_width * m2
    dft = dft.round(3)
    sci_contact_resistance = "{:.3e}".format(contact_resistance)
    dft2 = pd.DataFrame([[x_int, y_int, sheet_resistance, sci_contact_resistance]],columns=['X Intercept','Y Intercept','Sheet Resistance (\u03A9/sq)', 'Contact Resistance (\u03A9-cm2)'], index=None)
    dft2 = dft2.round(4)
    dt = datetime.now().strftime('%I_%M_%p')
    with pd.ExcelWriter("C:/Users/PVRL-01/Documents/S24PROBE_Test_Results/Results_" + str(dt) +".xlsx") as writer:  
        df.to_excel(writer, sheet_name='DataResults')
        dft2.to_excel(writer, sheet_name='CalculatedResults')
    #PLOT GRAPH
    coef = np.polyfit(xs,ys,1)
    poly1d_fn = np.poly1d(coef) 
    correlation = np.corrcoef(xs, ys)[0,1]
    r2 = correlation**2 
    annotate_str = "y = " + str("%.4f" % round(m2, 4)) + "x + " + str("%.4f" % round(y_int, 4)) + "\n" + "R\u00b2 = " + str("%.4f" % round(r2, 4))
    ax3 = plt.subplot(121)  
    ax3.plot(xs,ys, 'bo', xs, poly1d_fn(xs), '--g',)     #'--k'=black dashed line, 'bo' = blue circle marker
    ax3.set_xlabel('Distance (mm)',fontsize= 14)
    ax3.set_xlim(left = 0, right=(num_fingers*1))
    ax3.set_ylabel('Rmittel (\u03A9)',fontsize= 14)
    ax3.set_ylim(bottom = 0)
    ax3.set_title("Contact Resistance Regression",fontsize= 20, fontweight = 'bold')
    ax3.grid(True)
    ax3.axis(True)
    bbox = dict(boxstyle ="round", fc ="0.8") 
    ax3.annotate(annotate_str,(6.1,125), bbox = bbox)       ## REGRESSION STATISTIC ANNOTATION 
    #define Table 1
    ax1 = plt.subplot(122)
    ax1.axis('off')
    ax1.axis('tight')
    ax1.margins(0) 
    #create Table 1
    table1 = ax1.table(cellText=dft.values, colLabels=dft.columns, rowLabels=dft.index, colLoc='center', loc='center')
    table1.auto_set_font_size(False)
    table1.set_fontsize(10)
    table1.scale(1.12,1.2)
    #define Table 2
    ax2 = plt.subplot(122)
    ax2.axis('off')
    ax2.axis('tight')
    ax2.margins(0) 
    #create Table 2
    table2 = ax2.table(cellText=dft2.values, colLabels=dft2.columns, rowLabels=None, colLoc='center', loc='lower right', cellColours= [['#A49665','#A49665','#A49665','#A49665']], colColours= ['#A49665','#A49665','#A49665','#A49665'])
    table2.auto_set_font_size(False)
    table2.set_fontsize(8)
    table2.scale(1.22,1.2)
    # Display Figures
    plt.subplots_adjust(wspace=0.3, left= 0.075, right = 0.95, bottom = 0.075)
    plt.show()

def find_intercept(xs,ys):
    m = (((np.mean(xs)*np.mean(ys)) - np.mean(xs*ys)) / ((np.mean(xs)*np.mean(xs)) - np.mean(xs*xs)))
    b = np.mean(ys) - m*np.mean(xs)
    return m,b      

if __name__ == "__main__":
  app = App()
  app.mainloop()
  
  # PYTHON TO EXE line command below
  # pyinstaller gui.py --onefile --windowed 
