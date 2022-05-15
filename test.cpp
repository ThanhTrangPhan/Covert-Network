//
// Created by demo on 5/15/22.
//
//
// Created by demo on 5/15/22.
//

#include <bits/stdc++.h>
using namespace std;

// Function to convert decimal no.
// to hexadecimal number
string Hex(int num)
{
    // map for decimal to hexa, 0-9 are
    // straightforward, alphabets a-f used
    // for 10 to 15.
    map<int, char> m;

    char digit = '0';
    char c = 'a';

    for (int i = 0; i <= 15; i++) {
        if (i < 10) {
            m[i] = digit++;
        }
        else {
            m[i] = c++;
        }
    }

    // string to be returned
    string res = "";

    // check if num is 0 and directly return "0"
    if (!num) {
        return "0";
    }
    // if num>0, use normal technique as
    // discussed in other post
    if (num > 0) {
        while (num) {
            res = m[num % 16] + res;
            num /= 16;
        }
    }
        // if num<0, we need to use the elaborated
        // trick above, lets see this
    else {
        // store num in a u_int, size of u_it is greater,
        // it will be positive since msb is 0
        u_int n = num;

        // use the same remainder technique.
        while (n) {
            res = m[n % 16] + res;
            n /= 16;
        }
    }

    return res;
}

// Driver Code
int main()
{
    int x = 134, y = -1, z = -234;

    cout << "Hexa representation for" << endl;
    cout << x << " is " << Hex(x) << endl;
    cout << y << " is " << Hex(y) << endl;
    cout << z << " is " << Hex(z) << endl;

    return 0;
}