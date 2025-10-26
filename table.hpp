#ifndef TABLE_HPP
#define TABLE_HPP

#define TABLE_SIZE 2

struct table_t{
    int buffer[TABLE_SIZE];
    int in;
    int out;
};

#endif