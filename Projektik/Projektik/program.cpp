#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

struct TransferRecord {
	enum ColumnIndexes {
		enBYTES,
		enFLOWS,
		enBITS_PER_SECOND,
		enPACKETS_PER_SECOND,
		enPACKETS,
		enIP_SOURCE,
		enIP_DESTINATION,
	};

	unsigned int Bytes;
	unsigned int Flows;
	unsigned int BitsPerSecond;
	unsigned int PacketsPerSecond;
	unsigned int Packets;
	unsigned int IpSource;
	unsigned int IpDestination;
	unsigned int PortSource;
	unsigned int PortDestination;
	unsigned int StartTime;
	unsigned int EndTime;
	unsigned int Duration;
	unsigned int DnsQuestionCount;
	unsigned int CountrySource;
	unsigned int CountryDestination;
};

class CsvReader {

private: // this is just for information all members of classes are private by default
	unsigned int stringToInteger(std::string value)
	{
		// the handling here needs to be improved for case the value cannot be converted to integer
		return std::stoi(value);
	}

	void parseHeader(std::istream& inData)
	{
		// lets do somethink with header
		return;
	}
	TransferRecord parseLine(std::istream& inData)
	{
		std::string line;
		std::string cell;

		std::getline(inData, line);
		std::stringstream inLine(line);
		TransferRecord row;
		unsigned int colIdx{ TransferRecord::ColumnIndexes::enBYTES };

		while (std::getline(inLine, cell, ','))
		{
			TransferRecord::ColumnIndexes enColumns = static_cast<TransferRecord::ColumnIndexes>(colIdx);

			switch(enColumns)
			{
			case TransferRecord::ColumnIndexes::enBYTES:
			{
				row.Bytes = stringToInteger(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enFLOWS:
			{
				row.Flows = stringToInteger(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enBITS_PER_SECOND:
			{
				row.BitsPerSecond = stringToInteger(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enPACKETS_PER_SECOND:
			{
				row.PacketsPerSecond = stringToInteger(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enIP_SOURCE:
			{
				row.IpSource = stringToInteger(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enIP_DESTINATION:
			{
				row.IpDestination = stringToInteger(cell);
				break;
			}
			// the swtich needs to be extended by other columns
			default:
			{
				std::cout << "The amount of columns does not fit the data structure" << std::endl;
			}
			}
		}
		return row;
	}

	bool fileExist(std::string filePath)
	{
		std::filesystem::path filePathEx{ filePath }; // this is from the C++ 17
		if (std::filesystem::exists(filePathEx))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

public:

	bool loadFile(const std::string& filePath, std::vector<TransferRecord>& outputData)
	{
		if (!fileExist(filePath))
		{
			std::cout << "The file does not exists" << std::endl;
			return false;
		}

		std::fstream csvFile;
		csvFile.open(filePath);

		if (!csvFile.is_open())
		{
			std::cout << "The file: " << filePath << " cannot be open" << std::endl;
			return false;
		}

		if (csvFile.fail())
		{
			std::cout << "The file: " << filePath << " faild by ipening" << std::endl;
			csvFile.close();
			return false;
		}
				
		std::string line;
		// handle the header of the CSV file
		if (std::getline(csvFile, line))
		{
			std::stringstream lineStream(line);
			parseHeader(lineStream);
		}

		while (std::getline(csvFile, line))
		{
			TransferRecord row;
			std::stringstream lineStream(line);
			row = parseLine(lineStream);
			outputData.push_back(row);
		}
		return true;
	}
};

class DataVerifier {
	enum class DataState {
		enOK,
		enDATA_INCOMPLETE,
		enDATA_INVALID,
	};
};

class DataAnalyzer {
	unsigned int calcOneWayTransferInTime(int timeInterval) {
		return 0;
	}
	unsigned int calcTwoWaysTransferInTime(int timeInterval) {
		return 0;
	}
	unsigned int calcOneWayTransfer() {
		return 0;
	}
	unsigned int calcTwoWaysTransfer(){
		return 0;
	}
};

int main(int argc, char** argv)
{
	// Check if proper amount of arguments has been used by calling the application
	// first argument is the application name
	// second argument is the csv file path
	if (argc < 2)
	{
		std::cout << "No arguments has been passed" << std::endl;
		std::cout << "Use: program <csv file name> " << std::endl;
		return 0;
	}
	
	std::string filePath{ argv[1] };
	std::cout << "CSV file path: " << filePath << std::endl;
	
	CsvReader csvReader;
	std::vector<TransferRecord> csvData;

	csvReader.loadFile(filePath, csvData);

	return 0;
}