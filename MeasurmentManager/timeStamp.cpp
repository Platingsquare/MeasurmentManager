/* #include <iostream>
#include <ctime>


std::string GetCurrTstamp() {
    time_t now;
    time(&now);
    std::string ts = ctime(&now);
    return ts;
}





int main() {





    std::string timestamp = GetCurrTstamp();
    std::cout << "Time atm: " << timestamp << '\n'; // efter timestamp s� kan man l�gga in data kopplat till tiden

    /*time_t timestamp;
    time(&timestamp);
    std::cout << ctime(&timestamp);

    return 0;
} */