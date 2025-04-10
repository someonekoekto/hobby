#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <thread>
#include <future>
#include <mutex>
#include <fstream>
#include <unordered_set>
#include <regex>
#include <array>
#include <optional>
#include <map>

using namespace std;


std::array<std::regex, 6> regexes{
	std::regex{ "[A-Z]{3}[0-9]{1,4}" },
	std::regex{ "[A-Z]{2} [0-9]{1,4}" },
	std::regex{ "[A-Z]{2}[0-9]{1,5}" },
	std::regex{ "(([A-Z][0-9]|[A-Z]) [0-9]{1,4}|([A-Z]|[0-9][A-Z]) [0-9]{1,4})"},
	std::regex{ "(([A-Z][0-9]|[A-Z])[0-9]{1,5}|([A-Z]|[0-9][A-Z])[0-9]{1,5})" },
	std::regex{ "[0-9]{1,5}" }
};

std::pair<std::string, int> getPartValue(const std::string& ticket) {
		if (std::regex_match(ticket, regexes[0])) {
			return { {ticket.begin(), ticket.begin() + 3}, stoi(ticket.substr(3,string::npos))};
		} else if (std::regex_match(ticket, regexes[1])) {
			return { {ticket.begin(), ticket.begin() + 2}, stoi(ticket.substr(3,string::npos)) };
		} else if (std::regex_match(ticket, regexes[2])) {
			return { {ticket.begin(), ticket.begin() + 2}, stoi(ticket.substr(2,string::npos)) };
		} else if (std::regex_match(ticket, regexes[3])) {
			return { {ticket.begin(), ticket.begin() + 2}, stoi(ticket.substr(3,string::npos)) };
		} else if (std::regex_match(ticket, regexes[4])) {
			return { {ticket.begin(), ticket.begin() + 2}, stoi(ticket.substr(2,string::npos)) };
		} else if (std::regex_match(ticket, regexes[5])) {
			return { {}, stoi(ticket.substr(0,string::npos)) };
		} else {
			throw std::logic_error{ "incorrect ticket " + ticket };
		}
}

bool isTheSame(std::string& ticket1, std::string& ticket2) {
	try
	{
		return getPartValue(ticket1) == getPartValue(ticket2);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << "\n";
		return false;
	}
}

template <>
struct std::hash<pair<string, int>>
{
	std::size_t operator()(const pair<string, int>& k) const
	{
		return ((hash<string>()(k.first) ^ (hash<int>()(k.second) << 1)) >> 1);
	}
};


void copyAndDeleteDuplicate(const string& inputFileName, const string& outputFileName) {
	try
	{
		std::ifstream iFile(inputFileName);
		std::ifstream iiFile(inputFileName);
		std::ofstream oFile(outputFileName);
		string line;


		std::unordered_map<pair<string, int>, std::vector<std::string>> ticketsCollection; // first - ticket, second - freq
		while (getline(iFile, line)) {
			std::pair<string, int> parseTicket;
			try {
				parseTicket = getPartValue(line);
			}
			catch (std::exception& ex){
				std::cout << ex.what() << "\n";
				std::cout << "ignore this ticket\n";
				continue;
			}
            ticketsCollection[parseTicket].push_back(line);
		}
        for(const auto& curTicket: ticketsCollection) {
            if(curTicket.second.size() == 1) {
                oFile << curTicket.second[0] << "\n";
            }
        }

	}
	catch (const std::exception & ex)
	{
		std::cout << "maybe cannot open someone file " << ex.what() << "\n";
	}
}


int main()
{

	
	// task 1
	std::vector<std::string> arrayOfTest{"A3 0525", "ALF00001","A3525","49", "34 153", "3452", "0F29483"};
	std::cout << " ======= Tiny tests ======= \n";

	for (auto& leftTicket : arrayOfTest) {
		for (auto& rightTicket : arrayOfTest) {
			auto el = isTheSame(leftTicket, rightTicket);
			std::cout << "isn't the same?  ---- " << leftTicket << " ---- "
				<< rightTicket << " ?      Answer - " << el << "\n\n";
		}
	}

	std::cout << "   ======= END ======= \n\n";


	// task 2
	string files[]{"1_in.txt", "1_out.txt","2_in.txt", "2_out.txt" };
	std::future<void> fut1 = std::async(std::launch::async, copyAndDeleteDuplicate,files[0], files[1]);
	copyAndDeleteDuplicate(files[2], files[3]);
	fut1.wait();

	return 0;
}
