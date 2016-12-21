#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "mh.hpp"

using namespace std;

enum mode_t { ENCRYPT, DECRYPT, UNDEF };

struct arguments_t
{
    mode_t mode;
    string source_filename;
    string result_filename;
    string key_filename;

    arguments_t()
        : mode(UNDEF)
        , source_filename("")
        , result_filename("")
        , key_filename("")
    {};
};

bool parse_arguments(int, char* [], arguments_t&);
blocks_t read_file(const string&);
numbers_t read_numbers_file(const string&);
void write_file(const string&, const blocks_t&);
void write_numbers_file(const string&, const numbers_t&);
key_t read_public_key_file(const string&);
key_t read_private_key_file(const string&, uint_t&, uint_t&);

int main(int argc, char* argv[])
{
	arguments_t args;

	if (!parse_arguments(argc, argv, args))
    {
        return -1;
    }

    if (args.mode == ENCRYPT)
    {
    	key_t public_key = read_public_key_file(args.key_filename);
        blocks_t blocks = read_file(args.source_filename);
        numbers_t encrypted(blocks.size());

        for (size_t i = 0; i < blocks.size(); ++i)
        {
            encrypted[i] = encrypt_block(blocks[i], public_key);
        }

        write_numbers_file(args.result_filename, encrypted);
        cout << "encrypted!" << endl;
    }
    else if (args.mode == DECRYPT)
    {
    	uint_t N, M;
    	key_t private_key = read_private_key_file(args.key_filename, N, M);
        numbers_t numbers = read_numbers_file(args.source_filename);
        blocks_t decrypted(numbers.size());

        for (size_t i = 0; i < numbers.size(); ++i)
        {
            decrypted[i] = decrypt_number(numbers[i], private_key, N, M);
        }

        write_file(args.result_filename, decrypted);
        cout << "decrypted!" << endl;
    }

	return 0;
}

bool parse_arguments(int argc, char* argv[], arguments_t &args)
{
    for (int i = 1; i < argc; ++i)
    {
        string current = argv[i];

        if (current == "-k" || current == "--key")
        {
            if (i + 1 < argc)
            {
                args.key_filename = argv[i + 1];
                ++i;
            }
            else
            {
                cout << "Key filename is missed!" << endl;
                return false;
            }
        }
        else if (current == "-o" || current == "--output")
        {
            if (i + 1 < argc)
            {
                args.result_filename = argv[i + 1];
                ++i;
            }
            else
            {
                cout << "Output file is missed!" << endl;
                return false;
            }
        }
        else if (current == "-s" || current == "--source")
        {
            if (i + 1 < argc)
            {
                args.source_filename = argv[i + 1];
                ++i;
            }
            else
            {
                cout << "Source file is missed!" << endl;
                return false;
            }
        }
        else if (current == "-m" || current == "--mode")
        {
            if (i + 1 < argc)
            {
                string _mode = argv[i + 1];
                if (_mode == "encrypt")
                {
                    args.mode = ENCRYPT;
                }
                else if (_mode == "decrypt")
                {
                    args.mode = DECRYPT;
                }
                else
                {
                    cout << "Unknown mode " << _mode << endl;
                    return false;
                }
                ++i;   
            }
            else
            {
                cout << "Mode is missed!" << endl;
                return false;
            }
        }
        else
        {
            cout << "Unknown argument " << argv[i] << endl;
            return false;
        }
    }

    return true;
}

blocks_t read_file(const string &filename)
{
    ifstream file(filename.c_str(), fstream::in | fstream::binary);

    file.seekg(0, file.end);
    size_t length = file.tellg();
    file.seekg(0, file.beg);

    constexpr auto SYMBOLS_IN_BLOCK = BLOCK_SIZE / 8;
    size_t blocks_count = length / SYMBOLS_IN_BLOCK + (length % SYMBOLS_IN_BLOCK > 0);
    blocks_t blocks(blocks_count);

    file.read((char *)&blocks.front(), length);
    // close file
    file.close();

    return blocks;
}

numbers_t read_numbers_file(const string &filename)
{
    ifstream file(filename.c_str());

   	size_t size;
   	file >> size;
   	numbers_t numbers(size);

    for (size_t i = 0; i < size; ++i)
    {
    	file >> numbers[i];
    }

    file.close();

    return numbers;
}

void write_file(const string &filename, const blocks_t &blocks)
{
    ofstream file(filename.c_str(), fstream::binary);

    file.write((char *)&blocks.front(), blocks.size() * sizeof(block_t));
    // close file
    file.close();
}

void write_numbers_file(const string &filename, const numbers_t &numbers)
{
    ofstream file(filename.c_str());

   	file << numbers.size() << " ";

    for (size_t i = 0; i < numbers.size(); ++i)
    {
    	file << numbers[i] << " ";
    }

    file.close();
}

key_t read_public_key_file(const string &filename)
{
	ifstream file(filename.c_str());

   	key_t public_key;

    for (uint_t i = 0; i < BLOCK_SIZE; ++i)
    {
    	file >> public_key[i];
    }

    file.close();

    return public_key;
}

key_t read_private_key_file(const string &filename, uint_t &N, uint_t &M)
{
	ifstream file(filename.c_str());

   	key_t private_key;

    for (uint_t i = 0; i < BLOCK_SIZE; ++i)
    {
    	file >> private_key[i];
    }

    file >> N;
    file >> M;

    file.close();

    return private_key;
}
