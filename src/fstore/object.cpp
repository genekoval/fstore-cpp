#include <repo.h>

#include <commline/commands.h>
#include <iostream>

#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <service.h>

#include <string>

using std::cout;
using std::endl;
using std::string;

void commline::commands::add(const commline::cli& cli) {
        using namespace CryptoPP;

        HexEncoder encoder(new FileSink(cout));

        auto message = cli.args().front();
        string digest;

        SHA256 hash;
        hash.Update((const byte*) message.data(), message.size());
        digest.resize(hash.DigestSize());
        hash.Final((byte*) &digest[0]);

        cout << "Message: " << message << endl;

        cout << "Digest: ";
        StringSource(digest, true, new Redirector(encoder));
        cout << endl;
}
