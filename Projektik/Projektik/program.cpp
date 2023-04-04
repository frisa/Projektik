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
		enPORT_SOURCE,
		enPORT_DESTINATION,
		enSTART_TIME,
		enEND_TIME,
		enDURATION,
		enDNS,
		enSOURCE_COUNTRY,
		enDESTINATION_COUNTRY,
	};

	unsigned int Bytes;
	unsigned int Flows;
	unsigned int BitsPerSecond;
	unsigned int PacketsPerSecond;
	unsigned int Packets;
	std::string IpSource;
	std::string IpDestination;
	unsigned int PortSource;
	unsigned int PortDestination;
	std::string StartTime;
	std::string EndTime;
	unsigned int Duration;
	unsigned int DnsQuestionCount;
	unsigned int CountrySource;
	unsigned int CountryDestination;
};

typedef std::vector<std::string> tHeader;
typedef std::vector<TransferRecord> tData;

class CsvReader {

private: // this is just for information all members of classes are private by default
	unsigned int stringToInteger(std::string value)
	{
		// the handling here needs to be improved for case the value cannot be converted to integer
		int ret{ 0 };
		try {
			ret = std::stoi(value);
		}
		catch (std::invalid_argument const& ex){
			std::cout << "The value: " << value << " cannto be converted to integer (" << ex.what() << ")" << std::endl;
		}
		catch (std::out_of_range const& ex) {
			std::cout << "The value " << value << " is out of range (" << ex.what() << ")" << std::endl;
 		}
		return ret;
	}
	std::string stringToIp(std::string value)
	{
		// the handling here needs to be improved for case the value cannot be converted to IP
		return value;
	}

	tHeader parseHeader(std::istream& inHeader)
	{
		tHeader ret;
		std::string line;
		std::string cell;

		std::getline(inHeader, line);
		std::stringstream inLine(line);
		TransferRecord row;
		unsigned int colIdx{ TransferRecord::ColumnIndexes::enBYTES };

		while (std::getline(inLine, cell, ';'))
		{
			ret.push_back(cell);
		}
		return ret;
	}
	TransferRecord parseLine(std::istream& inData)
	{
		std::string line;
		std::string cell;

		std::getline(inData, line);
		std::stringstream inLine(line);
		TransferRecord row;
		unsigned int colIdx{ TransferRecord::ColumnIndexes::enBYTES };

		while (std::getline(inLine, cell, ';'))
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
			case TransferRecord::ColumnIndexes::enPACKETS:
			{
				row.Packets = stringToInteger(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enIP_SOURCE:
			{
				row.IpSource = stringToIp(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enIP_DESTINATION:
			{
				row.IpDestination = stringToIp(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enPORT_SOURCE:
			{
				row.PortSource = stringToInteger(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enPORT_DESTINATION:
			{
				row.PortDestination = stringToInteger(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enSTART_TIME:
			{
				row.StartTime = cell;
				break;
			}
			case TransferRecord::ColumnIndexes::enEND_TIME:
			{
				row.EndTime = cell;
				break;
			}
			case TransferRecord::ColumnIndexes::enDURATION:
			{
				row.Duration = stringToInteger(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enDNS:
			{
				row.DnsQuestionCount = stringToInteger(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enSOURCE_COUNTRY:
			{
				row.CountrySource = stringToInteger(cell);
				break;
			}
			case TransferRecord::ColumnIndexes::enDESTINATION_COUNTRY:
			{
				row.CountryDestination = stringToInteger(cell);
				break;
			}
			default:
			{
				std::cout << "Column does not fit the schema" << std::endl;
			}
			}
			colIdx++;
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

	bool loadFile(const std::string& filePath, std::vector<TransferRecord>& outputData, tHeader& outputHeader)
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

		if (std::getline(csvFile, line))
		{
			std::stringstream lineStream(line);
			outputHeader = parseHeader(lineStream);
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

class DataChecker {
public:
	bool checkMissingRecords(const tData& data) {
		return true;
	}
};

class DataAnalyzer {
public:
	unsigned int calcOneWayTransferInTime(int timeInterval, const tData& data) {
		return 0;
	}
	unsigned int calcTwoWaysTransferInTime(int timeInterval, const tData& data) {
		return 0;
	}
	unsigned int calcOneWayTransfer(const tData& data) {
		return 0;
	}
	unsigned int calcTwoWaysTransfer(const tData& data) {
		return 0;
	}
};

class DataPrinter {
	void printData(tData data)
	{
		for (const auto& itm : data)
		{
			std::cout << itm.Bytes << "\t";
			std::cout << itm.Flows << "\t";
			std::cout << itm.BitsPerSecond << "\t";
			std::cout << itm.PacketsPerSecond << "\t";
			std::cout << itm.Packets << "\t";
			std::cout << itm.IpSource << "\t";
			std::cout << itm.IpDestination << "\t";
			std::cout << itm.PortSource << "\t";
			std::cout << itm.PortDestination << "\t";
			std::cout << itm.StartTime << "\t";
			std::cout << itm.EndTime << "\t";
			std::cout << itm.Duration << "\t";
			std::cout << itm.DnsQuestionCount << "\t";
			std::cout << itm.CountrySource << "\t";
			std::cout << itm.CountryDestination;
			std::cout << std::endl;
		}
	}
	void printHeader(tHeader header)
	{
		for (const auto& itm : header)
		{
			std::cout << itm << "\t";
		}
		std::cout << std::endl;
	}
public:
	void print(const tHeader& header, const tData& data)
	{
		printHeader(header);
		printData(data);
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
	
	// Read data from the CSV file to the structure
	CsvReader csvReader;
	tData csvData;
	tHeader csvHeader;
	csvReader.loadFile(filePath, csvData, csvHeader);
	std::cout << "Read " << csvData.size() << " records form the file: " << filePath << std::endl;
	
	// Print the result which has been read form the input file
	DataPrinter printer;
	printer.print(csvHeader, csvData);

	// Check the data for corectness
	DataChecker dataChecker;
	bool areMissingRecords = false;
	areMissingRecords = dataChecker.checkMissingRecords(csvData);
	std::cout << "Missing records: " << areMissingRecords << std::endl;

	// Analyse the data
	DataAnalyzer dataAnalyzer;
	int oneWayTime = dataAnalyzer.calcOneWayTransfer(csvData);
	std::cout << "Calculated one way transfer time: " << oneWayTime << std::endl;

	return 0;
}