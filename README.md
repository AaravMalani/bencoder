# bencoder: A parser for BitTorrent .torrent files

## Installation
- Go to the [latest release](https://github.com/AaravMalani/bencoder/releases/latest)
- Download the RPM file for Red-Hat based Linux Systems, DEB file for Debian based systems or the DLL or LIB file for Windows
```sh
# Ubuntu/Debian
sudo dpkg -i <file>.deb

# RedHat Based Linux
sudo rpm -i <file>.rpm
```
For Windows, you have to link it into the Visual Studio Project using the Linker menu in the Properties of the Project.
Then download the [`bencoder.h`](https://raw.githubusercontent.com/AaravMalani/bencoder/main/include/bencoder.h) file and include that as a header file.

## Usage
```c
#include <bencoder.h>

char* bencodeTypes[4] = {
    "INTEGER", // data = long long* (A pointer not a constant value)
    "STRING", // data = char*
    "LIST", // data = struct BencodeStructure* (contiguous structs not to be confused with array of pointers)
    "DICTIONARY" // data = struct BencodeMap* (contiguous structs not to be confused with array of pointers)
};


int main(int argc, char **argv)
{
    if (argc != 2){
        printf("Usage: %s <torrentfile>\n", argv[0]);
        return 1;
    }
    // Load file
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        perror(argv[1]);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    rewind(f);
    char *data = malloc(fsize);
    fread(data, fsize, 1, f);
    fclose(f);

    // Parse bencode
    struct BencodeStructure* struc = parseBencode(data, fsize);
    free(data);
    if (!struc) // Error
    {
        printf("Error parsing file\n");
        return 1;
    }
    printf("Type: %s\n", bencodeTypes[struc->type]);
    freeBencode(struc);
}   
```



