//
//  main.cpp
//  p3-logman
//
//  Created by John Corio on 11/4/19.
//  Copyright © 2019 John Corio. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <unordered_map>
#include <algorithm>
//#include <sstream>
#include "string.h" // for strcasecmp
#include "entry.hpp"
#include "xcode_redirect.hpp"

// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

struct EntryFunctor {
    bool operator() (const Entry* e1, const Entry* e2)
    {
        if (e1->TSlong < e2->TSlong)
        {
            return true;
        }
        if (e1->TSlong > e2->TSlong)
        {
            return false;
        }
        int compare = strcasecmp(e1->category.c_str(), e2->category.c_str());
        if (compare < 0)
        {
            return true;
        }
        else if (compare > 0)
        {
            return false;
        }
        return (e1->entryID < e2->entryID);
    }
};

struct MasterFunctor {
    bool operator() (const Entry &e1, const Entry &e2)
    {
        if (e1.TSlong < e2.TSlong)
        {
            return true;
        }
        if (e1.TSlong > e2.TSlong)
        {
            return false;
        }
        int compare = strcasecmp(e1.category.c_str(), e2.category.c_str()); // compare "z" to "z!" gave compare == -33
        if (compare < 0)
        {
            return true;
        }
        else if (compare > 0)
        {
            return false;
        }
        return (e1.entryID < e2.entryID);
     
    }
};
/// Could optimize this by just returning the comparison
struct EqualRangeFunctor {
    bool operator() (const Entry &e, const long long timestamp)
    {
        
        /*if*/ return (!(e.TSlong >= timestamp));
        /*{
            return false;
        }
        else
        {
            return true;
        }
        */
    }
    bool operator() (const long long timestamp, const Entry &e)
    {
        
        /*if*/ return (!(e.TSlong <= timestamp));
        /*{
            return false;
        }
        else
        {
            return true;
        }*/
    }
};


class MasterLogManip {
public:
    
    // EXCERPT LIST EDITING FUNCTIONS
    //
    //
    // EXCERPT LIST EDITING FUNCTIONS
    
    // command A
    void append_entryID(int position)
    {
        if (static_cast<size_t>(position) < masterlist.size() && position >= 0)
        {
            lastcommandSort = false;
            Entry* eptr = &(masterlist[entryIDlist[position]]);
            excerptlist.push_back(eptr);
            std::cout << "log entry " << position << " appended\n";
        }
    }
    
    // command R
    void append_recentsearch()
    {
        // append all recently searched entries
        
        // nothing to add, return
        if (recentsearch.size() == 0)
        {
            std::cout << "0 log entries appended\n";
            return;
        }
        // add recently searched entries
        else
        {
            lastcommandSort = false;
            excerptlist.insert(excerptlist.end(), recentsearch.begin(), recentsearch.end());
        }
        
        // print statement
        std::cout << recentsearch.size() << " log entries appended\n";
        
        // traverse container, create and add pointers to end?
    }
    
    // command D
    void delete_entry(int position)
    {
        // delete by excerptlist #
        // out of bounds check
        if (static_cast<size_t>(position) >= excerptlist.size() || position < 0)
        {
            return;
        }
        lastcommandSort = false;
        // erase if we good
        std::cout << "Deleted excerpt list entry " << position << "\n";
        excerptlist.erase(excerptlist.begin() + position);
    }
    
    // command B erase, then insert with temp iterator
    void move_begin(int position)
    {
        // invalid position check
        if (position < 0 || static_cast<size_t>(position) >= excerptlist.size())
        {
            return;
        }
        lastcommandSort = false;
        // put entry to move into a temporary variable
        Entry* temp = excerptlist[position];
        // erase it
        excerptlist.erase(excerptlist.begin() + position);
        // move to front by entryID
        excerptlist.push_front(temp);
        // print
        std::cout << "Moved excerpt list entry " << position << "\n";
    }
    
    // command E
    void move_end(int position)
    {
        // invalid position check
        if (position < 0 || static_cast<size_t>(position) >= excerptlist.size())
        {
            return;
        }
        lastcommandSort = false;
        // put entry to move into a temporary variable
        Entry* temp = excerptlist[position];
        // erase it
        excerptlist.erase(excerptlist.begin() + position);
        // move to front by entryID
        excerptlist.push_back(temp);
        // print
        std::cout << "Moved excerpt list entry " << position << "\n";
    }
    
    // command S
    void sort_excerptlist()
    {
        // sort excerpt list by our functor
        std::cout << "excerpt list sorted\n";
        
        // empty check
        if (excerptlist.empty())
        {
            std::cout << "(previously empty)\n";
            return;
        }
        // previous ordering print
        std::cout << "previous ordering:\n";
        std::cout << 0 << "|" << excerptlist[0]->entryID << "|" << excerptlist[0]->timestamp << "|" << excerptlist[0]->category << "|" << excerptlist[0]->msg << "\n";
        std::cout << "...\n";
        std::cout << excerptlist.size() - 1 << "|" << excerptlist[excerptlist.size() - 1]->entryID << "|" << excerptlist[excerptlist.size() - 1]->timestamp << "|" << excerptlist[excerptlist.size() - 1]->category << "|" << excerptlist[excerptlist.size() - 1]->msg << "\n";
        
        // actually sort the damn thing if we haven't sorted recently (TIME SAVE)
        if (!lastcommandSort)
        {
            sort(excerptlist.begin(), excerptlist.end(), EntryFunctor());
        }
        
        // new ordering print
        std::cout << "new ordering:\n";
        std::cout << 0 << "|" << excerptlist[0]->entryID << "|" << excerptlist[0]->timestamp << "|" << excerptlist[0]->category << "|" << excerptlist[0]->msg << "\n";
        std::cout << "...\n";
        std::cout << excerptlist.size() - 1 << "|" << excerptlist[excerptlist.size() - 1]->entryID << "|" << excerptlist[excerptlist.size() - 1]->timestamp << "|" << excerptlist[excerptlist.size() - 1]->category << "|" << excerptlist[excerptlist.size() - 1]->msg << "\n";
        lastcommandSort = true;
    }
    
    // command L
    void clear_excerptlist()
    {
        std::cout << "excerpt list cleared\n";
        // empty check
        if (excerptlist.empty())
        {
            std::cout << "(previously empty)\n";
            return;
        }
        // print excerpt list
        std::cout << "previous contents:\n";
        std::cout << 0 << "|" << excerptlist[0]->entryID << "|" << excerptlist[0]->timestamp << "|" << excerptlist[0]->category << "|" << excerptlist[0]->msg << "\n";
        std::cout << "...\n";
        std::cout << excerptlist.size() - 1 << "|" << excerptlist[excerptlist.size() - 1]->entryID << "|" << excerptlist[excerptlist.size() - 1]->timestamp << "|" << excerptlist[excerptlist.size() - 1]->category << "|" << excerptlist[excerptlist.size() - 1]->msg << "\n";
        // clear the excerpt list completely (using STL ERASE??)
        excerptlist.clear();
        lastcommandSort = false;
    }
    
    
    
    // SEARCHING FUNCTIONS
    //
    //
    // SEARCHING FUNCTIONS
    
    // SEARCHES CAN RETURN 0 RESULTS, PRINT THAT IF IT DOES...
    
    
    // command T
    void timestamp_range(std::string &t1, std::string &t2)
    {
        // clear recent search
        recentsearch.clear();
        auto iter1 = lower_bound(masterlist.begin(), masterlist.end(), convert_to_longlong(t1), EqualRangeFunctor());
        auto iter2 = upper_bound(iter1, masterlist.end(), convert_to_longlong(t2), EqualRangeFunctor());
        
        // insert at the end, sorted order, CREATE POINTERS TO ENTRIES OF iter1 AND iter2
        int counter = 0;
        while (iter1 != iter2)
        {
            Entry* eptr = &(*iter1);
            recentsearch.push_back(eptr);
            counter++;
            iter1++;
        }
        
        std::cout << "Timestamps search: " << counter << " entries found\n";
    }
    
    // command M
    void timestamp_single(std::string &t)
    {
        // clear recent search
        recentsearch.clear();
        // pair of stuff
        auto pianohonk = equal_range(masterlist.begin(), masterlist.end(), convert_to_longlong(t), EqualRangeFunctor());
        // first is NOT less than t
        // second is first GREATER than t
        
        
        // manipulate pair
                
        auto it = pianohonk.first;
        while (it != pianohonk.second)
        {
            Entry* eptr = &(*it);
            recentsearch.push_back(eptr);
            it++;
        }
        
        std::cout << "Timestamp search: " << pianohonk.second - pianohonk.first << " entries found\n";
    }

    // command C
    void category_search(std::string &cate)
    {
        // clear old search
        recentsearch.clear();
        // search for all entries in cmap with .find(cate), append to recentsearch
        for (size_t i = 0; i < cate.size(); i++)
        {
            // ADDED IF TO OPTIMIZE AND AVOID IS LOWER, CAN CHANGE IF NECESSRY
            if (isalnum(cate[i]))
            {
                cate[i] = static_cast<char>(std::tolower(cate[i]));
            }
        }
        // cate not found, print 0 entries found
        if (cmap.find(cate) == cmap.end())
        {
            std::cout << "Category search: 0 entries found\n";
        }
        // cate is found, print and append to recentsearch
        else
        {
            //std::vector<Entry*> entries;
            //entries = cmap[cate];
            
            /// could avoid using entries, just do cmap[cate]
            
            std::cout << "Category search: " << cmap[cate].size() << " entries found\n";
            // append vector to recentsearch, cmap[cate].begin();
            recentsearch.insert(recentsearch.begin(), cmap[cate].begin(), cmap[cate].end());
        }
    }
    
    // command K
    void keyword_search(std::string &pphead)
    {
        // clear recent search
        recentsearch.clear();
        std::vector<std::string> keywords;
        parse(pphead, keywords);
        
        
        // doing keyword.find(word) and storing that iterator could be helpful?
        
        // iterate over vector and create a set intersection vector of entry pointers, sort the vector, append based on range to recentsearch, pass in EntryFunctor() to set_intersection
        std::vector<Entry*> entries;
        if (keywords.size() == 1)
        {
            if (kmap.find(keywords[0]) != kmap.end())
            {
                entries.reserve(kmap[keywords[0]].size());
                entries = kmap[keywords[0]];
                std::cout << "Keyword search: " << entries.size() << " entries found\n";
                recentsearch.insert(recentsearch.begin(), entries.begin(), entries.end());
                return;
            }
        }
        else
        {
            if (kmap.find(keywords[0]) == kmap.end() || kmap.find(keywords[1]) == kmap.end())
            {
                std::cout << "Keyword search: 0 entries found\n";
                return;
            }
            std::set_intersection(kmap[keywords[0]].begin(), kmap[keywords[0]].end(), kmap[keywords[1]].begin(), kmap[keywords[1]].end(), back_inserter(entries), EntryFunctor());
            for (size_t i = 2; i < keywords.size(); i++)
            {
                if (kmap.find(keywords[i]) == kmap.end())
                {
                    std::cout << "Keyword search: 0 entries found\n";
                    return;
                }
                else
                {
                    std::vector<Entry*> temp = entries;
                    entries.clear();
                    std::set_intersection(kmap[keywords[i]].begin(), kmap[keywords[i]].end(), temp.begin(), temp.end(), back_inserter(entries), EntryFunctor());
                    // FUNCTOR NEEDS TO BE MONITORED HERE
                    // WATCH BACK INSERTER AND ENTRIES CAREFULLY
                    // MAYBE MAKE SURE POINTERS ARE POINTING TO SAME PLACE USING FUNCTOR??
                }
            }
        }
        // insert and print
        recentsearch.insert(recentsearch.begin(), entries.begin(), entries.end());
        std::cout << "Keyword search: " << entries.size() << " entries found\n";
    }
    
    
    
    // PRINTING FUNCTIONS
    //
    //
    // PRINTING FUNCTIONS
    
    // command P
    void print_excerptlist()
    {
        auto it = excerptlist.begin();
        int counter = 0;
        while (it != excerptlist.end())
        {
            std::cout << counter << "|" << (*it)->entryID << "|" << (*it)->timestamp << "|" << (*it)->category << "|" << (*it)->msg << "\n";
            it++;
            counter++;
        }
    }
    
    // commmand G
    void print_recentsearch()
    {
        auto it = recentsearch.begin();
        while (it != recentsearch.end())
        {
            std::cout << (*it)->entryID << "|" << (*it)->timestamp << "|" << (*it)->category << "|" << (*it)->msg << "\n";
            it++;
        }
    }
    
    
    
    
    // UTILITY FUNCTIONS
    //
    //
    // UTILITY FUNCTIONS
    void push_entry(Entry &entry)
    {
        masterlist.emplace_back(entry);
    }
    
    void mastersort()
    {
        std::sort(masterlist.begin(), masterlist.end(), MasterFunctor());
    }
    
    long long convert_to_longlong(std::string &s1)
    {
        s1.erase(s1.begin() + 2);
        s1.erase(s1.begin() + 4);
        s1.erase(s1.begin() + 6);
        s1.erase(s1.begin() + 8);
        return std::stoll(s1);
    }

    void create_maps()
    {
        for (size_t i = 0; i < masterlist.size(); i++)
        {
            // create entry pointer and temp category string, transform temp string
            Entry* eptr = &masterlist[i];
            
            // grab category string and transform to lowercase
            std::string cat = masterlist[i].category;
            // set tolower everything
            for (size_t i = 0; i < cat.size(); i++)
            {
                if (isalnum(cat[i]))
                {
                    cat[i] = static_cast<char>(std::tolower(cat[i]));
                }
                
            }
             
            cmap[cat].push_back(eptr);
            
            // vector for all words in category and message
            std::vector<std::string> logWords;
            parse(cat, logWords);
            // grab message and transform to lowercase
            std::string message = masterlist[i].msg;
            parse(message, logWords);
            
            // by this point we have our vector of all words contained in our category and message in vector logWords, now we iterate and use .find and operator[] to push_back our entries
            for (size_t i = 0; i < logWords.size(); i++)
            {
                // if not in map, create entry and pushback our Entry *
                if (kmap.find(logWords[i]) == kmap.end())
                {
                    kmap[logWords[i]].push_back(eptr);
                }
                // is in map
                else
                {
                    // check if last Entry* pushed is eptr, if not push it in
                    if (*(kmap[logWords[i]].end() - 1) != eptr) // GO OVER THIS COMPARISON
                    {
                        kmap[logWords[i]].push_back(eptr);
                    }
                }
            }
        }
    }
    
    // create vector, position -> original position
    void create_entryIDvector()
    {
        entryIDlist.resize(masterlist.size());
        for (size_t i = 0; i < masterlist.size(); i++)
        {
            entryIDlist[masterlist[i].entryID] = static_cast<int>(i);
        }
    }
    
    void parse(std::string &toParse, std::vector<std::string> &keywords)
    {
        size_t end = 0;
        for (size_t i = 0; i < toParse.size(); i++)
        {
           // FREEZE i if alphanumeric
           if (isalnum(toParse[i]))
           {
               toParse[i] = static_cast<char>(std::tolower(toParse[i]));
               end = i + 1;
               if (end == toParse.size())
               {
                   keywords.push_back(toParse.substr(i, end - i));
                   break;
               }
               else
               {
                   while (isalnum(toParse[end]))
                   {
                       toParse[end] = static_cast<char>(std::tolower(toParse[end]));
                       end++;
                       if (end == toParse.size())
                       {
                           //keywords.push_back(toParse.substr(i, end - i));
                           break;
                       }
                   }
                   keywords.push_back(toParse.substr(i, end - i));
                   i = end;
               }
           }
        }
        return;
    }

private:
    std::vector<Entry> masterlist;
    std::deque<Entry*> excerptlist; // possibly make this a vector and edit the b command, also have a previous sort check for it so repeated sorts don't occur
    std::unordered_map<std::string, std::vector<Entry*>> kmap;
    // create a pointer to entry in question, create a word key for each, intialize or pushback entry pointer to vector, could do indices if needed for space
    std::unordered_map<std::string, std::vector<Entry*>> cmap;
    std::vector<Entry*> recentsearch;
    std::vector<int> entryIDlist;
    
    bool lastcommandSort = false;
};

bool check_validtimestamp(std::string &str)
{
    if (str.size() != 14)
    {
        return false;
    }
    return true;
}

int main (int argc, char* argv[]) {
    // optimization line
    std::ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    // command line processing
    std::string cmdline = argv[1]; // CHANGE TO ZERO WHEN SUBMITTING
    if (cmdline == "--help" || cmdline == "-h")
    {
        std::cout << "logman program... try putting a file to actually run it nerd...\n";
        return(0);
    }
    
    MasterLogManip logman;
    
    // declare a file stream, read into Entry and create entry objects to push into master list
    std::ifstream inFS(cmdline);
    std::string timestamp, category, message;
    int counter = 0;
    while(getline(inFS, timestamp, '|'))
    {
        getline(inFS, category, '|');
        getline(inFS, message);
        // create an entry based on custom constructor of Entry data structure
        Entry readin = Entry(timestamp, category, message, counter);
        // pushback into master log list
        logman.push_entry(readin);
        counter++;
    }
    // close file
    inFS.close();
    std::cout << counter << " entries read\n";
    
    // sort master vector
    logman.mastersort();
    
    // create entryID vector
    logman.create_entryIDvector();
    
    // initialize maps for later
    logman.create_maps(); 
    
    // start processing user commands
    bool hasSearched = false;
    char command;
    do {
        std::cout << "% ";
        std::cin >> command;
        
        // process command, wrote them down in order of spec
        // searching commands
        if (command == 't')
        {
            hasSearched = true;
            std::string time1, time2;
            getline(std::cin, time1, '|');
            time1.erase(0, 1); // erase the space
            getline(std::cin, time2);
            if (check_validtimestamp(time1) && check_validtimestamp(time2))
            {
                logman.timestamp_range(time1, time2);
            }
        }
        else if (command == 'm')
        {
            hasSearched = true;
            std::string time;
            std::cin >> time;
            //time.erase(0, 1); // erase the space
            if (check_validtimestamp(time))
            {
                logman.timestamp_single(time);
            }
        }
        else if (command == 'c')
        {
            hasSearched = true;
            std::string category;
            getline(std::cin, category); // how to ignore leading whitespace??
            if (category[0] == ' ')
            {
                category.erase(0, 1);
            }
            logman.category_search(category);
        }
        else if (command == 'k')
        {
            hasSearched = true;
            std::string keywords;
            getline(std::cin, keywords);
            if (keywords[0] == ' ')
            {
                keywords.erase(0, 1);
            }
            // how to deal with duplicate words
            logman.keyword_search(keywords);
        }
        // editing the excerpt list
        else if (command == 'a')
        {
            int position = 0;
            std::cin >> position;
            logman.append_entryID(position);
        }
        else if (command == 'r')
        {
            if (!hasSearched)
            {
                std::cerr << "uhhhh yeah pls search somethin first bud...\n";
            }
            else
            {
                logman.append_recentsearch();
            }
        }
        else if (command == 'd')
        {
            int position = 0;
            std::cin >> position;
            logman.delete_entry(position);
        }
        else if (command == 'b')
        {
            int position = 0;
            std::cin >> position;
            logman.move_begin(position);
        }
        else if (command == 'e')
        {
            int position = 0;
            std::cin >> position;
            logman.move_end(position);
        }
        else if (command == 's')
        {
            logman.sort_excerptlist();
        }
        else if (command == 'l')
        {
            logman.clear_excerptlist();
        }
        // output commands
        else if (command == 'g')
        {
            logman.print_recentsearch();
        }
        else if (command == 'p')
        {
            logman.print_excerptlist();
        }
        // do nothing command!
        else if (command == '#')
        {
            std::string junk;
            getline(std::cin, junk);
        }
        
    } while (command != 'q');
}
