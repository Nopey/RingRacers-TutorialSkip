#include "g_gamedata.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <cstdio>

using namespace std;
using namespace srb2;
using nlohmann::json;

void modify_gamesave(GamedataJson& js)
{
	js.milestones.finishedtutorialchallenge = true;
}

int main(int argc, char const* const* argv)
{
	string filename = "ringdata.dat";
	if (argc == 2)
	{
		filename = argv[1];
	}
	else
	{
		using namespace std::chrono_literals;
		cerr << "usage: kkdedit ringdata.dat\n"
			"(or drag & drop ringdata.dat into this program)\n";
		this_thread::sleep_for(2000ms);
		return 1;
	}


	vector<uint8_t> raw_bytes;
	char const header[] = {
		0x21, 0xD3, 0x5E, 0xBA, 0x00, 0x00
	};
	{
		// read raw bytes
		ifstream ifile(filename, ifstream::ate | ios::binary);
		ifile.unsetf(ios::skipws);
		auto filesize = ifile.tellg();
		if (filesize < sizeof header)
		{
			cerr << "empty file is not a save\n";
			return 1;
		}
		ifile.seekg(sizeof header);
		// raw_bytes.resize(filesize);
		raw_bytes.insert(raw_bytes.begin(),
			istream_iterator<uint8_t>(ifile),
			istream_iterator<uint8_t>());
		ifile.close();
	}

#if 0
	printf("%d bytes\n", raw_bytes.size());
	for (int x = 0; x < 8; x++)
	{
		printf("%x\n", raw_bytes[x]);
	}
	return 0;
#endif

	// parse ubjson
	json parsed = json::from_ubjson<uint8_t>(raw_bytes.data(), raw_bytes.size());
	GamedataJson js = parsed.template get<GamedataJson>();


	modify_gamesave(js);


	// write back to file
	vector<uint8_t> ubjson = json::to_ubjson(js);
	ofstream ofile(filename, ofstream::binary);
	ofile.write(header, sizeof header);
	ofile.write((char const*)ubjson.data(), ubjson.size());

	return 0;
}
