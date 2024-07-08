import pyvisa 
path = "<C:>\Users\Documents\S24PROBE\CODE"
def main():
    get_measurement_keithley_2401()

def get_measurement_keithley_2401():
    rm = pyvisa.ResourceManager(path)
    rm.list_resources()
    # Open Connection to Meter
    keithley = rm.open_resource('GPIB0::11::INSTR')
    print(keithley.query('*IDN?'))
        
    keithley.write('*RST')
    keithley.write('*CLS')
    
    keithley.write('CONF:VOLT:DC')

    interval_in_ms = 500
    number_of_readings = 10

    keithley.write("status:measurement:enable 512; *sre 1")
    keithley.write("sample:count %d" % number_of_readings)
    keithley.write("trigger:source bus")
    keithley.write("trigger:delay %f" % (interval_in_ms / 1000.0))
    keithley.write("trace:points %d" % number_of_readings)
    keithley.write("trace:feed sense1; feed:control next")


    voltages = keithley.query_ascii_values("trace:data?")
    print("Average voltage: ", sum(voltages) / len(voltages))

    keithley.query("status:measurement?")
    keithley.write("trace:clear; feed:control next")

    
if __name__== "__main__":
    get_measurement_keithley_2401()
     
