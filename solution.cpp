#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

// Structure to hold the test case data
struct TestCase {
    int n;
    int k;
    json data;
};

// Function to read and parse JSON test case
TestCase read_test_case(const std::string& file_path) {
    std::ifstream file(file_path);
    json data = json::parse(file);
    
    return TestCase{
        data["keys"]["n"],
        data["keys"]["k"],
        data
    };
}

// Function to convert string from any base to decimal
long long convert_base(const std::string& value, int base) {
    return std::stoll(value, nullptr, base);
}

// Function to decode points from JSON data
std::vector<std::pair<double, double>> decode_y_values(const json& data) {
    std::vector<std::pair<double, double>> points;
    
    // Iterate through items without using structured bindings
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (it.key() != "keys") {
            double x = std::stod(it.key());
            int base = std::stoi(it.value()["base"].get<std::string>());
            double y = convert_base(it.value()["value"], base);
            points.emplace_back(x, y);
        }
    }
    
    return points;
}

// Function to perform Lagrange interpolation and find constant term
double find_constant_term(const std::vector<std::pair<double, double>>& points) {
    double x = 0; // We want to evaluate at x = 0 for constant term
    double result = 0;
    int n = points.size();
    
    for (int i = 0; i < n; i++) {
        double term = points[i].second; // yi
        
        for (int j = 0; j < n; j++) {
            if (i != j) {
                // Calculate (x - xj)/(xi - xj)
                term *= (x - points[j].first) / 
                       (points[i].first - points[j].first);
            }
        }
        result += term;
    }
    
    return result;
}

int main() {
    try {
        TestCase test = read_test_case("test_case.json");
        
        auto points = decode_y_values(test.data);
        
        std::cout << "Points:" << std::endl;
    //    for (const auto& point : points) {
      //      std::cout << "(" << point.first << ", " << point.second << ")" << std::endl;
       // }
        
        double c = find_constant_term(points);
        
        std::cout << "Constant term: " << c << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
