#include<bits/stdc++.h>
using namespace std;

int main()
{
    string com;
    int a = 0;

    cout << "Enter comment: ";
    getline(cin, com);

    if (com[0] == '/')
    {
        if (com[1] == '/')
        {
            cout << "It is a comment" << endl;
        }
        else if (com[1] == '*')
        {
            for (size_t i = 2; i < com.length() - 1; ++i)
            {
                if (com[i] == '*' && com[i + 1] == '/')
                {
                    cout << "It is a comment" << endl;
                    a = 1;
                    break;
                }
            }
            if (a == 0)
                cout << "It is not a comment" << endl;
        }
        else
        {
            cout << "It is not a comment" << endl;
        }
    }
    else
    {
        cout << "It is not a comment" << endl;
    }

    return 0;
}

