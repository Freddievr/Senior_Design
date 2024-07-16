import pyvisa
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from statistics import mean

def main():
    get_measurement_keithley_2401()

def get_measurement_keithley_2401():
    # Setup Connection to Meter
    rm = pyvisa.ResourceManager('@ivi')
    resources = rm.list_resources()
    print(resources)
    
    # Open Connection to Meter
    keithley = rm.open_resource('GPIB0::11::INSTR')
    print(keithley.query('*IDN?'))
    # Reset/Clear Status on Meter
    keithley.write('*RST')                  
    keithley.write('*CLS')
    # Setup VARIABLES 
    gap_width = 0.89            # in mm
    strip_width = 2             # in mm
    num_fingers = 6                 ## CHANGE ##                ###### NOT OVER 7??????
    numReadings = str(num_fingers*2)
    rowName = "Finger"
    
    
    # Setup Meter
    keithley.write(":SOUR:FUNC:MODE CURR")             # Select current source 
    # setup current sweep mode & points
    keithley.write(":SOUR:CURR:MODE LIST")
    keithley.write(":SOUR:LIST:CURR -0.020,0.020")     # Sweep points -20mA,20mA
    keithley.write(":SOUR:SWE:POIN " + numReadings)
    keithley.write(":SOUR:SWE:DIR UP")
    # turn off concurrent functions
    keithley.write(":SENSE:FUNC:CONC OFF")
    keithley.write(":SENSE:CURR:PROT:RSYN ON")  
    # 0ms Trigger delay
    keithley.write(":TRIG:DEL 0.0")
    # 0ms Source delay
    keithley.write(":SOUR:DEL 0.0")
    # set current complaince to 10x10^-3 (10mA)
    keithley.write(":SENSE:CURR:PROT 10e-3")
    keithley.write(":SYST:RSEN ON")                #4 point probe
    # Disable auto-zero state
    keithley.write(":SYSTEM:AZERO:STAT OFF")
    # ??? clear trace buffer?
    keithley.write(":TRAC:CLE")
    
    # store n readings in buffer
    keithley.write(":TRAC:POINTS " + numReadings)
    # set ARM count n=1
    keithley.write(":ARM:COUN " + numReadings)
    # enable buffer for trace?
    keithley.write(":TRAC:FEED:CONT NEXT")
    # of sweep points n
    keithley.write("TRIG:COUN " + numReadings)
    
    # turn on output
    keithley.write(":OUTPUT ON")
    # trigger readings
    keithley.write(":INIT")

    inputs = keithley.query_ascii_values("trace:data?")
  
    values = np.array(inputs)
    soa = np.size(values)       # SIZE OF ARRAY
    values = values.reshape(-1, 10)
    #print(values)

    # setup main dataframe
    col = ['Voltage-','Current-','Resistance1','Time1','Status1', 'Voltage+','Current+','Resistance2','Time2','Status2']
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
    df['Distance'] = distance_array
    df['R+'] = df['Voltage+'] / df['Current+'] 
    df['R-'] = df['Voltage-'] / df['Current-'] 
    df['Rmittel'] = (df['R+'] - df['R-'])/2
    # Setup Temp DATAFRAME FOR CALCULATIONS
    df2 = pd.DataFrame()
    df2.loc[:,'Distance'] = df.loc[:,'Distance']
    df2.loc[:,'Rmittel'] = df.loc[:,'Rmittel']
    df2.plot.line(x='Distance', y='Rmittel')
    # PLOT GRAPH
    plt.title("Contact Resistance Regression")
    plt.xlabel("Distance (mm)")
    plt.ylabel("Resistance (\u03A9)")
    plt.grid(True)
    plt.show()
    # CALCULATE INTERCEPTS
    rmittel_array = df2['Rmittel'].to_numpy()
    xs = np.array(distance_array, dtype=np.float64)
    ys = np.array(rmittel_array, dtype=np.float64)
    # print(xs,ys)      ACI-RD0123G EXAMPLE DATA REPLACE # OF FINGERS WITH 11
    #xs = np.array([130.65, 252.225, 362.525, 475.575, 581.3, 665.975, 739.275, 791.175, 827.225, 861.075, 905.225], dtype=np.float64)
    #ys = np.array([0.89, 1.815, 2.74, 3.665, 4.59, 5.515, 6.44, 7.365, 8.29, 9.215, 10.14], dtype=np.float64)
    m1, b1 = intercept(xs,ys)   
    m2, y_intercept = intercept(ys,xs)
    x_intercept = abs(b1)/num_fingers
    contact_resistance = ((x_intercept/(strip_width/10))*(y_intercept/(strip_width/10))*((strip_width/10)/num_fingers)) # ohm-cm^2
    
    x_y_cr = [x_intercept, y_intercept, contact_resistance]
    print(df)
    print(x_y_cr)
    return df   

def intercept(xs,ys):
    m = (((mean(xs)*mean(ys)) - mean(xs*ys)) 
         ((mean(xs)*mean(xs)) - mean(xs*xs)))
    
    b = mean(ys) - m*mean(xs)
    return m, b

if __name__== "__main__":
    get_measurement_keithley_2401()
     
