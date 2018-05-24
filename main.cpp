#include "header.h"

int main() {
    int position = 0, key = 0;
    string command;
    SList l;

    while (Fin >> command)
    {
        if (command == "insert")
        {
            Fin >> position >> key;
            l.listInsert(position, key);
        }
        else if (command == "delete")
        {
            Fin >> position;
            l.listDelete(position);
        }
        else if (command == "max")
            Fout << l.listMax() << endl;
        else if (command == "min")
            Fout << l.listMin() << endl;
        else if (command == "print")
            l.listPrint();
        else if (command == "edit")
        {
            Fin >> position >> key;
            l.listEdit(position, key);
        }
        else if (command == "reverse")
            l.listReverse();
        else if (command == "searchall")
        {
            Fin >> key;
            l.listSearchall(key);
        }
        else if (command == "searchfirts")
        {
            Fin >> key;
            l.listSearchfirst(key);
        }
    }

    return 0;
}