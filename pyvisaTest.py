import pyvisa

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
    # Reset/Clear Meter
    keithley.write('*RST')                  
    keithley.write('*CLS')
    # Setup Meter
    interval_in_ms = 500
    number_of_readings = 10
    
    keithley.write(":SOUR:FUNC:MODE CURR")             # Select current source 
    keithley.write(":SOUR:LIST:CURR -0.020,0.020")     # Sweep points -20mA,20mA
    # 0ms Trigger delay
    keithley.write(":TRIG:DEL 0.0")
    # 0ms Source delay
    keithley.write(":SOUR:DEL 0.0")
    # set current complaince to 10x10^-3 (10mA)
    keithley.write(":SENSE:CURR:PROT 1e-3")
    # ??? clear trace buffer?
    keithley.write(":TRAC:CLE")
    # store n readings in buffer
    keithley.write(":TRAC:POINTS 2")
    # enable buffer for trace?
    keithley.write(":TRAC:FEED:CONT NEXT")
    # of sweep points (2)
    keithley.write("TRIG:COUN 2")
    # turn on output
    keithley.write(":OUTPUT ON")
    # trigger readings
    keithley.write(":INIT")
    
    voltages = keithley.query_ascii_values("trace:data?")
    print(voltages)
    print("Average voltage: ", sum(voltages) / len(voltages))
    
    # # set ARM count n=1
    # keithley.write(":ARM:COUNT 1")
    # select 1V range
    # # turn off concurrent functions
    # keithley.write(":SENSE:FUNC:CONC OFF")

    # # Disable filter
    # keithley.write(":SENSE:AVERAGE:STAT OFF")
    # # Disable display circuitry
    # keithley.write(":DISP:ENAB OFF")
    # # Disable auto-zero state
    # keithley.write(":SYSTEM:AZERO:STAT OFF")



    # # list volts sweep mode
    # keithley.write(":SOUR:CURR:MODE LIST")
    # # turn on output
    # keithley.write(":OUTPUT ON")
    # # trigger readings
    # keithley.write(":INIT")
   
   
    #keithley.write("status:measurement:enable 512; *SRE 1") 
    #keithley.write("sample:count %d" %number_of_readings)
    #keithley.write("trigger:source bus")
    #keithley.write("trigger:delay %f" % (interval_in_ms / 1000.0))
    #keithley.write("trace:points %d" % number_of_readings)
   # keithley.write("trace:feed sense1; feed:control next")
'''

    voltages = keithley.query_ascii_values("trace:data?")
    print("Average voltage: ", sum(voltages) / len(voltages))

    keithley.query("status:measurement?")
    keithley.write("trace:clear; feed:control next") 
'''
    
if __name__== "__main__":
    get_measurement_keithley_2401()
     
