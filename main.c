//This is the EECS 581 "Extracting IPv4 Addresses from Noisy Text" assignment. In the part where I commented "my code" is where I wrote some of it while AI completed the rest of that part of the code.
//Name: Maha Jornaz
//Date: 09/27/2026

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

bool extractIPv4(const string& str, unsigned long& outAddress, int& outPort)
{
    outAddress = 0;
    outPort = -1;

    size_t i = 0;

    while (i < str.length())
    {
        if (!isdigit(static_cast<unsigned char>(str[i])))
        {
            if ((str[i] == '.' || str[i] == ':') &&
                i + 1 < str.length() &&
                isdigit(static_cast<unsigned char>(str[i + 1])))
            {
                i++;

                while (i < str.length() &&    //my code
                       (isdigit(static_cast<unsigned char>(str[i])) ||  //AI's code
                        str[i] == '.' ||   //my code
                        str[i] == ':'))    //my code
                {
                    i++;         //AI's code
                }

                continue;   //my code
            }

            i++;      //my code
            continue;  //my code
        }

        if (i > 0 && (str[i - 1] == '.' || str[i - 1] == ':'))
        {
            while (i < str.length() &&
                   (isdigit(static_cast<unsigned char>(str[i])) ||
                    str[i] == '.' ||
                    str[i] == ':'))
            {
                i++;
            }

            continue;
        }

        size_t start = i;

        while (i < str.length() &&
               (isdigit(static_cast<unsigned char>(str[i])) ||
                str[i] == '.' ||
                str[i] == ':'))
        {
            i++;
        }

        size_t end = i;

        i = start;

        unsigned long octets[4];
        bool valid = true;

        for (int octetIndex = 0; octetIndex < 4; octetIndex++)
        {
            if (i >= end ||
                !isdigit(static_cast<unsigned char>(str[i])))
            {
                valid = false;
                break;
            }

            if (str[i] == '0')
            {
                if (i + 1 < end &&
                    isdigit(static_cast<unsigned char>(str[i + 1])))
                {
                    valid = false;
                    break;
                }
            }

            unsigned long value = 0;
            int digitCount = 0;

            while (i < end &&
                   isdigit(static_cast<unsigned char>(str[i])))
            {
                if (digitCount == 3)
                {
                    valid = false;
                    break;
                }

                int digit = str[i] - '0';
                value = value * 10 + digit;

                digitCount++;
                i++;
            }

            if (!valid)
            {
                break;
            }

            if (value > 255)
            {
                valid = false;
                break;
            }

            octets[octetIndex] = value;

            if (octetIndex < 3)
            {
                if (i >= end || str[i] != '.')
                {
                    valid = false;
                    break;
                }

                i++;
            }
        }

        if (!valid)
        {
            continue;
        }

        int port = -1;

        if (i < end && str[i] == ':')
        {
            i++;

            if (i >= end ||
                !isdigit(static_cast<unsigned char>(str[i])))
            {
                continue;
            }

            if (str[i] == '0')
            {
                if (i + 1 < end &&
                    isdigit(static_cast<unsigned char>(str[i + 1])))
                {
                    continue;
                }
            }

            unsigned long portValue = 0;
            int portDigits = 0;

            while (i < end &&
                   isdigit(static_cast<unsigned char>(str[i])))
            {
                if (portDigits == 5)
                {
                    valid = false;
                    break;
                }

                int digit = str[i] - '0';
                portValue = portValue * 10 + digit;

                portDigits++;
                i++;
            }

            if (!valid || portValue > 65535)
            {
                continue;
            }

            port = static_cast<int>(portValue);
        }

        if (i != end)
        {
            continue;
        }

        unsigned long address =
            (octets[0] << 24) |
            (octets[1] << 16) |
            (octets[2] << 8) |
            octets[3];

        outAddress = address;
        outPort = port;

        return true;
    }

    return false;
}


int main()
{
    string input;

    while (true)
    {
        cout << "Enter a string (or 'END' to quit): ";
        getline(cin, input);

        if (input == "END")
        {
            cout << "Program terminated." << endl;
            break;
        }

        unsigned long address;
        int port;

        bool found = extractIPv4(input, address, port);

        if (!found)
        {
            cout << "Invalid input: no valid IPv4 address found" << endl;
            continue;
        }

        unsigned long a = (address >> 24) & 255;
        unsigned long b = (address >> 16) & 255;
        unsigned long c = (address >> 8) & 255;
        unsigned long d = address & 255;

        cout << "Extracted IPv4 address: "
             << a << "."
             << b << "."
             << c << "."
             << d
             << " (decimal value: "
             << address
             << ", port: ";

        if (port == -1)
        {
            cout << "none";
        }
        else
        {
            cout << port;
        }

        cout << ")" << endl;
    }

    return 0;
}
