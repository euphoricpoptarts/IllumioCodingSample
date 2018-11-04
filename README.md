# IllumioCodingSample

This project takes 2 files (a nat map file and a flow file) as input and returns 1 as output.

The program will ask the user for the paths to each input file. The program will also ask the user the path to create the output file.
There is no default location.

The core concept of the implementation is the use of an unordered_map to map input ip address-port strings (the key) to output ip address-port strings (the value).
In order to map each flow to an output, the program checks for the existence of 3 key strings, the original flow string, the flow string with the ip address replaced with "*", and the flow string with the port replaced with "*".

I assumed that the NAT map file would not have multiple possible mappings for a given input.
I also assumed that no output for a NAT mapping would exist as an input in the NAT mapping (ie. a flat mapping structure).

The program will alert the user and quit if any of the 2 input files or the output file fails to open.
