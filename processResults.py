# importing pandas library 
import pandas as pd 
  
# reading given csv file and creating dataframe 
file = pd.read_csv('results.csv', sep='/t', engine='python')
file.drop([0, 1, 2],inplace= True)                                          # Drop First Rows (Description/Titles)
file.drop(file.tail(10).index, inplace = True)                              # Drop Last Rows (Parameters)
file.rename(columns={'[[DESCRIPTION]]': 'Input'}, inplace = True)
lowerBound = file[file['Voltage'] < 1]

df = file['Input'].str.split(n=4, expand=True)                              # Split Results into Columns

df = df.drop(columns=[3, 4])                                                # Drop Unneeded Columns (Status)
df.rename(columns={0: 'Voltage', 1: 'Current', 2: 'Time'}, inplace = True)  # Rename Columns

# df["Resistance"] = ""

print(df) 

#list = df.values.tolist()
#print(list)
df.to_csv('testFile.csv', index=False)                                   # Convert DataFrame to xlsx (excel)