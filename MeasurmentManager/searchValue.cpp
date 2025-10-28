/*void searchValue(const std::vector<double>& data)
{
    if (data.empty())
    {
        std::cout << "No measurements available to search.\n";
        return;
    }

    std::cout << "Enter value to search for: ";
    std::string line;
    if (!std::getline(std::cin, line)) return;

    double query;
    if (!tryParseDouble(line, query))
    {
        std::cout << "Invalid number.\n";
        return;
    }

    std::cout << "Enter tolerance (press Enter to continue): ";
    if (!std::getline(std::cin, line)) return;

    double eps = 1e-6;
    if (!line.empty())
    {
        if (!tryParseDouble(line, eps) || eps < 0.0)
        {
            std::cout << "Invalid epsilon. Using default 1e-6.\n";
            eps = 1e-6;
        }
    }

    bool any = false;
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        if (std::fabs(data[i] - query) <= eps)
        {
            if (!any)
            {
                std::cout << "Matches found:\n";
                any = true;
            }
            std::cout << "  Index " << i << ": " << data[i] << "\n";
        }
    }

    if (!any)
        std::cout << "No matches within epsilon = " << eps << ".\n";


        */