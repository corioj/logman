//
//  entry.hpp
//  p3-logman
//
//  Created by John Corio on 11/4/19.
//  Copyright © 2019 John Corio. All rights reserved.
//

#ifndef entry_hpp
#define entry_hpp

#include <stdio.h>
#include <string>
#include "string.h"

// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

struct Entry {
    Entry(std::string time, std::string cat, std::string msg, int IDnum) :
    timestamp(time), category(cat), msg(msg), entryID(IDnum)
    {
        // erase all colons
        time.erase(time.begin() + 2);
        time.erase(time.begin() + 4);
        time.erase(time.begin() + 6);
        time.erase(time.begin() + 8);
        
        TSlong = std::stoll(time);
    }
    
    std::string timestamp;
    std::string category;
    std::string msg;
    long long TSlong;
    int entryID;
};

#endif /* entry_hpp */
