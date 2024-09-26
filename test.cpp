#include <iostream>
#include <map>
#include <cmath>
#include <string>
#include <fstream>
#include <json/json.h>  // JSON parsing library (you'll need to use an equivalent library based on your language)

// Function to convert a string number in a given base to decimal (base 10)
long long decodeValue(std::string value, int base) {
    return std::stoll(value, 0, base);
}

// Function to perform Lagrange Interpolation and find the constant term (c)
double lagrangeInterpolation(std::map<int, long long>& points, int k) {
    double c = 0.0;

    // Iterate through each point (xi, yi)
    for (auto i = points.begin(); i != points.end(); ++i) {
        int xi = i->first;
        long long yi = i->second;

        double term = yi;
        // Calculate the Lagrange basis polynomial Li(x)
        for (auto j = points.begin(); j != points.end(); ++j) {
            if (j->first != xi) {
                term *= (0 - j->first) / static_cast<double>(xi - j->first);
            }
        }
        c += term;
    }

    return c;
}

int main() {
    // Open and parse the JSON file (use your own file path)
    std::ifstream file("testcase.json");
    Json::Value root;
    file >> root;

    // Extract the number of points (n) and required points (k)
    int n = root["keys"]["n"].asInt();
    int k = root["keys"]["k"].asInt();

    // Map to store the decoded (x, y) points
    std::map<int, long long> points;

    // Loop over the given points and decode the values based on the provided base
    for (int i = 1; i <= n; ++i) {
        std::string baseKey = std::to_string(i);
        if (root.isMember(baseKey)) {
            int x = i;
            int base = std::stoi(root[baseKey]["base"].asString());
            std::string value = root[baseKey]["value"].asString();

            // Decode the value based on the given base
            long long y = decodeValue(value, base);
            points[x] = y;
        }
    }

    // Ensure we have enough points for interpolation
    if (points.size() < k) {
        std::cerr << "Not enough points to solve the polynomial." << std::endl;
        return -1;
    }

    // Perform Lagrange interpolation to find the constant term 'c'
    double constantTerm = lagrangeInterpolation(points, k);

    // Output the constant term
    std::cout << "The constant term c is: " << constantTerm << std::endl;

    return 0;
}
