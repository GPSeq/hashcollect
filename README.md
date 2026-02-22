# hashcollect
Collection of some hash functions

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

```bash
Int: 123456789
Float: 3.14159
String: ACCGTAAATTTGCCCCCCX
FNV-1a-32
  int  : 0xa93bb346
  float: 0x87aa7230
  str  : 0x7746e3e4
FNV-1a-64
  int  : 0x09ddda58da364874
  float: 0x87e96bd298024c2a
  str  : 0x370f8518ed8aa727
DJB2-32
  int  : 0x49076c7c
  float: 0x97e3d47c
  str  : 0xf73ae30b
SDBM-32
  int  : 0x64a05c36
  float: 0x6cbd79a6
  str  : 0xf210aadf
Jenkins-OAAT-32
  int  : 0x42bcd882
  float: 0x11219ecf
  str  : 0xcdf8de43
Murmur2-32
  int  : 0xd3357788
  float: 0x7fd565f8
  str  : 0x0d77355a
Murmur3-x86-32
  int  : 0xaf2621bf
  float: 0x62fe623f
  str  : 0xaa7ea954
Murmur3-x64-128
  int  : 0xe4acbe68835748ffdcfe39b87ee4259e
  float: 0x20c3df807e3d6a25fa3fb5336e67b3f2
  str  : 0xcd00742eac0e49755ec767b40a9085fc
xxHash32
  int  : 0xee850ab2
  float: 0x8067d265
  str  : 0x601f9da4
xxHash64
  int  : 0xcafc526fc5f65083
  float: 0xd6d32d2aa2ab0b6e
  str  : 0xe8b5873b0f3e94ab
SipHash-2-4
  int  : 0x4811969c17cb24d5
  float: 0x90a58661f1a48268
  str  : 0xf53cb0c80e394594
CRC32-IEEE
  int  : 0x9e6cdf18
  float: 0x00749634
  str  : 0x6a7ba70c
CRC32C
  int  : 0xc5a603f5
  float: 0x9ef65d6d
  str  : 0x7c2bb8c0
Adler32
  int  : 0x45017c03
  float: 0x73016b04
  str  : 0x5305cb34
CRC64-ECMA
  int  : 0xbfcdcd79aeeac099
  float: 0xbf08ae0fc06a156e
  str  : 0x69fbe6fedd8aa2c3
Pearson64
  int  : 0x1347f4101f275f5a
  float: 0xeb06fc79152b57a6
  str  : 0x923b4267a9f0703c
Buzhash32
  int  : 0x2fcb91e0
  float: 0xc237c988
  str  : 0x1612a655
RabinKarp64
  int  : 0xe4a1de05090bca86
  float: 0xd4308a256157f6c6
  str  : 0x82937cb31436593a
MD5
  int  : 0x623ad5eda301ae082d466141a7f6dcaf
  float: 0xad14c956e63f2313841f85c817947889
  str  : 0x4e5b0b3b13b87e107f119cb03dabbf7c
SHA-256
  int  : 0x72de837c74b40716d430c711eebde10ff965fcc4a70c98e63a233ff36eebd6a1
  float: 0xf1af6ed5ca0314eaaccafa1bb8b213e19029ad6720a86c5f65f4a9df137e430a
  str  : 0x1e976226fbce9774aab5dcd3548f8a9d9ae21c57f2b2d0a82579ea4523915714
```