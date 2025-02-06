#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

// Function to read authentication details from a file
std::pair<std::string, std::string> readAuthDetails(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open authentication file");
    }

    std::string username, password;
    std::getline(file, username, ':');
    std::getline(file, password);

    if (username.empty() || password.empty()) {
        throw std::runtime_error("Invalid authentication file format");
    }

    return { username, password };
}

// Function to authenticate the user
bool authenticateUser(const std::string& username, const std::string& password, const std::pair<std::string, std::string>& authDetails) {
    return username == authDetails.first && password == authDetails.second;
}

// Function to check if a URL is blocked
bool isUrlBlocked(const std::vector<std::string>& blockedUrls, const std::string& url) {
    for (const std::string& blockedUrl : blockedUrls) {
        if (url == blockedUrl) {
            return true;
        }
    }
    return false;
}

// Function to test the readAuthDetails function
void testReadAuthDetails() {
    try {
        auto authDetails = readAuthDetails("auth.cfg");
        assert(authDetails.first == "admin");
        assert(authDetails.second == "secret");
    }
    catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        assert(false);
    }

    std::cout << "readAuthDetails test passed!" << std::endl;
}

// Function to test the authenticateUser function
void testAuthenticateUser() {
    auto authDetails = readAuthDetails("auth.cfg");
    assert(authenticateUser("admin", "secret", authDetails) == true);
    assert(authenticateUser("admin", "wrongpassword", authDetails) == false);
    assert(authenticateUser("wronguser", "secret", authDetails) == false);

    std::cout << "authenticateUser test passed!" << std::endl;
}

// Function to test the isUrlBlocked function
void testIsUrlBlocked() {
    std::vector<std::string> blockedUrls = { "www.badsite.com", "www.anotherbadsite.com" };

    assert(isUrlBlocked(blockedUrls, "www.badsite.com") == true);
    assert(isUrlBlocked(blockedUrls, "www.anotherbadsite.com") == true);
    assert(isUrlBlocked(blockedUrls, "www.goodsite.com") == false);

    std::cout << "isUrlBlocked test passed!" << std::endl;
}

int main() {
    try {
        // Read authentication details
        auto authDetails = readAuthDetails("auth.cfg");

        // Authenticate the user
        std::string username, password;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        if (!authenticateUser(username, password, authDetails)) {
            std::cerr << "Authentication failed!" << std::endl;
            return 1;
        }

        std::vector<std::string> blockedUrls = { "www.badsite.com", "www.anotherbadsite.com" };

        // Ask the user to input a URL
        std::string userUrl;
        std::cout << "Please enter a URL: ";
        std::cin >> userUrl;

        // Check if the URL is blocked
        if (isUrlBlocked(blockedUrls, userUrl)) {
            std::cout << "The URL is blocked." << std::endl;
        }
        else {
            std::cout << "The URL is allowed." << std::endl;
        }

        // Run tests
        testReadAuthDetails();
        testAuthenticateUser();
        testIsUrlBlocked();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}