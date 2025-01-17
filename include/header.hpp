#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <iostream>
#include "../third-party/PicoSHA2/picosha2.h"
#include <string>
#include <chrono>
#include <thread>
#include <csignal>
#include <nlohmann/json.hpp>
#include <mutex>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <sstream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>

using time_point = std::chrono::system_clock::time_point;
using high_clock = std::chrono::high_resolution_clock;
using std::cout;
using std::endl;
using string = std::string;
using json = nlohmann::json;
using boost::shared_ptr;

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;

static const string Hex_End = "0000";

class Hasher {
public:
    Hasher(int argc, char *argv[]);

    ~Hasher();

    void static Catch_Signal(int signum){
        Write_To_Json_File("hash_hex_log.json");
        sleep(1);
        std::cout << "\nProgram aborted with code " << --signum << "\n";
        exit(signum);
    }

    void static Write_To_Json_File(const string& file_name){
        std::ofstream output_file;
        std::ifstream input_file;
        output_file.open(file_name);
        input_file.open(file_name);
        json out_json;
        if(input_file.peek() != EOF)
            input_file >> out_json;
        for(auto & right_hash : right_hashs) {
            out_json["values"].push_back(right_hash);
        }
        output_file << out_json.dump(4);
        output_file.close();
    };

    void Start(const bool& key);

    void Sort_Hash(string& hash);

    void Hashing(const bool& key);
private:
    std::mutex mutex;
    unsigned int number_threads;
    long duration;
    string src_str;
    string hash_hex_str;
    std::vector<std::thread> threads;
    static std::vector<json> right_hashs;
    src::severity_logger< severity_level > slg;
};

#endif // INCLUDE_HEADER_HPP_
