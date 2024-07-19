import pyvisa
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from statistics import mean
import os
from datetime import datetime

def main():
    get_measurement_keithley_2401()

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
    num_fingers = 11                ## CHANGE ##                ###### NOT OVER 7??????
    numReadings = str(num_fingers*2)
    rowName = "Finger"
    keithley.timeout = 5000 * num_fingers               ## timeout set up according to # of fingers
        
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
    #xs = np.array(distance_np, dtype=np.float64)
    #ys = np.array(rmittel_np, dtype=np.float64)
    #print(xs,ys)     # ACI-RD0123G EXAMPLE DATA REPLACE # OF FINGERS WITH 11
    xs = np.array([130.65, 252.225, 362.525, 475.575, 581.3, 665.975, 739.275, 791.175, 827.225, 861.075, 905.225], dtype=np.float64)
    ys = np.array([0.89, 1.815, 2.74, 3.665, 4.59, 5.515, 6.44, 7.365, 8.29, 9.215, 10.14], dtype=np.float64)
    m1, b1 = find_intercept(xs,ys)   
    m2, y_int = find_intercept(ys,xs)
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
    #df2.plot(x='Distance (mm)', y='Rmittel (\u03A9)', layout=(1,3), subplots=True, xlabel = "Distance (mm)", ylabel = "Resistance (\u03A9)",kind = 'line', title = "Contact Resistance Regression", grid = True)        
    coef = np.polyfit(ys,xs,1)
    poly1d_fn = np.poly1d(coef) 
    correlation = np.corrcoef(ys, xs)[0,1]
    r2 = correlation**2 
    annotate_str = "y = " + str("%.4f" % round(m2, 4)) + "x + " + str("%.4f" % round(y_int, 4)) + "\n" + "R\u00b2 = " + str("%.4f" % round(r2, 4))
    ax3 = plt.subplot(121)  
    ax3.plot(ys,xs, 'bo', ys, poly1d_fn(ys), '--g',)     #'--k'=black dashed line, 'bo' = blue circle marker
    ax3.set_xlabel('Distance (mm)',fontsize= 14)
    ax3.set_ylabel('Rmittel (\u03A9)',fontsize= 14)
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

if __name__== "__main__":
    get_measurement_keithley_2401()
     
