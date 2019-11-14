# FMGon + fmgon_test

This is an updated version of fmgon_test which can take custom "Timbres" (patch definitions) and output a test sound to help with audio preview for [BambooCopy](https://github.com/nobuyukinyuu/bambooCopy).  The project has also been updated to support 64 bit architectures on Windows and dependency links updated accordingly.

To compile, you need the OpenAL SDK installed on your system. OpenAL-soft is not likely to work....

FreeALUT is included in the project as it is also a dependency.

## Proposed features
* Support for custom MML strings
* Better error handling
* LFO support would be nice (but is NOT planned)


## Acknowledgements
This project utilizes fmgen by cisc, an older FM chip emulator.  They acknowledge the following people:
* Tatsuyuki Satoh (fm.c)
* Hiromitsu Shioya (ADPCM-A)
* DMP-SOFT. (OPNB)
* KAJA (test program)


### Original ReadMe.txt follows below.
------


```
                            fmgon
                        -------------

このフォルダーは、ciscさん (cisc@retropc.net) の「FM Sound Generator」を
片山博文MZ (katayama.hirofumi.mz@gmail.com) が改造したものです。
詳しくは、ファイル「OldReadMe.txt」をご覧下さい。
主な変更点についてはファイル「ChangeLog.txt」をご覧下さい。

使用する場合は、ファイル「OldReadMe.txt」の
【著作権、免責規定】に従って下さい。

Copyright (C) 2015-2016 Katayama Hirofumi MZ.
Copyright (C) 2015-2016 okrt. All Rights Reserved.

/////////////////////////////////////////////////////
// 片山博文MZ (katahiromz) [蟻]
// ホームページ http://katahiromz.web.fc2.com/
// 掲示板       http://katahiromz.bbs.fc2.com/
// メール       katayama.hirofumi.mz@gmail.com
/////////////////////////////////////////////////////
```
