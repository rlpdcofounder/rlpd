#include "../include/rlpd.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    PRNFile database;

    database.add_tag("ahegao");
    database.add_tag("gay");
    database.add_tag("ntr");

    database.add_entry(entry_t({2}, {'a', 'b', 'c'}));

    std::cout << "added entry." << std::endl;

    database.write("database_example.prn");

    return 0;
}
