#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std::literals;

struct CommandAdd {
    int id_person;
    size_t page;
};

class BookBase {
public:
    void UpdateBase(CommandAdd command_add) {
        size_t start_page = 0;
        if (people_page_.count(command_add.id_person) != 0) {
            start_page = people_page_.at(command_add.id_person) + 1;
        }
        
        people_page_[command_add.id_person] = command_add.page;
        if (progress_count_people_.size() - 1 < command_add.page) {
            progress_count_people_.resize(command_add.page + 1);
        }
        for (; start_page < command_add.page + 1; ++start_page) {
            progress_count_people_[start_page] += 1;
        }
    }

    void GetInfo(int id, std::ostream& output) {
        double part_losing_peoples = 0;
        
        if (people_page_.count(id) != 0) {
            if (people_page_.size() == 1) {
                part_losing_peoples = 1;
            }
            else { 
                part_losing_peoples = static_cast<double>(people_page_.size() - progress_count_people_[people_page_.at(id)])
                                      / static_cast<double>(people_page_.size() - 1);
            }
        }
        
        output << part_losing_peoples << std::endl;
    }


private:
    std::unordered_map<int, size_t> people_page_;
    std::vector<size_t> progress_count_people_ = {0};
};

CommandAdd ParseAddCommand(std::string description) {
    auto space_pos = description.find(' ');
    return {std::stoi(description.substr(0, space_pos)),
            std::stoul(description.substr(space_pos + 1, description.size() - 1))};
}


void ProcessRequest (std::istream& input, std::ostream& output, BookBase book) {
    int base_request_count;
    input >> base_request_count >> std::ws;
    for (int i = 0; i < base_request_count; ++i) {
        std::string line;
        std::getline(input, line);
        auto space_pos = line.find(' ');
        std::string command = line.substr(0, space_pos);
        std::string description = line.substr(space_pos + 1, line.size() - 1);
        if (command == "CHEER"s) {
            book.GetInfo(std::stoi(description), output);
        }
        else if (command == "READ"s) {
            book.UpdateBase(ParseAddCommand(move(description)));
        }
        else {
            output << "Unknown request" << std::endl;
        }
    }
}


int main() {
    BookBase book;
    ProcessRequest (std::cin, std::cout, book);
}