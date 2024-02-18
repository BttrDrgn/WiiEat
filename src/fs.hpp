#ifndef _FS_H_
#define _FS_H_

class fs
{
    public:
        static void init()
        {
	        fatInitDefault(); // Initialize file system
        }
};

#endif