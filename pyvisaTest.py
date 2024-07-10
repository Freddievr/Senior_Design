import pyvisa
import pandas as pd
import numpy as np

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
    # Setup Meter 
    num_fingers = 6
    numReadings = str(num_fingers*2)
    rowName = "Finger"
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
    soa = np.size(values)
    values = values.reshape(-1, 10)
    #print(values)

    col = ['Voltage-','Current-','Resistance1','Time1','Status1', 'Voltage+','Current+','Resistance2','Time2','Status2']
   
    df = pd.DataFrame(values,columns=col)
    df.drop(columns = ['Resistance1','Time1','Status1','Resistance2','Time2','Status2'], inplace = True)   
    row_array = []
    for i in range(0, num_fingers, 1) :
        row_array.append("Finger_" + str(i+1))
    df.index = row_array   
    print(df)
    
    
    
    
    
    
    
    
    ###########################################################################
    
    # average_voltage = sum(voltages) / len(voltages)
    # print("Average voltage: ", average_voltage)
    

    
    #print(keithley.query(":SOUR:LIST:COUN?"))     # Sweep points -20mA,20mA
    # turn off output
    #keithley.write(":OUTPUT OFF")
    #keithley.write("trace:clear; feed:control next")
    
    # # Disable filter
    # keithley.write(":SENSE:AVERAGE:STAT OFF")
    # # Disable display circuitry
    # keithley.write(":DISP:ENAB OFF")

    # # list volts sweep mode
    # keithley.write(":SOUR:CURR:MODE LIST")
    # # turn on output
    # keithley.write(":OUTPUT ON")
    # # trigger readings
    # keithley.write(":INIT")
 

if __name__== "__main__":
    get_measurement_keithley_2401()
     
