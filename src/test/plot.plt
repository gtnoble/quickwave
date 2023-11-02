script=ARG1
input_filename=ARG2
output_filename=ARG3

load(script)

set datafile separator ","
set term pdf
set output output_filename
plot for [col=int(start_column):int(end_columnn)] input_filename using 0:col with lines title columnhead