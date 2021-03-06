// IllumioCodingSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

//creates an unordered_map whose key-value pairs represent an input address-port pair and an output address-port pair to translate to
std::unordered_map<std::string, std::string> createNATMap(std::string fileLoc) {
	std::unordered_map<std::string, std::string> NATMap;
	std::fstream file;
	file.open(fileLoc.c_str(), std::fstream::in);

	//do not proceed if file failed to open
	if (file.is_open()) {
		std::string line;

		//read in file line by line
		while (file >> line) {
			//find comma, then split string into input and output addresses
			int commaLoc = line.find(",");
			std::string input = line.substr(0, commaLoc);
			std::string output = line.substr(commaLoc + 1);

			//insert pair mapping into unordered_map
			std::pair<std::string, std::string> mapping(input, output);
			NATMap.insert(mapping);
		}
		return NATMap;
	}
	else {
		//used the answer to this question https://stackoverflow.com/questions/6248404/c-exceptions-is-throwing-c-string-as-an-exception-bad
		//to throw a runtime error
		throw std::runtime_error("Could not open NAT map file " + fileLoc);
	}
}

void mapFlows(std::string inputFileLoc, std::string outputFileLoc, std::unordered_map<std::string, std::string> map) {
	std::fstream inFile, outFile;
	inFile.open(inputFileLoc.c_str(), std::fstream::in);
	outFile.open(outputFileLoc.c_str(), std::fstream::out);

	//do not proceed if file failed to open
	if (inFile.is_open() && outFile.is_open()) {
		std::string line;

		//read file line by line
		while (inFile >> line) {

			//check if exact address has a nat mapping
			std::unordered_map<std::string, std::string>::const_iterator lookup = map.find(line);

			//if no nat mapping exists for exact address
			if (lookup == map.end()) {
				std::string potentialMapping;

				//split into ip and port
				int colonLoc = line.find(":");
				std::string ip = line.substr(0, colonLoc);
				std::string port = line.substr(colonLoc + 1);

				//check if mapping exists for any ip address and exact port
				potentialMapping = "*:" + port;
				lookup = map.find(potentialMapping);
				if (lookup == map.end()) {

					//check if mapping exists for exact ip address and any port
					potentialMapping = ip + ":*";
					lookup = map.find(potentialMapping);
				}
			}

			//if any mapping was found for the address, print it
			if (lookup != map.end()) {
				outFile << line << " -> " << lookup->second << std::endl;
			}
			else {
				outFile << "No nat match for " << line << std::endl;
			}
		}
	}
	else {
		//same as above exception throw
		if (!inFile.is_open()) {
			throw std::runtime_error("Could not open flow file " + inputFileLoc);
		}
		else {
			throw std::runtime_error("Could not open output file " + outputFileLoc);
		}
	}
}

int main()
{

	//queries the user for the locations of each file in their filesystem
	std::string NATMapFileLoc;
	std::string flowFileLoc;
	std::string outputFileLoc;
	std::cout << "Enter the path of the NAT map file:" << std::endl;
	std::cin >> NATMapFileLoc;
	std::cout << "Enter the path of the flow file:" << std::endl;
	std::cin >> flowFileLoc;
	std::cout << "Enter the location for the output file:" << std::endl;
	std::cin >> outputFileLoc;

	try {
		std::unordered_map<std::string, std::string> map = createNATMap(NATMapFileLoc);
		mapFlows(flowFileLoc, outputFileLoc, map);
		std::cout << "Results written to " << outputFileLoc << std::endl;
	}
	catch (const std::exception& ex) {
		//also used that stackoverflow question to catch the error
		std::cerr << ex.what() << std::endl;
	}

    return 0;
}