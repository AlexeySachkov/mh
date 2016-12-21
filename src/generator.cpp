#include <iostream>
#include <fstream>
#include <string>

#include "mh.hpp"

using namespace std;

struct arguments_t
{
    string output_filename;

    arguments_t()
        : output_filename("")
    {};
};

bool parse_arguments(int, char* [], arguments_t&);

int main(int argc, char* argv[])
{
	arguments_t args;
	const uint_t N = 17;
	const uint_t M = 126443;
	const key_t private_key = {3, 4, 7, 15, 30, 60, 120, 240, 480, 960, 1920, 3840, 7680, 15360, 30720, 61440};

    if (!parse_arguments(argc, argv, args))
    {
        return -1;
    }

    string public_key_filename = args.output_filename + string(".public");
    string private_key_filename = args.output_filename + string(".private");

    ofstream private_key_file(private_key_filename.c_str());

	for (uint_t i = 0; i < BLOCK_SIZE; ++i)
	{
		private_key_file << private_key[i] << " ";
	}
	private_key_file << endl;
    private_key_file << N << endl;
    private_key_file << M << endl;
    private_key_file.close();

    key_t public_key = generate_public_key(private_key, N, M);

    ofstream public_key_file(public_key_filename.c_str());

	for (uint_t i = 0; i < BLOCK_SIZE; ++i)
	{
		public_key_file << public_key[i] << " ";
	}
	public_key_file << endl;
    public_key_file.close();
}

bool parse_arguments(int argc, char* argv[], arguments_t &args)
{
    for (int i = 1; i < argc; ++i)
    {
        string current = argv[i];

        if (current == "-o" || current == "--output")
        {
            if (i + 1 < argc)
            {
                args.output_filename = argv[i + 1];
                ++i;
            }
            else
            {
                cout << "Output file is missed!" << endl;
                return false;
            }
        }
    }

    return true;
}